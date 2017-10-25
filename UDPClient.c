#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define DEBUG 1

#define MAX_ADDRESS 100
#define BUFFER_SIZE 8192

int main(int argc, char *argv[])
{
    int sock;
    char server_IP[MAX_ADDRESS];
    char dest_IP[MAX_ADDRESS];
    int server_port;
    int dest_port;
    int loss_rate;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];
    int recv_length;
    srand(time(NULL));//initializing rand for loss rate check
    // Getting program parameters and error checking
    if(argc == 6)
    {
        strcpy(server_IP, argv[1]);
        server_port = atoi(argv[2]);
        strcpy(dest_IP, argv[3]);
        dest_port = atoi(argv[4]);
        loss_rate = atoi(argv[5]);

        if(loss_rate < 0 || loss_rate > 100)
        {
            fprintf(stderr, "loss_rate must be between 0 and 100\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        fprintf(stderr, "./udp_relay <source_IP> <source_port> <dest_IP> <dest_port> <loss_rate>\n");
        return EXIT_FAILURE;
    }
    
    // Creating the socket
    if((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully created the socket\n");
#endif

    // Setting address information
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_IP);
    
    if(bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Socket binding failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Sucessfully bound the socket\n");
#endif

    // Setting client's socket information
    client_addr.sin_family = PF_INET;
    client_len = sizeof(client_addr);

#if DEBUG
    fprintf(stdout, "DEBUG: Ready to relay UDP packets...\n");
#endif
    
    while(1)
    {
        // Receiving a UDP packet
        if((recv_length = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_addr, &client_len)) < 0)
        {
            perror("Packet receiving failure");
            return EXIT_FAILURE;
        }

        // Changing the client address and port for relaying
        client_addr.sin_addr.s_addr = inet_addr(dest_IP);
        client_addr.sin_port = htons(dest_port);

        // end the datagram to the other machine
	int loss =(rand() % 100) + 1; //generates a random val to check loss rate against
	if(loss >= loss_rate)//if loss_rate met the packet isn't sent
	  {
	    if(sendto(sock, buffer, recv_length, 0, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
	      {
		perror("Sending packet failure");
		return EXIT_FAILURE;
	      }
	  }
    }

    // Closing the server socket
    if(close(sock) < 0)
    {
        perror("Closing socket failure");
        return EXIT_FAILURE;
    }

#if DEBUG
    fprintf(stdout, "DEBUG: Successfully closed the socket\n");
#endif

    return EXIT_SUCCESS;
}


