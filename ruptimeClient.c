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
        sock = socket(AF_INET, SOCK_STREAM, 0);
        return sock;
}
 
//try to connect with server
int SocketConnect(int sock, char localHost_IP[])
{
 
        int iRetval=-1;
        int ServerPort = 45716;//id_num as indicated in lab description
        struct sockaddr_in remote={0};
 
        remote.sin_addr.s_addr = inet_addr(localHost_IP); 
        remote.sin_family = AF_INET;
        remote.sin_port = htons(ServerPort);
 
        iRetval = connect(sock , (struct sockaddr *)&remote , sizeof(struct sockaddr_in));
 
 
        return iRetval;
}
 
 
//receive the data from the server 
int SocketReceive(int hSocket,char* Rsp,short RvcSize)
{
 
	int shortRetval = -1;
 
	shortRetval = recv(hSocket, Rsp , RvcSize , 0);
 
	printf("Response %s\n",Rsp);
 
	return shortRetval;
 }
 
 
 //main driver program
int main(int argc , char *argv[])
{
  int sock;
  int read_size;
  struct sockaddr_in server;
  char server_reply[512] = {0};
  char localHost_IP[200] = argv[1];//127.0.0.1
  char server_IP[200] = argv[2];
 
        //Create socket
	sock = SocketCreate();
	if(sock == -1)
	{
	  printf("Could not create socket\n");
	  return 1;
	}
 
	printf("Socket is created\n");
 
	//Connect to remote server
	if (SocketConnect(sock) < 0)
	{
		perror("connect failed.\n");
		return 1;
	}
 
	printf("Sucessfully conected with server\n");
	
 
	//Received the data from the server
	read_size = SocketReceive(sock , server_reply , 200);
	
	printf("%s: %s\n", server_IP, server_reply);
 
 	close(sock);
	shutdown(sock,0);
	shutdown(sock,1);
	shutdown(sock,2);

    return 0;
}
