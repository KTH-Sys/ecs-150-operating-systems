// Teaching example adapted from "03 - Processes.pdf", slide 16.
// Goal: create a child, run echo in it, and let the parent continue afterward.
// This is the basic pattern a shell uses for a foreground command.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic fork_exec_wait.cpp -o fork_exec_wait
// Run:   ./fork_exec_wait

#include <iostream>    // std::cout, std::cerr, and std::endl.
#include <sys/types.h> // pid_t: the integer type used for process IDs.
#include <sys/wait.h>  // wait(): collect a child's termination.
#include <unistd.h>    // fork() and execv() on macOS/Linux.

int main() {
    // Prepare the program path and its arguments. echo is NOT running yet.
    char command[] = "/bin/echo";
    char message[] = "ECS 150";

    // args[0] is the program name; args[1] is the message passed to echo.
    // nullptr marks the END of the argument array.
    // echo prints the message because that is echo's job.
    char *args[] = {command, message, nullptr};

    // STEP 1: create a child, initially running a copy of this program.
    // Both processes continue after fork(), with different return values.
    // Example: parent actual PID = 819; child actual PID = 820.
    //   Parent: pid contains 820 (the child's actual PID).
    //   Child:  pid contains 0 (a marker identifying the child).
    pid_t pid = fork();

    if (pid > 0) {
        // PARENT ONLY: skip the child's execv branch and wait here.
        // STEP 2 for parent: collect a child's termination.
        // This program has only one child, so wait collects that child.
        // If it is still running, the parent blocks. If it already ended,
        // wait can collect it immediately.
        // nullptr means we are ignoring the child's exit-status information.
        pid_t child = wait(nullptr);

        if (child == -1) {
            std::cerr << "wait failed!" << std::endl;
            return 1;
        }

        // On success, child contains the terminated child's PID (e.g., 820).
        // STEP 3 for parent: continue after the child has terminated.
        std::cout << "child done!" << std::endl;

    } else if (pid == 0) {
        // CHILD ONLY: skip the parent's wait branch.
        // STEP 2 for child: replace this program with /bin/echo.
        // The child keeps its PID (e.g., 820); execv creates NO new process.
        // Equivalent terminal command: /bin/echo "ECS 150"
        execv(command, args);

        // On success, echo prints the message and then terminates.
        // It NEVER returns to this old code, even after it finishes.
        // The lines below run ONLY if execv fails (it returns -1).
        std::cerr << "execv failed!" << std::endl;
        return 1; // Terminate the child with a failure status.

    } else {
        // fork returned -1: no child exists, so do not try to wait for one.
        std::cerr << "fork failed!" << std::endl;
        return 1;
    }

    // The parent finishes successfully after collecting the child.
    return 0;
}

// Successful output:
// ECS 150
// child done!
//
// Parent: fork -> wait blocks if needed -> collect child -> print -> finish.
// Child:  return from fork -> execv -> echo prints -> echo terminates.
// The OS may schedule either process first, but wait enforces this print order.
//
// IMPORTANT: "child done!" means the child terminated, not that echo succeeded.
// If execv fails, the child prints an error and returns 1. The parent still
// collects it and prints "child done!" because this example ignores exit status.
//
// Practice:
// 1. Without wait(), would "child done!" still be guaranteed to print second?
// 2. Why must execv run in the child if we want the original program to survive?
// 3. If echo replaces this program in PID 820, what is echo's process ID?
