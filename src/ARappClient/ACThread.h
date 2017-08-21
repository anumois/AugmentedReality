/****************************************************
 *                                                  *
 * ACThread.h                                       *
 *                                                  *
 * Written by Gihong Do at 2017.8.7                 *
 * Functions used by socket process                 *
 *                                                  *
 ****************************************************/

#ifndef ACTHREAD_H
#define ACTHREAD_H

#include "ACMessage.h"

void *ACT_Socket_Thread(void *AC_Sockfd);
void ACT_Process_Message(struct ACM_Message *Recv_Message); 

#endif
