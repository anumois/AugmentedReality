/*************************************************
 *                                               *
 *                ASSharedData.h                 *
 *                                               *
 * Made by Gihong Do at 2017.7.31                *
 * Contains Overall Data That should be cared by *
 * Every process in the server                   *
 *                                               *
 *************************************************/
#ifndef ASSHAREDDATA_H
#define ASSHAREDDATA_H

#include "ARappServer.h"

#include <stdint.h>

struct ASSD_Data
{
  uint8_t  Ball_Holder[AS_NUM_OF_MARKER];
  uint32_t TeamA_Score;
  uint32_t TeamB_Score;
  int Client_Sockfd[AS_NUM_OF_MARKER];
  int ASSD_Lock;
};

struct ASSD_Data ASSD_Shared;

void ASSD_Init(void);

void ASSD_Acquire_Lock(void);
void ASSD_Release_Lock(void);
#endif
