/****************************************
 *                                      *
 * ACSetup.h                            *
 *                                      *
 * Written by Gihong Do at 2017.8.7     *
 * Holds functions and constants for ini*
 * -tializing                           *
 *                                      *
 ****************************************/

#ifndef ACSETUP_H
#define ACSETUP_H

#define ACS_PORT "8080"

#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/gsub_lite.h>

struct ACS_InitBuf
{
  int MarkerID;
  int magic;
};

int ACS_Network_Init(char *ACS_Hostname, int *ACS_Sockfd);
int ACS_SetupMarker(const char *patt_gen1,
		    const char *patt_gen2,
		    const char *patt_gen3,
		    const char *patt_gen4,
		    const char *patt_post1,
		    const char *patt_post2,
		    const char *patt_player1,
		    const char *patt_player2,
		    const char *patt_player3,
		    const char *patt_player4,
		    int *patt_id_gen1,
		    int *patt_id_gen2,
		    int *patt_id_gen3,
		    int *patt_id_gen4,
		    int *patt_id_post1,
		    int *patt_id_post2,
		    int *patt_id_player1,
		    int *patt_id_player2,
		    int *patt_id_player3,
		    int *patt_id_player4,
		    ARHandle *arhandle,
		    ARPattHandle **pattHandle_p);

int ACS_SetupCamera(const char *cparam_name,
		    char *vconf,
		    ARParamLT **cparamLT_p,
		    ARHandle **arhandle,
		    AR3DHandle **ar3dhandle);

static int ACS_Valid(struct ACS_InitBuf);
#endif
