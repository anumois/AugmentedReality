/**************************************************
 *                                                *
 * ACMessage.h                                    *
 *                                                *
 * Written by Gihong Do at 2017.8.7               *
 * Defines the template of message sent to Server *
 *                                                *
 **************************************************/

#ifndef ACMESSAGE_H
#define ACMESSAGE_H

#include "ARappClient.h"

#include <stdint.h>

enum ACM_Message_Type
  {
    STEAL,
    GET,
    PASS,
    SHOOT,
    BROADCAST
  };

struct ACM_Message
{
  uint8_t Ball_Holder[AC_NUM_OF_MARKER];
  enum ACM_Message_Type Message_Type;
  uint32_t Magic;
  uint32_t TeamA_Score; //Not used when sending message
  uint32_t TeamB_Score;
  uint8_t From;
  uint8_t To;
};

int ACM_Validate(struct ACM_Message *ACM_Valid_Message);

int ACM_Pub_Message(enum ACM_Message_Type ACM_Type,
		     int From,
		     int To,
		     int Sockfd);

int ACM_Shoot(int Goal, int Sockfd);
int ACM_Tackle(int Player_num, int Sockfd);
int ACM_Get(int Gen_num, int Sockfd);
#endif
