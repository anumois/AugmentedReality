#include <stdio.h>
#include "sharedData.h"

static struct sharedItem Resource;
static int semaphore;

//Semaphore of shared resource
//It is practically used as a unary lock

void sema_init()
{
  int i;
  semaphore = 1;

  for(i = 0; i < NUMBER_OF_MARKERS; i++)
    {
      Resource.marker_valid[i] = 1;
    }
}

void sema_down()
{
  while(semaphore < 1);

  semaphore--;
}

void sema_up()
{
  semaphore++;
}

// processMsg: Receives MsgTemplate,
// modifies the shared resources according to the received data
// returns response MsgTemplate that should be sent to the client

struct MsgTemplate *processMsg(struct MsgTemplate msgbuf)
{
  int i;
  uint16_t ref = 0;
  uint16_t checksum = 0;
  struct MsgTemplate *retMsg = (struct MsgTemplate *) malloc(sizeof(struct MsgTemplate));

  printf("processing...\n");
  sema_down();
  printf("sema_down\n");
  retMsg->checksum = 0;
  for(i = 0; i < NUMBER_OF_MARKERS; i++)
    {
      retMsg->Valid[i] = 0;
      retMsg->GiveScore[i] = 0;
      if(msgbuf.GiveScore[i] == 1 &&
	 Resource.marker_valid[i] == 1)
	{
	  retMsg->Valid[i] = 0;
	  retMsg->GiveScore[i] = 1;
	  Resource.marker_valid[i] = 0; 
	}
      else if(Resource.marker_valid[i] == 0)
	{
	  retMsg->Valid[i] = 0;
	}
      else
	{
	  retMsg->Valid[i] = 1;
	}
    }

  for(i = 0;
      i < sizeof(struct MsgTemplate) / sizeof(uint16_t);
      i++)
    {
      ref += *(((uint16_t *) retMsg) + i);
    }
  checksum = (uint16_t) CHECKSUM_MAGIC - ref;
  retMsg->checksum = checksum;
  sema_up();

  return retMsg;
}
