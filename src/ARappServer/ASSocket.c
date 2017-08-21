/********************************************
 *                                          *
 *             ASSocket.c                   *
 *                                          *
 *    Made by Gihong Do 2017.8.1            *
 * Functions that are used by Server-Client *
 * Socket                                   *
 *                                          *
 ********************************************/

#include "ASSocket.h"
#include "ASMessage.h"
#include "ASSharedData.h"
#include "ARappServer.h"

// Headers for Random generator

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Network Headers

#include <sys/socket.h>

void ASCK_Process_Message(struct ASM_Message *ASCK_Recv_Message);

void ASCK_Server(int AS_Sockfd)
{
  //infinite loop
  struct ASM_Message *ASCK_Recv_Message;
  ASCK_Recv_Message = (struct ASM_Message *) malloc(sizeof(struct ASM_Message));
  for(;;)
    {
      if(recv(AS_Sockfd,
	      ASCK_Recv_Message,
	      sizeof(struct ASM_Message),
	      0) == -1)
	{
	  fprintf(stderr,"ASSocket: recv error at %d\n", AS_Sockfd);
	  break;
	}

      ASCK_Process_Message(ASCK_Recv_Message);
    }
}
  
void ASCK_Process_Message(struct ASM_Message *ASCK_Recv_Message)
{
  int i;
  srand(time(NULL));
  
  ASSD_Acquire_Lock();
  switch(ASCK_Recv_Message -> Message_Type)
    {
    case STEAL:
      if(ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> To])
	{
	  ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> To] = AS_FALSE;
	  ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> From] = AS_TRUE;
	  ASM_Broadcast();
	}
      break;
    case GET:
      if(ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> To])
	{
	  ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> To] = AS_FALSE;
	  ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> From] = AS_TRUE;
	  ASM_Broadcast();
	}
      break;
    case PASS:
      if(ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> From])
	{
	  ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> From] = AS_FALSE;
	  ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> To] = AS_TRUE;
	  ASM_Broadcast();
	}
      break;
    case SHOOT:
      if(ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> From] &&
	 ASCK_Recv_Message -> To == AS_GOAL_A)
	{
	  for(i = 0; i < AS_NUM_OF_MARKER; i++)
	    {
	      ASSD_Shared.Ball_Holder[i] = AS_FALSE;
	    }
	  ASSD_Shared.Ball_Holder[(rand() % 4) + 4] = AS_TRUE;
	  ASSD_Shared.TeamB_Score++;
	  ASM_Broadcast();
	}

      else if (ASSD_Shared.Ball_Holder[ASCK_Recv_Message -> From] &&
	       ASCK_Recv_Message -> To == AS_GOAL_B)
	{
	  for(i = 0; i < AS_NUM_OF_MARKER; i++)
	    {
	      ASSD_Shared.Ball_Holder[i] = AS_FALSE;
	    }
	  ASSD_Shared.Ball_Holder[(rand() % 4) + 4] = AS_TRUE;
	  ASSD_Shared.TeamA_Score++;
	  ASM_Broadcast();
	} 
      break;
    case BROADCAST:
      break;
    default:
      break;
    }
  ASSD_Release_Lock();
}
