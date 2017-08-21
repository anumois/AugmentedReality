/*************************************************
 *                                               *
 * ARappServer.c                                 *
 * Made by Gihond Do at 2017.7.31                *
 * Main function for Game server used by ARapp   *
 *                                               *
 *************************************************/

// Commonly used Headers

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

// Headers for Networking

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

// Headers for Multi-Process programming

#include <sys/wait.h>
#include <signal.h>

// Headers for Initialization

#include "ARappServer.h"
#include "ASsetup.h"
#include "ASSharedData.h"

// Headers for Implemented Functions

#include "ASSocket.h"
#include "ASMessage.h"

// Functions

static void ARAS_sigchld_handler(int s);
static void ARAS_Game(void);
static void *ARAS_get_in_addr(struct sockaddr *ARAS_sa);

// int main(void)
// Main function for Game Server 
int main(void)
{
  int ARAS_Server_Sockfd, ARAS_Client_Sockfd[4];
  int ARAS_Client_Count = 0;
  int ARAS_PID;
  /*************************/
  struct addrinfo ARAS_Hints, *ARAS_Servinfo, *ARAS_Pointer;
  int rv;
  int yes = 1;
  /*************************/
  struct sockaddr_storage ARAS_Their_Addr;
  socklen_t ARAS_Sin_Size;
  struct sigaction ARAS_Chldsa;

  char ARAS_Addr_String[INET6_ADDRSTRLEN];
  
  //Network Initialization
  /*
  if(!ASsetup_Network_Init(&ARAS_Server_Sockfd))
    {
      fprintf(stderr,"Network Initialization failed\n");
      return AS_FALSE;
    }
  */

  memset(&ARAS_Hints, 0, sizeof(ARAS_Hints));
  ARAS_Hints.ai_family = AF_UNSPEC;
  ARAS_Hints.ai_socktype = SOCK_STREAM;
  ARAS_Hints.ai_flags = AI_PASSIVE;

  if((rv = getaddrinfo(NULL, AS_PORT, &ARAS_Hints, &ARAS_Servinfo)) != 0)
    {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return -1;
    }

  for(ARAS_Pointer = ARAS_Servinfo;
      ARAS_Pointer != NULL;
      ARAS_Pointer = ARAS_Pointer -> ai_next)
    {
      if((ARAS_Server_Sockfd = socket(ARAS_Pointer -> ai_family,
				      ARAS_Pointer -> ai_socktype,
				      ARAS_Pointer -> ai_protocol)) == -1)
	{
	  fprintf(stderr, "server: socket\n");
	  continue;
	}

      if(setsockopt(ARAS_Server_Sockfd,
		    SOL_SOCKET,
		    SO_REUSEADDR,
		    &yes,
		    sizeof(int)) == -1)
	{
	  fprintf(stderr,"server: setsockopt\n");
	  return -1;
	}

      if(bind(ARAS_Server_Sockfd,
	      ARAS_Pointer -> ai_addr,
	      ARAS_Pointer -> ai_addrlen) == -1)
	{
	  close(ARAS_Server_Sockfd);
	  fprintf(stderr,"server: bind\n");
	  continue;
	}
      break;
    }

  freeaddrinfo(ARAS_Servinfo);
  if(ARAS_Pointer == NULL)
    {
      fprintf(stderr," server: failed to bind\n");
      return -1;
    }

  if(listen(ARAS_Server_Sockfd, 10) == -1)
    {
      fprintf(stderr, "server: listen\n");
      return -1;
    }
  
  //Shared Data Initialization
  ASSD_Init();
  
  ARAS_Chldsa.sa_handler = ARAS_sigchld_handler;
  sigemptyset(&ARAS_Chldsa.sa_mask);
  ARAS_Chldsa.sa_flags = SA_RESTART;
  
  if(sigaction(SIGCHLD, &ARAS_Chldsa, NULL) == -1)
    {
      fprintf(stderr, "sigaction\n");
      return AS_FALSE;
    }
  
  while(ARAS_Client_Count < 4)
    {
      printf("Waiting for Connection...\n");
      printf("ARAS_Client_Count = %d\n", ARAS_Client_Count);
      ARAS_Sin_Size = sizeof(ARAS_Their_Addr);
      if((ARAS_Client_Sockfd[ARAS_Client_Count] = accept(ARAS_Server_Sockfd,
				      (struct sockaddr *) &ARAS_Their_Addr,
				      &ARAS_Sin_Size)) == -1)
	{
	  fprintf(stderr, "accept\n");
	  continue;
	}
      
      inet_ntop(ARAS_Their_Addr.ss_family,
		ARAS_get_in_addr((struct sockaddr *)&ARAS_Their_Addr),
		ARAS_Addr_String,
		sizeof(ARAS_Addr_String));
      printf("server: got connection from %s\n", ARAS_Addr_String);

      //Child Process
      if(!(ARAS_PID = fork()))
	{
	  close(ARAS_Server_Sockfd);
	  ASCK_Server(ARAS_Client_Sockfd[ARAS_Client_Count]);
	  break;
	}

      ARAS_Client_Count++;
      
    }

  if(ARAS_PID)
    {
      ASsetup_Game_Init(ARAS_Client_Sockfd);
      ARAS_Game();
    }
  
}

static void ARAS_sigchld_handler(int s)
{
  int saved_errno = errno;
  while(waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}

static void ARAS_Game()
{
  for(;;)
    {
      ASSD_Acquire_Lock();
      ASM_Broadcast();
      ASSD_Release_Lock();

      sleep(1);
    }
}
static void *ARAS_get_in_addr(struct sockaddr *ARAS_sa)
{
  if(ARAS_sa ->sa_family == AF_INET)
    {
      return &(((struct sockaddr_in *) ARAS_sa) -> sin_addr);
    }

  return &(((struct sockaddr_in6 *) ARAS_sa) -> sin6_addr);
}

