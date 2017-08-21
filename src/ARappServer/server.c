#include "server.h"
#include "sharedData.h"

#include <stdlib.h>

#define FALSE 0
#define TRUE 1

//Functions
int ValidateMsg(struct MsgTemplate *msgbuf);

void serverFunc(int nSockfd)
{
  int numbytes;
  struct MsgTemplate msgbuf;
  struct MsgTemplate *sendMsg;
  while(1)
    {
      if((numbytes = recv(nSockfd, (uint8_t *)&msgbuf,
			  sizeof(struct MsgTemplate), 0)) == -1)
	{
	  fprintf(stderr, "recv\n");
	  break;
	}

      if(!ValidateMsg(&msgbuf)) continue;
      printf("got something\n");
      sendMsg = processMsg(msgbuf);
      if((numbytes = send(nSockfd, (uint8_t *)sendMsg,
			  sizeof(struct MsgTemplate), 0)) == -1)
	{
	  fprintf(stderr, "send\n");
	  break;
	}
      printf("sent\n");
      free(sendMsg);
    }
}

int ValidateMsg(struct MsgTemplate *msgbuf)
{
  /*
  int i;
  uint16_t ref = 0;
  for(i = 0;
      i < sizeof(struct MsgTemplate) / sizeof(uint16_t);
      i++)
    ref = ref + *((uint16_t *)msgbuf + i);

  if(ref != CHECKSUM_MAGIC)
    {
      fprintf(stderr,"Checksum Failed! \n");
      return FALSE;
    }

  else
  */
  return TRUE;
}
