// Teaching example adapted from "03 - Processes.pdf", slide 14.
// Goal: replace this program with /bin/echo and pass it a message to print.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic execv_example.cpp -o execv_example
// Run:   ./execv_example

#include <iostream> // std::cout, std::cerr, and std::endl.
#include <unistd.h> // execv() on macOS/Linux.

int main() {
    // This character array contains the PATH of the executable to run.
    // We use a writable array, rather than assigning a string literal to char*.
    char command[] = "/bin/echo";

    // This is an ARGUMENT to that program. It does not replace command.
    // echo's job is to print its arguments, which is why this text appears.
    char message[] = "ECS 150";

    // args is an ARRAY OF POINTERS to character strings:
    //   args[0] -> "/bin/echo" : program name, by convention.
    //   args[1] -> "ECS 150"   : the first argument given to the program.
    //   args[2] = nullptr     : marks the end of the argument array.
    // Each string ends with '\0'; nullptr ends the array of string pointers.
    char *args[] = {command, message, nullptr};

    // This is still our C++ program. endl adds a newline and flushes output
    // before execv replaces the program and its memory.
    std::cout << "Before execv" << std::endl;

    // Read the call as: run the executable at command, with arguments args.
    // It is equivalent to running this terminal command:
    //   /bin/echo "ECS 150"
    //
    // On SUCCESS:
    //   - The current process keeps its PID; no child process is created.
    //   - This program's code, stack, and heap are replaced by the new program.
    //   - echo receives two arguments: its name and the message.
    //   - echo prints "ECS 150", then finishes.
    //   - Execution NEVER returns here, even after echo finishes.
    //
    // On FAILURE (for example, a nonexistent executable path):
    //   - execv returns -1 and our original program continues below.
    int result = execv(command, args);

    // This line runs ONLY if execv failed. A successful call replaced this code.
    std::cerr << "execv failed! Return value = " << result << std::endl;

    // Report failure to whoever launched this program.
    // On success, echo supplies its own exit status instead.
    return 1;
}

// Expected successful output:
// Before execv
// ECS 150
//
// Trace: our program prints -> execv replaces it -> echo prints -> echo exits.
// It does NOT replace the string "/bin/echo" with the string "ECS 150".
// It replaces the RUNNING PROGRAM and gives the new program that message.
//
// Practice:
// 1. Change only message to "Hello". Which program runs, and what prints?
// 2. Change command to "/no/such/program". Which lines execute now?
// 3. Why would a shell call fork() before letting the child call execv()?
