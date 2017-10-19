#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>    

#define DEBUG 1

#define COMMAND_SIZE 30
#define MAX_LENGTH 512

/*
 * Binding the created serve socket
 * parameters:
 * sock: the server socket
 * return value:
 * 0 if success, -1 if failure
 */
int bind_created_socket(int sock)
{

    int client_port = 3212;
    struct sockaddr_in  remote;

    // Setup connection
    remote.sin_family = PF_INET;
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(client_port);

    // Binding
    return bind(sock, (struct sockaddr *)&remote, sizeof(remote));
}

/*
 * Getting uptime from the UNIX uptime command
 * parameters:
 * data: the data string containing the command output
 * return value:
 * none
 */
void get_uptime(char data[])
{
    FILE *pipe = NULL;
    char command[COMMAND_SIZE];

    // Creating the command
    sprintf(command, "uptime"); 

    // Setup the pipe for reading and execute the created command
    pipe = popen(command, "r"); 

    if(pipe == NULL){
        perror("Pipe command creation failure");
        exit(EXIT_FAILURE);
    }

    // Geting the data from the command
    fgets(data, MAX_LENGTH, pipe);

    // Closing the pipe
    if(pclose(pipe) < 0){
        perror("Pipe command closing failure");
        exit(EXIT_FAILURE);
    }
}

/*
 * Main function
 */
int main(int argc , char *argv[])
{
    int sersock;
    int consock;
    char data[MAX_LENGTH] = {0};
    struct sockaddr_in client;
    socklen_t client_len;

    // Creating socket
    if((sersock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully created the socket\n");
#endif
  
    // Binding
    if(bind_created_socket(sersock) < 0)
    {
        perror("Socket binding failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully bound the socket\n");
#endif
 
    // Listening
    if(listen(sersock, 10) < 0)
    {
        perror("Socket listening failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully listening for connections\n");
#endif

    // Infinite loop
    while(1)
    {
	    
#if DEBUG
        fprintf(stdout, "DEBUG: Listening for connections...\n");
#endif
        
        client_len = sizeof(client);
 
        // Accepting connections from an incoming client
        if((consock = accept(sersock, (struct sockaddr *)&client, &client_len)) < 0)
        {
            perror("Accepting connection failure");
            return EXIT_FAILURE;
        }

#if DEBUG
        fprintf(stdout, "DEBUG: Successfully accepted connection\n");
#endif
	  
        // Server calling uptime
        get_uptime(data);

#if DEBUG
        fprintf(stdout, "DEBUG: Successfully run uptime command\n");
#endif

        // Send data to the listening client
        if(send(consock, data, strlen(data), 0) < 0)
        {
            perror("Sending data to client failure");
            return EXIT_FAILURE;
        }

#if DEBUG
        fprintf(stdout, "DEBUG: Successfully sent data to the client\n");
#endif

        // Closing the connection
        if(close(consock) < 0)
        {
            perror("Closing connection socket failure");
            return EXIT_FAILURE;
        }

#if DEBUG
        fprintf(stdout, "DEBUG: Successfully closed the connection socket\n");
#endif

        // Sleep for 1 second
        sleep(1);
    }

    // Closing the server socket
    if(close(sersock) < 0)
    {
        perror("Closing server socket failure");
        return EXIT_FAILURE;
    }

#if DEBUG
        fprintf(stdout, "DEBUG: Successfully closed the server socket\n");
#endif

    return EXIT_SUCCESS;
}
