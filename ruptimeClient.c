#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEBUG 0

#define MAX_LENGTH 512
#define MAX_ADDRESS 100

/*
 * Connection to the server
 * parameters:
 * sock: the client socket
 * server_IP: the server IP address
 * return value:
 * 0 if success, -1 if failure
 */
int socket_connect(int sock, char server_IP[])
{
    int ServerPort = 3212; // University ID as indicated in lab description
    struct sockaddr_in remote;

    // Setup remote connection
    remote.sin_addr.s_addr = inet_addr(server_IP); 
    remote.sin_family = PF_INET;
    remote.sin_port = htons(ServerPort);

    // Connection
    return connect(sock, (struct sockaddr *)&remote, sizeof(remote));
}

/*
 * Main function
 */
int main(int argc, char *argv[])
{
    int sock;
    char server_reply[MAX_LENGTH] = {0};
    char local_host_IP[MAX_ADDRESS];
    char server_IP[MAX_ADDRESS];

    // Arguments test
    if(argc != 3)
    {
        fprintf(stderr, "2 arguments needed: localhost_IP and server_IP\n");
        return EXIT_FAILURE;
    }
    else
    {
        strcpy(local_host_IP, argv[1]);
        strcpy(server_IP, argv[2]);
    }
 
    // Creating the socket
    if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully created the socket\n");
#endif
 
    // Connection to remote server
    if(socket_connect(sock, server_IP) < 0)
    {
        perror("Remote connection failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully conected to the server\n");
#endif
    
    // Receiving the data from the server
    if(recv(sock, server_reply, MAX_LENGTH , 0) < 0)
    {
        perror("Receiving data failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully received data from remote server\n");
#endif

    // Printing data
    fprintf(stdout, "%s:%s", server_IP, server_reply);

    // Shutdown the connection
    if(shutdown(sock, SHUT_RDWR) < 0)
    {
        perror("Shutdown connection failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully shutdown the connection\nExiting successfully\n");
#endif

    // Closing the socket
    if(close(sock) < 0)
    {
        perror("Closing socket failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully closed the socket\n");
#endif    
 // Creating the local socket
    if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully created the socket\n");
#endif
 
    // Connection to remote server
    if(socket_connect(sock,local_host_IP) < 0)
    {
        perror("Remote connection failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully conected to the server\n");
#endif
    
    // Receiving the data from the server
    if(recv(sock, server_reply, MAX_LENGTH , 0) < 0)
    {
        perror("Receiving data failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully received data from remote server\n");
#endif

    // Printing data
    fprintf(stdout, "%s:%s", local_host__IP, server_reply);

    // Shutdown the connection
    if(shutdown(sock, SHUT_RDWR) < 0)
    {
        perror("Shutdown connection failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully shutdown the connection\nExiting successfully\n");
#endif

    // Closing the socket
    if(close(sock) < 0)
    {
        perror("Closing socket failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully closed the socket\n");
#endif

    return EXIT_SUCCESS;
}
