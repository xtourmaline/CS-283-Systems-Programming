1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  execvp replaces the currently running process with a new process so when the new process exits, the shell doesn't come back because it has been replaced by the new process. this is why we use fork/execvp. fork will clone our process and then the child can be replaced with execvp.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails, then -1 is returned and errno is set to indicate the error. My implementarion checks for -1 and then prints a descriptive error message.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() finds the command to execute by checking the PATH environment variable of the system.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of calling wait() in the parent process after forking is to avoid zombie processes which happens when the parents finishes its execution before the child finishes and as a result, the child stays in the kernel.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() gives us the exit code of the child and is important because it tells us if the process ran successfully or not so the parent knows that the process is complete.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  In my implementation, the code would traverse through each letter and when I encounter a quotation mark, I would find the position of the next one and then I would copy everything in that space between the quotation marks. Then, I would continue to do this till it reaches the null terminator.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In this assignment, I had to worry about cases where I need to preserve white spaces between quote marks which I did not have to do during the last assignment. I incorporated my previous trim function so it wasn't too bad.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**: Signals in a Linux system are a form of inter process communication that facilitates communication between processes so it tells programs to do something specific. It is different from other forms of interprocess communication (IPC) because it is usually between multiple processes.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL: forcefully terminates a program, it cannot be caught or ignored (Programmers use this as a last resort which may need to happen when there is a bug or issue during cleanup process). SIGTERM: terminates the program gracefully (Programmers use this when it doesn't need to shut down immediately, it can choose handle it, ignore it, or perform cleanup tasks). SIGINT: interrupts the current program, it's usually sent by CTRL + C (Programmers do this when they want an interruption request so how it is handled is dependent on the process and situation).

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP stops a process and they cannot be caught, blocked or ignored. This is because it is handled by the kernel, which stops the execution of the process.