/**************************************************
 *                                                *
 * ACSharedData.c                                 *
 * Written by Gihong Do at 2017.8.7               *
 * Keeps functions that manage shared data        *
 *                                                *
 **************************************************/

// Local Headers

#include "ACSharedData.h"

void ACSD_Init()
{
  int i;
  for(i = 0; i < AC_NUM_OF_MARKER; i++)
    {
      ACSD_Shared.Ball_Holder[i] = 0;
    }
  ACSD_Shared.TeamA_Score = 0;
  ACSD_Shared.TeamB_Score = 0;
  ACSD_Shared.ACSD_Lock = 1;
}

void ACSD_Acquire_Lock(void)
{
  while(!ACSD_Shared.ACSD_Lock);
  ACSD_Shared.ACSD_Lock--;
}

void ACSD_Release_Lock(void)
{
  ACSD_Shared.ACSD_Lock++;
}
