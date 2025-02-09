1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for this application because it reads line by line. Then, when we press enter, it reads the \n character and stops further reading. This fits very well with how a person would type in the terminal, making it a good choice.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  We meed to use `malloc()` to allocate memory for `cmd_buff` in `dsh_cli.c` because we might need more space to carry out our commands. For instance, sometimes commands would take up multiple lines and we might need to increase the buffer size as a result. We can do so dynamically through `realloc()` while the program is running, in comparison to a fixed size array, where the size could only be changed via a full recompile.


3. In `dshlib.c`, the function `build_cmd_list()` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  We must trim leading and trailing spaces from each command before storing it before storing it so we can get just the exe and then the arguments. If we didn't trim it, a command like `  cmd1  arg1 arg2   ` would give you `  cmd1`, but if you tried to actually execute it, the code will probably be looking for something like `cmd1` without the whitespaces. This could lead to issues of the shell not corretly recognizing the command.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Three examples of redirection is redirecting stdout to a file, redirecting file data into stdin file, and redirecting stderr to stdout. Some challenges implementing them will probably be handling edge cases where maybe there is an invalid file where it cannot read from or write to.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Piping is allows you to redirect output streams from one process to another as an input stream while redirection is when you redirect streams to/from files or other streams.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep STDERR and STDOUT seperate so that you can redirect it to different places like files if you choose to. It would be less annoying for programmers because errors and actual output would not be mixed together.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our custom shell should return an error code and then print a message on why it failed in the STDERR. A case where both outputs might want to be merged is when there is an error and some output from STDOUT can help us debug the code. We should provide a way to merge them by implementing redirection operations from multiple streams on to one stream.