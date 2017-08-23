/********************************************
 *                                          *
 * ACMessage.h                              *
 *                                          *
 * Written by Gihong Do at 2017.8.1         *
 * Defines functions used to communicate    *
 *                                          *
 ********************************************/

#include "ACMessage.h"
#include "ARappClient.h"
#include "ACSharedData.h"

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

int ACM_Validate(struct ACM_Message *ACM_Valid_Message)
{
  if(ACM_Valid_Message -> Magic != AC_MAGIC) return AC_FALSE;
  return AC_TRUE;
}

int ACM_Pub_Message(enum ACM_Message_Type ACM_Type,
		     int From,
		     int To,
		     int Sockfd)
{
  struct ACM_Message Send_Message;
  int i;

  Send_Message.Magic = AC_MAGIC;
  Send_Message.Message_Type = ACM_Type;
  Send_Message.From = (uint8_t) From;
  Send_Message.To = (uint8_t) To;

  if(send(Sockfd,
	  (uint8_t *) &Send_Message,
	  sizeof(struct ACM_Message),
	  0) == -1)
    {
      fprintf(stderr,"ACM_Pub_Message: Send\n");
      return AC_FALSE;
    }

  return AC_TRUE;
}

int ACM_Get(int Gen_num, int Sockfd)
{
  int To;
  switch(Gen_num)
    {
    case 1:
      To = AC_GEN1;
      break;
    case 2:
      To = AC_GEN2;
      break;
    case 3:
      To = AC_GEN3;
      break;
    case 4:
      To = AC_GEN4;
      break;
    default:
      fprintf(stderr,"invalid generator number\n");
      return AC_FALSE;
    }

  if(!ACM_Pub_Message(GET,
		      ACSD_Shared.Marker_ID,
		      To,
		      Sockfd))
    {
      fprintf(stderr,"ACM_Get: Network problem\n");
      return AC_FALSE;
    }

  return AC_TRUE;
}

int ACM_Tackle(int Player_num, int Sockfd)
{
  int To;
  switch(Player_num)
    {
    case 1:
      To = AC_PLAYER1;
      break;
    case 2:
      To = AC_PLAYER2;
      break;
    case 3:
      To = AC_PLAYER3;
      break;
    case 4:
      To = AC_PLAYER4;
      break;
    default:
      fprintf(stderr,"invalid player number\n");
      return AC_FALSE;
    }

  if(!ACM_Pub_Message(STEAL,
		      ACSD_Shared.Marker_ID,
		      To,
		      Sockfd))
    {
      fprintf(stderr,"ACM_Get: Network problem\n");
      return AC_FALSE;
    }

  return AC_TRUE;
}

int ACM_Shoot(int Goal, int Sockfd)
{
  int To;

  if(Goal == 1)
    {
      To = AC_GOAL1;
    }
  if(Goal == 2)
    {
      To = AC_GOAL2;
    }

  if(!ACM_Pub_Message(SHOOT, ACSD_Shared.Marker_ID, To, Sockfd))
    {
      fprintf(stderr,"ACM_Shoot: Network problem\n");
      return AC_FALSE;
    }

  return AC_TRUE;
}
