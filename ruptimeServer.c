#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
 
//create a Socket for server communication
short SocketCreate(void)
{
  short sock;
  
  sock = socket(PF_INET, SOCK_STREAM, 0);
  return sock;
}
 
 
int BindCreatedSocket(int sock)
{
 
  int iRetval=-1;//error val
  int ClientPort = 3212;
  struct sockaddr_in  remote;
 
  remote.sin_family = PF_INET; /* Internet address family */
  remote.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
  remote.sin_port = htons(ClientPort); /* Local port */
  iRetval = bind(sock,(struct sockaddr *)&remote,sizeof(remote));
  
  return iRetval;
}

//ruptime implementation via calling ruptime through command line
void get_uptime(char data[512])
{
    FILE *pf;
    char command[20];

    // Execute a process listing
    sprintf(command, "uptime"); 

    // Setup our pipe for reading and execute our command.
    pf = popen(command,"r"); 

    // Error handling

    // Get the data from the process execution
    fgets(data, 512 , pf);

    // the data is now in 'data'

    if (pclose(pf) != 0)
        fprintf(stderr," Error: Failed to close command stream \n");

    return;
}
    // Error handling

 
int main(int argc , char *argv[])
{
  int socket_desc; 
  int sock;
  int clientLen;
  int read_size;
  struct sockaddr_in server; 
  struct sockaddr_in client;
  char data[512] = {0};
 
	//Create socket
	socket_desc = SocketCreate();
	if (socket_desc == -1)
	{
	 printf("Could not create socket");
	 return 1;
	}
	printf("socket created\n");
 
  
	//Bind
	if( BindCreatedSocket(socket_desc) < 0)
	{
	 //print the error message
	 perror("bind failed.");
	 return 1;
	}
	printf("bind sucessful\n");
 
	//Listen
	listen(socket_desc , 10);
 
	//Accept and incoming connection
 	while(1)
	  {
	    
	    printf("Waiting for incoming connections...\n");
	    clientLen = sizeof(client);
 
	    //accept connection from an incoming client
	    sock = accept(socket_desc, (struct sockaddr *)&client, &clientLen);

	    printf("Test\n");
	    if (sock < 0)
	      {
		perror("accept failed");
		return 1;
	      }
	    printf("Connection accepted\n");
 
	  
	    //server calling ruptime
	    get_uptime(data);

	    // Send some data
	    if( send(sock, data, strlen(data) , 0) < 0)
	      {
		printf("Send failed");
		return 1;
	      }
 
	    close(sock);
	    sleep(1);
	  }
	return 0;
}
