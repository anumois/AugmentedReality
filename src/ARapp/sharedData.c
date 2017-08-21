#include <stdio.h>
#include "sharedData.h"

#include <stdio.h>
static struct sharedItem Resource;
static int semaphore;

void sema_init()
{
  int i;
  semaphore = 1;

  for(i = 0; i < NUMBER_OF_MARKERS; i++)
    {
      Resource.marker_valid[i] = 1;
    }
}
void DEBUGprintItem()
{
  int i;
  for(i = 0; i < NUMBER_OF_MARKERS; i++)
    {
      printf("valid%d : %d\n",i , Resource.marker_valid[i]);
    }
  printf("score: %d\n", Resource.score);
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

void processMsg(struct MsgTemplate msgbuf)
{
  int i;
  sema_down();
  for(i = 0; i < NUMBER_OF_MARKERS; i++)
    {
      Resource.marker_valid[i] = msgbuf.Valid[i];
      if(msgbuf.GiveScore[i] == 1) Resource.score++;
    }
  sema_up();
}

//must be called inside sema down
int isMarkerValid(int i)
{
  int ret_val = 0;
  
  if(Resource.marker_valid[i] == 1)
    ret_val = 1;
  
  return ret_val;
}
