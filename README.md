# System_Lab

Assignment No. 01
1. Personal Dictionary : Basic concepts on shell scripting
2. Implementation of Shell Commands : Concepts on basic shell commands
3. ‘Snakes & Ladders’ Game : Process creation and Inter-process communication (Using PIPES)


Assignment No. 02
1. Adding a New System Call to the Kernel
2. Synchronisation
      - Car Manufacturing System
      - Banking System
      - Automated Voting System
 
 
Assignment No. 03
1. CPU Management : Implementation of CPU Scheduling Algorithms, i.e. SJF(preemptive and non-preemptive)
2. Memory Management : Implementation of Page Replacement Algorithms, i.e. LRU and Optimal
3. Implementation of Disk Scheduling Algorithms for Elevator Control System, i.e. SCAN


Assignment No. 04
The aim is to implement a simple 2 stage communication protocol. You require to implement two C/C++ programs, 
namely server and client to communicate with each other based on both TCP and UDP sockets as follows :
Phase I :
      1. Initially, server will be waiting for a TCP connection from the client. 
      2. Client will connect to the server using server’s TCP port already known to the client. 
      3. After successful connection, the client sends a Request Message (Type 1 message) to the server via TCP port 
         to request a UDP port from server for future communication. 
      4. After receiving the Request Message, server obtains a UDP port number and sends this port number back to the 
         client as a Response Message (Type 2 Message) over the TCP connection. 
      5. After this negotiation phase, the TCP connection on both the server and client should be closed gracefully 
         releasing the socket resource. 
      
Phase II :
      1. The client transmits a short Data Message (Type 3 message) over the negotiated UDP port. The server will 
         display the received Data Message and sends a Data Response (type 4 message) to indicate the 
         successful reception. 
      2. After this data transfer phase, both sides close their UDP sockets. 
     
Note:-
      1. The messages format: Message_type (integer), Message_length (integer), Message (characters of a fixed length)
      2. <Data Message> in Client will be the content of the message in Type 3 as explained above. 
      3. You also require implementing a "Concurrent Server" (a server that accepts connections from multiple clients and 
         serves all of them concurrently). 
      4. You should accept the IP Address and Port number from the command line (Don't use a hard-coded port number). 
      5. Prototype for command line is as follows: 
         Client :<executable code><Server IP Address><Server Port number> 
         Server :<executable code><Server Port number> 

*Please make necessary and valid assumptions whenever required.

