// Teaching example adapted from "03 - Processes.pdf", slide 10.
// Goal: see that BOTH processes continue immediately after fork().
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic simple_fork.cpp -o simple_fork
// Run:   ./simple_fork

#include <cstring>     // std::strcpy: copy a C-style string.
#include <iostream>    // std::cout, std::cerr, and std::endl.
#include <unistd.h>    // fork() and getpid() on macOS/Linux.

// No command-line arguments are needed, so argc and argv are omitted.
int main() {
    // These statements run once, BEFORE a child exists.
    int a = 1234;

    // Allocate room for 128 characters. b stores the array's address.
    char *b = new char[128];

    // Copy the text AND its terminating '\0' into the allocated array.
    // The destination goes first; it must have enough room for the string.
    std::strcpy(b, "hello world!");

    // Create a child. The child starts with a logical copy of our memory,
    // including a, the pointer b, and the character array b points to.
    // Here we check only for failure; the next example distinguishes roles.
    if (fork() == -1) {
        // No child was created, so only the original process runs this branch.
        std::cerr << "fork failed" << std::endl;
        delete[] b;
        return 1;
    }

    // BOTH parent and child reach this statement; neither restarts main().
    // a and b's string contents initially match in the two processes.
    // Their ordinary memory changes are independent after the fork.
    // For char*, cout prints the string; getpid() gives the caller's ACTUAL PID.
    std::cout << "a = " << a << ", b = " << b
              << ", my pid = " << getpid() << std::endl;

    // Each process releases its own array. This does not free the other's copy.
    delete[] b;

    // Each process independently finishes main() successfully.
    return 0;
}

// One possible output (the PIDs and scheduling order will vary):
// a = 1234, b = hello world!, my pid = 819
// a = 1234, b = hello world!, my pid = 820
// There is no wait() here, so neither process is guaranteed to print first.
// Output operations from the processes can also interleave.
// Practice: if you add a += 1 immediately after the fork's error check,
// what value will each process print? Explain using separate memory copies.
