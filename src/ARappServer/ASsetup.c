/******************************************
 *                                        *
 *               ASsetup.c                *
 *                                        *
 *      Made by Gihond Do 2017.7.31       *
 *  Usage:Initialization for ARappServer  *
 *                                        *
 ******************************************/ 
// Local Headers

#include "ASsetup.h"
#include "ARappServer.h"
#include "ASSharedData.h"
#include "ASMessage.h"

// Commonly used headers

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
//Network related headers

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

void ASsetup_Game_Init(int *Client_Sockfd_Array)
{
  int i;
  srand(time(NULL));
  ASSD_Acquire_Lock();
  for(i = 0; i < AS_NUM_OF_PLAYER; i++)
    {
      ASSD_Shared.Client_Sockfd[i] = Client_Sockfd_Array[i];
    }
  ASSD_Shared.Ball_Holder[AS_NUM_OF_PLAYER + (rand() % 4)] = AS_TRUE;
  ASM_Broadcast();
  ASSD_Release_Lock();
}

// ASsetup_Network_Init
// Setup Network server for ARappserver
// Returns Server socket file descriptor(Parameter)
// and whether initialization is successful (AS_TRUE/FALSE)

int ASsetup_Network_Init(int *Server_Sockfd)
{
  struct addrinfo ASNI_Hints;
  struct addrinfo *ASNI_Servinfo, *ASNI_p;
  int ASNI_rv;
  int ASNI_yes = 1;
  
  memset(&ASNI_Hints, 0, sizeof(ASNI_Hints));
  ASNI_Hints.ai_family = AF_UNSPEC;
  ASNI_Hints.ai_socktype = SOCK_STREAM;
  
  if((ASNI_rv = getaddrinfo(NULL, AS_PORT, &ASNI_Hints, &ASNI_Servinfo)) != 0)
    {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ASNI_rv));
      return AS_FALSE;
    }

  for(ASNI_p = ASNI_Servinfo;
      ASNI_p != NULL;
      ASNI_p = ASNI_p -> ai_next)
    {
      if((*Server_Sockfd = socket(ASNI_p -> ai_family,
				  ASNI_p -> ai_socktype,
				  ASNI_p -> ai_protocol)) == -1)
	{
	  fprintf(stderr, "ASsetup: socket\n");
	  continue;
	}

      if(setsockopt(*Server_Sockfd,
		    SOL_SOCKET,
		    SO_REUSEADDR,
		    &ASNI_yes,
		    sizeof(int)) == -1)
	{
	  fprintf(stderr, "setsockopt\n");
	  return AS_FALSE;
	}

      if(bind(*Server_Sockfd,
	      ASNI_p -> ai_addr,
	      ASNI_p -> ai_addrlen) == -1)
	{
	  close(*Server_Sockfd);
	  fprintf(stderr,"ASsetup: bind\n");
	  continue;
	}

      break;
    }

  freeaddrinfo(ASNI_Servinfo);

  if(ASNI_p == NULL)
    {
      fprintf(stderr, "ASsetup: failed to bind\n");
      return AS_FALSE;
    }

  if(listen(*Server_Sockfd, AS_BACKLOG) == -1)
    {
      fprintf(stderr, "listen\n");
      return AS_FALSE;
    }
  printf("Server Established\n");
  return AS_TRUE;
}
