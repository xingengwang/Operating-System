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
#include <os.h>
#include <sys/time.h>
#include <standards.h>


/* define data buffer size for keyboard input*/
#define BUFSIZE 512
#define DEFAULT_SYSIN 0
#define debug_mode 1

/* create some static thread info*/

void keyboard_read()
{
	printf("This thread is for read keyboard input.\n");
}
void wait_UDP()
{
	printf("This thread is for wait UDP .\n");
}
void print_char()
{
	printf("This thread is for print char to screen.\n");
}

void send_UDP()
{
	printf("This thread is for send data via UDP.\n");
}



/*mainp funtion for ubc pthread*/
int mainp(int argc, char *argv[])
{
  /*check you parameters!*/
	int local_port =(int)atoi(argv[1]);
	int remote_port = (int)atoi(argv[3]);
if((local_port !=(30001 || 40000)) || (remote_port!=(30001 || 40000)))
{
	printf("\t NOT valid port.\n");
	exit(1);
}


	
  /*initializa by parsing, getting ip address, ports */
  
  if(debug_mode)
  {
    printf("\t Local port set: %d\n",local_port);
  }
    
    if(debug_mode)
    {
      printf("\t Remote port set: %d\n", remote_port);
    }
    
   
    /*create some threads to do the serve chat program*/
    	Create( (void(*)()) keyboard_read, 16000, "keyboard_read", (void *) NULL, NORM, USR );
	Create( (void(*)()) wait_UDP, 16000, "wait_UDP", (void *) NULL, NORM, USR );
	Create( (void(*)()) print_char, 16000, "print_char", (void *) NULL, NORM, USR );
	Create( (void(*)()) send_UDP, 16000, "send_UDP", (void *) NULL, NORM, USR );
    return 0;
}


