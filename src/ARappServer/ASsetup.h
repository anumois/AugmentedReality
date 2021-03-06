/********************************************
 *                                          *
 * Header file for ASsetup.c                *  
 * Made by Gihong Do 2017.7.31              *
 * Usage: Initialization for ARappServer.c  *
 *                                          *
 ********************************************/

#ifndef ASSETUP_H
#define ASSETUP_H

// Constants

#define AS_PORT "8080"
#define AS_BACKLOG 10

struct ARS_InitBuf
{
  int MarkerID;
  int magic;
};

int ASsetup_Network_Init(int *Server_Sockfd);
void ASsetup_Game_Init(int *Client_Sockfd_Array);
#endif
