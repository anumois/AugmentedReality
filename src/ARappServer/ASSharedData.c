/**********************************************
 *                                            *
 *                ASSharedData.c              *
 *                                            *
 * Made by Gihond Do at 2017.7.31             *
 * Keeps functions that manage shared data    *
 *                                            *
 **********************************************/

// Local Headers

#include "ASSharedData.h"

void ASSD_Init(void)
{
  int i;
  for(i = 0; i < AS_NUM_OF_MARKER; i++)
    {
      ASSD_Shared.Ball_Holder[i] = 0;
    }
  ASSD_Shared.TeamA_Score = 0;
  ASSD_Shared.TeamB_Score = 0;
  ASSD_Shared.ASSD_Lock = 1;
}

void ASSD_Acquire_Lock(void)
{
  while(ASSD_Shared.ASSD_Lock == 0);
  ASSD_Shared.ASSD_Lock--;
}

void ASSD_Release_Lock(void)
{
  ASSD_Shared.ASSD_Lock++;
}
