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
  
