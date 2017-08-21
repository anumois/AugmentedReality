#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <stdint.h>
#include <stdlib.h>

#define NUMBER_OF_MARKERS 2
#define CHECKSUM_MAGIC 0xBEEF

//Information that is held by shared data
struct sharedItem
{
  uint16_t marker_valid[NUMBER_OF_MARKERS];
};

//Message protocol
struct MsgTemplate
{
  uint16_t Valid[NUMBER_OF_MARKERS];            //Information of which marker is valid
  uint16_t GiveScore[NUMBER_OF_MARKERS];        //Information that which marker gives score
  uint16_t checksum;
};

void sema_init(void);
void sema_down(void);
void sema_up(void);
struct MsgTemplate *processMsg(struct MsgTemplate msgbuf);

#endif
