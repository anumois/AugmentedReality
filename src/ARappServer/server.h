#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <stdio.h>

// Headers for Network Configuration
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void serverFunc(int nSockfd);

#endif
