#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
 
 
//Create a Socket for server communication
short SocketCreate(void)
{
 
        short sock;
        printf("Create the socket\n");
        sock = socket(PF_INET, SOCK_STREAM, 0)
        return sock;
}
 
//try to connect with server
int SocketConnect(int sock, char server_IP[])
{
 
        int iRetval=-1;
        int ServerPort = 45716;//id_num as indicated in lab description
        struct sockaddr_in remote;
 
        remote.sin_addr.s_addr = inet_addr(localHost_IP); 
        remote.sin_family = PF_INET;
        remote.sin_port = htons(ServerPort);
 
        iRetval = connect(sock , (struct sockaddr *)&remote , sizeof(remote));
 
 
        return iRetval;
}
 
 
//receive the data from the server 
int SocketReceive(int hSocket,char* Rsp,short RvcSize)
{
 
	int shortRetval = -1;
 
	shortRetval = recv(hSocket, Rsp , RvcSize , 0);
 
	return shortRetval;
 }
 
 
 //main driver program
int main(int argc , char *argv[])
{
  int sock;
  int read_size;
  char server_reply[512] = {0};
  char localHost_IP[200];
  char server_IP[200];

  strcpy(localHost_IP, argv[1]);
  strcpy(server_IP, argv[2]);
 
        //Create socket
	sock = SocketCreate();
	if(sock == -1)
	{
	  printf("Could not create socket\n");
	  return 1;
	}
 
	printf("Socket is created\n");
 
	//Connect to remote server
	if (SocketConnect(sock, server_IP) < 0)
	{
		perror("connect failed");
		return 1;
	}
 
	printf("Sucessfully conected with server\n");
	
 
	//Received the data from the server
	read_size = SocketReceive(sock , server_reply , 200);
        if(read_size);
	
	printf("%s: %s\n", server_IP, server_reply);
 
 	close(sock);
	shutdown(sock,0);
	shutdown(sock,1);
	shutdown(sock,2);

    return 0;
}
