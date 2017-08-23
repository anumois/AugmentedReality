/****************************************************
 *                                                  *
 *                    ASMessage.h                   *
 *                                                  *
 * Made by Gihong Do at 2017.8.1                    *
 *  Defines the template of message that is sent to *
 * Client                                           *
 *                                                  *
 ****************************************************/

#ifndef ASMESSAGE_H
#define ASMESSAGE_H

#include "ARappServer.h"

#include <stdint.h>

enum ASM_Message_Type
  {
    STEAL,
    GET,
    PASS,
    SHOOT,
    BROADCAST
  };

struct ASM_Message
{
  uint8_t Ball_Holder[AS_NUM_OF_MARKER];
  enum ASM_Message_Type Message_Type;
  uint32_t Magic;
  uint32_t TeamA_Score;
  uint32_t TeamB_Score;
  uint8_t From;
  uint8_t To;
};

void ASM_Broadcast(void); 
int ASM_Validate(struct ASM_Message *ASM_Valid_Message);

#endif
  
