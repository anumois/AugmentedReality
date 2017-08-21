/************************************************
 *                                              *
 *                 ASMessage.c                  *
 *                                              *
 *        Made by Gihong Do at 2017.8.1         *
 *  Defines functions used to communicate with  *
 *  Clients                                     *
 *                                              *
 ************************************************/

#include "ASMessage.h"
#include "ASSharedData.h"

#include <stdio.h>

// Network Headers

#include <sys/types.h>
#include <sys/socket.h>

// ASM_Broadcast
// Sends to all clients information about Global data
// Call this function only inside ASSD_Acquire_Lock()
void ASM_Broadcast(void)
{
  int i;
  struct ASM_Message ASM_Broadcast_Message;

  // Form Broadcast Message
  ASM_Broadcast_Message.Message_Type = BROADCAST;
  ASM_Broadcast_Message.Magic = AS_MAGIC;
  for(i = 0; i < AS_NUM_OF_MARKER; i++)
    {
      ASM_Broadcast_Message.Ball_Holder[i] = ASSD_Shared.Ball_Holder[i];
    }
  

  // Send Broadcast Message
  for(i = 0; i < AS_NUM_OF_MARKER; i++)
    {
      if(send(ASSD_Shared.Client_Sockfd[i],
	      (uint8_t *)&ASM_Broadcast_Message,
	      sizeof(struct ASM_Message) + 1,
	      0) == -1)
	{
	  fprintf(stderr,
		  "Broadcast: Send error at fd = %d\n",
		  ASSD_Shared.Client_Sockfd[i]);
	}
    }
}

int ASM_Validate(struct ASM_Message *ASM_Valid_Message)
{
  if(ASM_Valid_Message -> Magic == AS_MAGIC)
    return AS_TRUE;
  else
    return AS_FALSE;
}
