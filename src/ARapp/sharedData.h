#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <stdint.h>

#define NUMBER_OF_MARKERS 2

struct sharedItem
{
  uint16_t marker_valid[NUMBER_OF_MARKERS];
  int score;
  int gain;
};

struct MsgTemplate
{
  uint16_t Valid[NUMBER_OF_MARKERS];
  uint16_t GiveScore[NUMBER_OF_MARKERS];
  uint16_t checksum;
};

void DEBUGprintItem(void);
void sema_init(void);
void sema_down(void);
void sema_up(void);
void processMsg(struct MsgTemplate msgbuf);
int isMarkerValid(int i);
#endif
