/*                                     CS558: System Lab
*           Question 1:- Implement two C/C++ programs, namely server and client to communicate 
*                        with each other based on both TCP and UDP sockets.
*           Group Members:- Shivangi Garg(194101046) and Sweety Dhale(194101052)
*/


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define MAXLINE 1024 /*max text line length*/
#define TCP 1
#define UDP 2


// structure to store message related info like type, data and length of data
struct Message
{
    int type;
    int length;
    char data[MAXLINE];
};


// variables to store server port value received from user, file descriptor of socket created and
// UDP port value received from server
int serverPortNo,socketId,udpPortId;
//  inbuilt structure to store server related information
struct sockaddr_in serverAddress;
struct Message msg;
socklen_t serverLen;


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
void setServerAddress(const char *ip,int conn_type)
{
    switch(conn_type)
    {
        case 1:
                serverAddress.sin_family = AF_INET;
                serverAddress.sin_addr.s_addr= inet_addr(ip);
                serverAddress.sin_port =  htons(serverPortNo);
                break;
        case 2:
                serverLen = sizeof(serverAddress);
                serverAddress.sin_port = htons(udpPortId);
                break;
    }
}

// function to create connection of required type between client and server 
int create_connetion()
{
    return connect(socketId, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
}

// function to set the value of the type of the message
void set_msg_type(int t)
{
    msg.type = t;
}

// function to send connection message to the server
ssize_t sent_connection_request()
{
    return send(socketId, &msg, sizeof(msg), 0);
}

// function to get data to be sent to server
void get_input_from_client()
{
    fgets(msg.data,sizeof(msg.data),stdin);
}

// function to send message to server
void send_Packet_server()
{
    sendto(socketId, &msg, sizeof(msg), 0,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
}

// function to receive acknowledgement from server
void receive_acknowledgement()
{
    recvfrom(socketId,&msg,sizeof(msg),0,(struct sockaddr *)&serverAddress, &serverLen);
}

// driver function to implement client program
int main(int argc, char **argv)
{
    // to check whether requires arguments received or not
    if (argc !=3)
    {
        show_error("Provide proper arguments...\nIP address and Port number of the server is required",1);
    }

    printf("------------------------------------ HANDSHAKING PHASE USING TCP ------------------------------\n");
    // TCP socket creation and verification 
    serverPortNo = atoi(argv[2]);
    socketId = create_socket(TCP);
    if ( socketId < 0)
        show_error("Unable to create TCP socket\n",2);
    else
        printf("TCP socket created successfully..\n"); 

    memset(&serverAddress, 0, sizeof(serverAddress));
    // Binding newly created socket to given IP and verification
    setServerAddress(argv[1],TCP);
    int connect = create_connetion();
    if (connect < 0)
    {
        show_error("Problem in connecting to the server",3);
    }
    
    set_msg_type(1);
    msg.length = 0;
    ssize_t send = sent_connection_request();
    // Now server is ready to listen and verification
    printf("Sent request message to get UDP port of the server\n");

    if (recv(socketId, &msg, sizeof(msg),0) == 0)
    {
       show_error("The server terminated prematurely",4);
    }
    
    printf("UDP port is received as an Acknowledgement from the server of message type : %d\n",msg.type);

    printf("-------------------------------------- MESSAGE TRANSFER PHASE USING UDP ------------------------------\n");

    udpPortId = msg.length;
    
    printf("UDP Port Received from the server: %d\n",udpPortId);

    // UDP socket creation and verification
    socketId = create_socket(UDP);
    if (socketId <0)
        show_error("Problem in creating UDP socket\n",2);
    else
        printf("UDP socket created successfully..\n");
    
    setServerAddress(NULL,UDP);

    set_msg_type(3);

    printf("Data that has to be transfered : ");
    get_input_from_client();

    msg.length = strlen(msg.data);
    msg.data[msg.length-1]='\0';

    printf("Sent message to the server of type : %d\n",msg.type);
    send_Packet_server();
    receive_acknowledgement();

    msg.data[msg.length]='\0';
    
    printf("Acknowledgement Received from the server for message type: %d\n",msg.type);

    fflush(stdout);

    return 0;
}



