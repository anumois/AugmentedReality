/******************************************
 *                                        *
 * ACThread.c                             *
 *                                        *
 * Written by Gihong Do                   *
 * Holds functions used by socket thread  *
 *                                        *
 ******************************************/

// Local Header

#include "ACThread.h"
#include "ACSharedData.h"

// Commonly Used Header

#include <stdio.h>

// Network Header

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

void *ACT_Socket_Thread(void *AC_Sockfd)
{
  int Sockfd = *(int *)AC_Sockfd;
  struct ACM_Message Recv_Message;

  printf("Client Initialized\n");

  while(1)
    {
      if(recv(Sockfd,
	      (uint8_t *)&Recv_Message,
	      sizeof(struct ACM_Message),
	      0) == -1)
	{
	  fprintf(stderr, "recv\n");
	  break;
	}

      if(!ACM_Validate(&Recv_Message)) continue;

      ACT_Process_Message(&Recv_Message);
    }
}

void ACT_Process_Message(struct ACM_Message *Recv_Message)
{
  int i;
  ACSD_Acquire_Lock();
  for(i = 0; i < AC_NUM_OF_MARKER; i++)
    {
      ACSD_Shared.Ball_Holder[i] = Recv_Message -> Ball_Holder[i];
      ACSD_Shared.TeamA_Score = Recv_Message -> TeamA_Score;
      ACSD_Shared.TeamB_Score = Recv_Message -> TeamB_Score;
    }
  ACSD_Release_Lock();
}
