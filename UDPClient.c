#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
  
#define BUFLEN 512
#define NPACK 10


void diep(char *s)
{
  perror(s);
  exit(1);
}
 
int main(int argc, char *argv[])
{
  char source_IP[200];//source is the program ip
  char dest_IP[200];//streaming to
  int source_port, dest_port, loss_rate;
  struct sockaddr_in si_source, si_dest;
  int sourceSock, destSock, sourceLen = sizeof(si_source), recv_len, destLen = sizeof(si_dest);
  char buf[BUFLEN];

  // Arguments test
  if(argc != 6)
    {
      fprintf(stderr, "5 arguments needed: localhost_IP and server_IP\n");
      return EXIT_FAILURE;
    }
  else
    {
      strcpy(source_IP, argv[1]);
      source_port = atoi(argv[2]);
      strcpy(dest_IP, argv[3]);
      dest_port = atoi(argv[4]);
      loss_rate = atoi(argv[5]);
    }

  //create socket
  if ((sourceSock=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  //binding socket
  memset((char *) &si_source, 0, sizeof(si_source));

  si_source.sin_family = PF_INET;
  si_source.sin_addr.s_addr = inet_addr(source_IP);
  si_source.sin_port = htons(source_port);
  
  bind(sourceSock, (struct sockaddr *)&si_source, sizeof(si_source));
 
  memset((char *) &si_dest, 0, sizeof(si_dest));

  si_dest.sin_family = PF_INET;
  si_dest.sin_addr.s_addr = inet_addr(dest_IP);
  si_dest.sin_port = htons(dest_port);
  
  while(1)
    {
      if ((recv_len = recvfrom(sourceSock, buf, BUFLEN, 0, (struct sockaddr *) &si_source, &sourceLen)) == -1)   // read datagram from server socket
	{
	  diep("recvfrom()");
	}

      if (sendto(destSock, buf, recv_len, 0, (struct sockaddr*) &si_dest, &destLen) == -1)
	{
	  diep("sendto()");
	}
    }
}
