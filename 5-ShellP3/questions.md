1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

> **Answer**:  We use the function waitpid() which would wait for the child before it goes on to the next pid. If we forgot to call waitpid() on all child processes then the parent would not wait for the child processes to finish running so it would start to accept inputs again.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

> **Answer**:  It is necessary to close unused pipe ends because it creates new file descriptors that is allocated for the process which is wasteful in memory so we close it.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

> **Answer**:  cd is implemented a built-in rather than an external command because with the shell, you need to know what shell you are in so cd being in an internal command will give you more control over the changing directory syscall. If it was implemented as an external process, we would not have as much control and you would not be able to make the syscall to change directory.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

> **Answer**:  I would realloc memory as needed when typing in more commands that does not fit the memory block. I would need to think about how many commands could be handled at a time because that would take a lot of memory and then limit their commands that way.
