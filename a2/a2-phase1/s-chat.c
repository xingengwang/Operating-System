#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <list.h>


/* define data buffer size for keyboard input*/
#define BUFSIZE 512
#define DEFAULT_SYSIN 0

/* create some static thread info*/


/*mainp funtion for ubc pthread*/
int mainp()
{
  /*check you parameters!*/
  /*initializa by parsing, getting ip address, ports */
  local_port =atio(argv[1]);
  if(debug_mode)
  {
    printf(local_port);
  }
    remote_port = atio(argv[3]);
    if(debug_mode)
      print("\t Remote port set: %d\n", remote_port);
    
    
    if((host_entity = gethostbyname(argv[2])) ==NULL)
    {
      printf("unable to find remote\n");
      exit(1);
    }
    G{
//       printf("HOST FOUND");
    }
    
    address_list = (struct in_addr **)host_entity->h_addr_list;
    if(address_list[0] != NULL)
    {
      remote_ip = inet_ntoa(*address_list[0]);
      if(debug_mode)
	printf("\t[INIT] Remote Name: %s, IP: %s\n", remote_name, remote_ip);
    }
    
    /*create some threads to do the serve chat program*/
    
    return 0;
}


int client(void *arg)
{
  /*listen to send*/
  while(1)
  {
    /*maybe some thread concurrency here with some list stuff?*/
    buffer = "hello";
    if(sendto(fd, buffer, strlen(buffer),0,(struct sockaddr*)&remote_address,slen<0))
    {
      /*if send fail, re queue*/
      perroe("\t[RCVR] FAILED to SEND requeueing\n");
    }
  return 0;
}

int Listener(void *arg)
{
  struct sockaddr_in local_address, remote_address;
  socklen_t address_len -sizeof(remote_address);
  int recvlen;
  char *buffer;
  char *reply;
  int fd;
  
  /*crea a socket*/
  if((fd = socket(AF_INET, SOCKDGRAM, IPPROTO_UDP)) <0)
  {
    perroe("\t[RCVR] Uable to create socket\n");
    exit(1);
  }
  memset((char *) &local_address,0, sizeof(local_address));
  local_address.sin_family = AF_INET;
  local_address.sin_addr.sin_addr = htonl (INADDR_ANY);
  local_address.sin_port = htons(local_port);
  
  
  /*bind the socket*/
  if((bind(fd, (struct sockaddr*)&local_address  sizeof(local_address)) <0)
  {
    perroe("\t[RCVR] Failed to bind the port \n");
    exit(1);
  }
  
  while(1)
  {
    /*listen to the port for incoming msg*/
    recvlen = recvfrom(fd, buffer, BUFSIZE, 0, (struct sockaddr*)&remote_address, &address_len);
    if(recvlen >0)
    {
      /*if msg was received*/
      buffer[BUFSIZE-2] = '\n';
      buffer[BUFSIZE-1] = 0;
       /*send to server, thread stuff */
    }
    sleep(sometime);
  }
}

int Server(void *arg)
{
  char* buffer;
  char* buf_cpy;
  char* reply;
  char* sndr;
  char* rcvr;
//   call other threads to deal with incoming info
}