1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

> **Answer**:  The remote client determines when a command's output is fully recieved from the server when the socket recieves all of its data. It does not allow for data to be read and the program does not continue until all of the data is in the socket and how the end is signified is to add some kind of character at the end, like using EOF.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

> **Answer**:  A network shell protocol by using a delimeter character in order to detect if it is the beginning or the end of a command that is sent through TCP connection. If it is not handles correctly, then you may take too little or too much data which would give you the incorect command.

3. Describe the general differences between stateful and stateless protocols.

> **Answer**:  Stateful protocols are network protocols that when a client sends a request to the server, it expects some kind of response back, but if it does not get a response, it resends requests. Meanwhile, stateless protocols are network protocols that the Client sends a request to the server and the server responds back according to the current state. Additionally, stateless protocols do not keep any kind of “stateful” information regarding clients, so each interaction between a client and server is independent.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

> **Answer**:  We would use it when we would want information quickly over reliability because UDP does not check if anyone is listening and would just give information without needing a response back. This is good for things like calling and online gaming where waiting for a response may take too long and losing some data will be fine.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

> **Answer**:  Sockets are provided by the operating system to enable applications to use network communicstions.