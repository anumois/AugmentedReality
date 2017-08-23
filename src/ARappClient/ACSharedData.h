/*************************************************
 *                                               *
 * ACSharedData.h                                *
 *                                               *
 * Written by Gihong Do at 2017.8.7              *
 * Contains overall data that is used throughout *
 * every process of this client                  *
 *                                               *
 *************************************************/

#ifndef ACSHAREDDATA_H
#define ACSHAREDDATA_H

#include "ARappClient.h"

#include <stdint.h>

struct ACSD_Data
{
  uint8_t Ball_Holder[AC_NUM_OF_MARKER];
  uint32_t TeamA_Score;
  uint32_t TeamB_Score;
  int AC_Sockfd;
  int ACSD_Lock;
  int Marker_ID;
};

struct ACSD_Data ACSD_Shared;

void ACSD_Init(void);

void ACSD_Acquire_Lock(void);
void ACSD_Release_Lock(void);

#endif
