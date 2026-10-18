// Teaching example adapted from "03 - Processes.pdf", slide 13.
// Goal: make the parent print only AFTER its child has terminated.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic fork_wait.cpp -o fork_wait
// Run:   ./fork_wait

#include <cstddef>     // NULL: a null pointer, used here to ignore exit status.
#include <iostream>    // std::cout, std::cerr, and std::endl.
#include <sys/types.h> // pid_t: the integer type used for process IDs.
#include <sys/wait.h>  // wait(): collect a child's termination.
#include <unistd.h>    // fork() and getpid() on macOS/Linux.

int main() {
    // Create a child. Both processes continue here with different results:
    //   Parent: pid contains the child's actual PID, a positive number.
    //   Child:  pid contains 0, identifying the child branch.
    //   Failure: pid contains -1, and no child was created.
    pid_t pid = fork();

    if (pid > 0) {
        // Only the parent reaches this branch.
        // wait(NULL) blocks if no child has terminated yet. While blocked,
        // the parent does not execute its next statement.
        // If a child has already terminated, wait can collect it immediately.
        // This program creates only one child, so that is the child collected.
        // With multiple children, wait() can collect any of them.
        //
        // NULL means "do not store the child's exit-status information".
        // It does NOT mean "wait for no child".
        // On success, wait returns the TERMINATED CHILD'S PID, not its status.
        pid_t child = wait(NULL);

        // Check for an error before claiming that waiting succeeded.
        if (child == -1) {
            std::cerr << "wait failed" << std::endl;
            return 1;
        }

        // Example: if the child's actual PID is 820, child now contains 820.
        // The parent reaches these prints only after collecting termination.
        std::cout << "wait() returned child PID = " << child << std::endl;
        std::cout << "I am the parent!" << std::endl;
    } else if (pid == 0) {
        // Only the child reaches this branch. It skips the parent's wait().
        // getpid() is the actual PID; fork's return value in pid is still 0.
        std::cout << "I am the child! My actual PID = " << getpid()
                  << std::endl;

        // Printing alone does NOT unblock the parent. The child must finish.
        // This return ends the child's main() and terminates it successfully.
        return 0;
    } else {
        // Only the original process exists if fork failed.
        std::cerr << "Oops! fork failed." << std::endl;
        return 1;
    }

    // The parent finishes successfully after the wait and its prints.
    return 0;
}

// One example output (the PID will vary, but this order follows from wait):
// I am the child! My actual PID = 820
// wait() returned child PID = 820
// I am the parent!
//
// Trace: child prints -> child terminates -> wait returns -> parent prints.
// Practice: if the parent printed BEFORE calling wait(NULL), would the
// child's message still be guaranteed to appear before the parent's?
