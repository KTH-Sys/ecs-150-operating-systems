// Teaching example adapted from "03 - Processes.pdf", slide 11.
// Goal: use fork's RETURN VALUE to distinguish the parent from the child.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic distinguish_parent_child.cpp -o distinguish_parent_child
// Run:   ./distinguish_parent_child

#include <iostream>    // Standard output and error streams.
#include <sys/types.h> // pid_t: the integer type used for process IDs.
#include <unistd.h>    // fork() and getpid() on macOS/Linux.

int main() {
    // Read this as: create a child, then store fork's result in a variable.
    // pid_t = TYPE; pid = VARIABLE NAME; fork() = FUNCTION CALL.
    // After success, each process has its OWN variable named pid:
    //   Parent: pid holds the child's actual PID (a positive number).
    //   Child:  pid holds 0, a marker meaning "I am the child".
    // On failure: the original process gets -1; no child exists.
    pid_t pid = fork();

    // IMPORTANT: pid is fork's return value, NOT necessarily my own PID.
    // Example: parent actual PID = 819; child actual PID = 820.
    //   In parent: pid == 820, getpid() == 819.
    //   In child:  pid == 0,   getpid() == 820.

    if (pid > 0) {
        // Only the parent enters this branch.
        std::cout << "I am the parent! getpid() = " << getpid()
                  << ", fork returned " << pid << std::endl;
    } else if (pid == 0) {
        // Only the child enters this branch. Its actual PID is NOT zero!
        // Use == to COMPARE; a single = would ASSIGN a value instead.
        std::cout << "I am the child! getpid() = " << getpid()
                  << ", fork returned " << pid << std::endl;
    } else {
        // fork returned -1. cerr is the stream used for error messages.
        std::cerr << "Oops! fork failed." << std::endl;
    }

    // This is OUTSIDE all three branches, so both processes execute it
    // after a successful fork. If fork failed, only the original does.
    // endl adds a newline AND flushes pending output.
    std::cout << "After the fork!" << std::endl;

    // Return failure if fork failed; otherwise finish successfully.
    if (pid == -1) {
        return 1;
    }
    return 0;
}

// One possible output (the PIDs will vary):
// I am the parent! getpid() = 819, fork returned 820
// After the fork!
// I am the child! getpid() = 820, fork returned 0
// After the fork!
// The child may print first, and output operations can interleave.
// Each process prints its own identity before its own "After the fork!".
// Practice: why does "After the fork!" execute twice on success,
// even though it appears only once in the source code?
