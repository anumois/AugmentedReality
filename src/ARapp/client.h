#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include "sharedData.h"
#define CHECKSUM_MAGIC 0xBEEF

void *ClientThread(void *nSockfd);
int ValidateMSG(struct MsgTemplate *msgbuf);
struct MsgTemplate *FillMsg(uint8_t *array);
void DEBUGprintMsg(struct MsgTemplate *msg);
#endif
