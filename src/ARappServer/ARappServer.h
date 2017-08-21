/*************************************************************
 *                                                           *
 * Header file for ARappServer                               *
 * Made by Gihong Do 2017.7.31                               *
 * Usage: Common Constants used throughout ARappServer       *
 *                                                           *
 *************************************************************/

#ifndef ARAPPSERVER_H
#define ARAPPSERVER_H

//Constants

#define AS_FALSE 0
#define AS_TRUE  1
#define AS_NUM_OF_MARKER 10
#define AS_NUM_OF_PLAYER 4

#define AS_RANDGEN_1 AS_NUM_OF_PLAYER
#define AS_RANDGEN_2 AS_NUM_OF_PLAYER + 1
#define AS_RANDGEN_3 AS_NUM_OF_PLAYER + 2
#define AS_RANDGEN_4 AS_NUM_OF_PLAYER + 3

#define AS_GOAL_A AS_NUM_OF_PLAYER + 4
#define AS_GOAL_B AS_NUM_OF_PLAYER + 5

#define AS_MAGIC 0xBEEFBFFE
#endif
