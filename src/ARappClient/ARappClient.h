/******************************************
 *                                        *
 * ARappClient.h                          *
 *                                        *
 * Written by Gihong Do at 2017.8.4       *
 * Holds constants used overall in this   *
 * Project                                *
 *                                        *
 ******************************************/

#ifndef ARAPPCLIENT_H
#define ARAPPCLIENT_H

#define AC_TRUE 1
#define AC_FALSE 0
#define AC_NUM_OF_MARKER 10
#define AC_MAGIC 0xBEEFBFFE

#define VIEW_SCALEFACTOR 1.0
#define VIEW_DISTANCE_MIN 40.0
#define VIEW_DISTANCE_MAX 10000.0

#define WIDTH  1920 //default window size to scale
#define HEIGHT 1080

#define AIM_SIZE 30

enum markerID
  {
    AC_PLAYER1,
    AC_PLAYER2,
    AC_PLAYER3,
    AC_PLAYER4,
    AC_GEN1,
    AC_GEN2,
    AC_GEN3,
    AC_GEN4,
    AC_GOAL1,
    AC_GOAL2
  };
#endif
