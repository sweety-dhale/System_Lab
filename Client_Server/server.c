/*                                     CS558: System Lab
*           Question 1:- Implement two C/C++ programs, namely server and client to communicate 
*                        with each other based on both TCP and UDP sockets.
*           Group Members:- Shivangi Garg(194101046) and Sweety Dhale(194101052)
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 1024 /*max text line length*/
#define LISTENQ 8 /*maximum number of client connections*/
#define TCP 1
#define UDP 2


// structure to store message related info like type, data and length of data
struct message
{
    int type;
    int length;
    char data[MAXLINE];
};


int listenfd, connfd,serv_port;
struct message msg;
pid_t childpid;
socklen_t clilen;
char buf[MAXLINE];
struct sockaddr_in cliaddr, servaddr;


// function to show what error has occured
void show_error(char *error_msg, int exit_num)
{
    perror(error_msg);
    exit(exit_num);
}


// function to create socket of required type using IPv4 protocol
int create_socket(int conn_type)
{
    switch(conn_type)
    {
        case 1:
                return socket (AF_INET, SOCK_STREAM, 0);
        case 2:
                return socket (AF_INET, SOCK_DGRAM, 0);
    }
}

// function to set server related information like binding IP and Port
void setServerAddress(int conn_type)
{
     switch(conn_type)
    {
        case 1:
                servaddr.sin_family = AF_INET;
                servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
                servaddr.sin_port = htons(serv_port);
                break;
        case 2:
                servaddr.sin_port = htons(0);
                break; 
    }
}


// Binding newly created socket to given IP and verification 
int bind_conn()
{
	return bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
}


// function to set the value of the type of the message
void set_Packet(int t)
{
    msg.type = t;
}


// function to transfer data to clients concurrently using UDP socket
int data_transfer()
{
    // Accept the data packet from client and verification
    int client_num = 0;
    printf("Server running...\nWaiting for connections...\n");
    while (1)
    {
        clilen = sizeof(cliaddr);
        connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("Received request...\n");
    
        client_num++;
        if ( (childpid = fork ()) == 0 )  //Child process
        {
            printf("%d\tcreated for dealing with client requests\n", client_num);
            close (listenfd);

            // read the message from client and copy it in buffer
            if( recv(connfd, &msg, MAXLINE,0) == 0)
                show_error("The client terminated prematurely\n",5);
            else
                printf("Client terminated successfully...\n"); 
            
            printf("Message type received from the client: %d\n",msg.type);
            listenfd = socket (AF_INET, SOCK_DGRAM, 0);
            if (listenfd <0)
                show_error("Unable to create UDP socket\n",2);
            else
                printf("UDP socket created successfully...\n"); 

            int bind = bind_conn();
            if (bind<0)
                show_error("Problem in binding to UDP socket\n",3);
            else
                printf("UDP socket binded successfully...\n"); 

            struct sockaddr_in localAddress;
            socklen_t addressLength = sizeof localAddress;
            getsockname(listenfd, (struct sockaddr*)&localAddress,&addressLength);

            printf("Server UDP port: %d\n", (int) ntohs(localAddress.sin_port));
            set_Packet(2);
            
            msg.length = ntohs(localAddress.sin_port);
            send(connfd, &msg, sizeof(msg), 0);
       
            printf("Sent message of type %d\n",msg.type);
               
            recvfrom(listenfd,&msg,sizeof(msg),0,(struct sockaddr *)&cliaddr, &clilen);
            printf("Receiving from (%s , %d) : ",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

            msg.data[msg.length]='\0';
            printf("Message type received from the client: %d\n",msg.type);
            // print buffer which contains the client contents
            printf("\t\tMessage: %s\n",msg.data);
            // copy server message in the buffer
            set_Packet(4);

            sendto(listenfd, &msg, sizeof(msg), 0,(struct sockaddr *)&cliaddr, clilen);
            printf("Sent message of type %d\n",msg.type);
            
            fflush(stdout);
            
            return 0;
        }
        close(connfd);
    }
}

// driver function to implemet server program
int main (int argc, char **argv)
{
    // to check whether required arguments received or not
	if(argc!=2)
        show_error("Provide proper arguments...\nPort number of the server is required",1);

    // socket creation and verification 
    serv_port = atoi(argv[1]);
	listenfd = create_socket(TCP);
    if (listenfd <0)
        show_error("Unable to create TCP socket\n",2);
    else
        printf("TCP socket created successfully...\n"); 

    // assign IP, PORT 
	setServerAddress(TCP);
    // Binding newly created socket to given IP and verification 
	int bind = bind_conn();
    if (bind<0)
        show_error("Problem in binding to TCP socket\n",3);
    else
        printf("Socket successfully binded...\n");

    // Now server is ready to listen and verification
    if (listen (listenfd, LISTENQ)<0)
        show_error("Failed to listen on TCP socket\n",4);
    else
        printf("Server listening..\n"); 

    // function to transfer data
    data_transfer();

    close(listenfd);
    return 0;
}

