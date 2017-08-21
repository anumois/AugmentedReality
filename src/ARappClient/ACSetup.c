/*********************************************
 *                                           *
 * ACsetup.c                                 *
 *                                           *
 * Made by Gihong Do at 2017.8.4             *
 * Functions used for initialization         *
 *                                           *
 *********************************************/

#include "ARappClient.h"
#include "ACSetup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

static void *get_in_addr(struct sockaddr *sa);

// ACS_Network_Init
// Initialize network socket to be ready to communicate with the server
int ACS_Network_Init(char *ACS_Hostname, int *ACS_Sockfd)
{
  struct addrinfo ACS_Hints;
  struct addrinfo *ACS_Servinfo, *ACS_Pointer;
  int ACS_rv;
  char ACS_String[INET6_ADDRSTRLEN];
  
  memset(&ACS_Hints, 0, sizeof(ACS_Hints));
  ACS_Hints.ai_family = AF_UNSPEC;
  ACS_Hints.ai_socktype = SOCK_STREAM;

  printf("Name: %s\n", ACS_Hostname);
  if((ACS_rv = getaddrinfo(ACS_Hostname,
			   ACS_PORT,
			   &ACS_Hints,
			   &ACS_Servinfo)) != 0)
    {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ACS_rv));
      return AC_FALSE;
    }

  for(ACS_Pointer = ACS_Servinfo;
      ACS_Pointer != NULL;
      ACS_Pointer = ACS_Pointer->ai_next)
    {
      if((*ACS_Sockfd = socket(ACS_Pointer->ai_family,
			       ACS_Pointer->ai_socktype,
			       ACS_Pointer->ai_protocol)) == -1)
	{
	  fprintf(stderr,"ACS_Network_Init: socket\n");
	  continue;
	}
      printf("socket fd = %d\n", *ACS_Sockfd);
      if(connect(*ACS_Sockfd,
		 ACS_Pointer->ai_addr,
		 ACS_Pointer->ai_addrlen) == -1)
	{
	  close(*ACS_Sockfd);
	  fprintf(stderr, "ACS_Network_Init: connect\n");
	  continue;
	}
      break;
    }

  if(ACS_Pointer == NULL)
    {
      fprintf(stderr, "ACS_Network_Init: failed to connect\n");
      return AC_FALSE;
    }

  inet_ntop(ACS_Pointer -> ai_family,
	    get_in_addr((struct sockaddr *) ACS_Pointer -> ai_addr),
	    ACS_String,
	    sizeof(ACS_String));
  printf("ACS_Network_Init: Connecting to %s\n", ACS_String);

  freeaddrinfo(ACS_Servinfo);

  return AC_TRUE;
}

int ACS_SetupCamera(const char *cparam_name,
		    char *vconf,
		    ARParamLT **cparamLT_p,
		    ARHandle **arhandle,
		    AR3DHandle **ar3dhandle)
{
  ARParam cparam;
  int xsize, ysize;
  AR_PIXEL_FORMAT pixFormat;

  //Open the video path
  if(arVideoOpen(vconf) < 0)
    {
      fprintf(stderr,"ACS_SetupCamera(): Unable to open connection to camera.\n");
      return AC_FALSE;
    }

  // Find the size of the window
  if(arVideoGetSize(&xsize, &ysize) < 0)
    {
      fprintf(stderr, "ACS_SetupCamera(): Unable to determine camera frame size.\n");
      arVideoClose();
      return AC_FALSE;
    }

  printf("Camera image size(x,y) = (%d, %d)\n", xsize, ysize);

  // Get the format in which the camera is returning pixels.
  pixFormat = arVideoGetPixelFormat();
  if(pixFormat == AR_PIXEL_FORMAT_INVALID)
    {
      fprintf(stderr, "ACS_SetupCamera(): Camera is using unsupported pixel format\n");
      arVideoClose();
      return AC_FALSE;
    }

  // Load the camera parameters, resize for the window and init.
  if (arParamLoad(cparam_name, 1, &cparam) < 0)
    {
      fprintf(stderr, "ACS_SetupCamera(): Error loading parameter file %s for camera.\n", cparam_name);
      arVideoClose();
      return AC_FALSE;
    }

  if(cparam.xsize != xsize || cparam.ysize != ysize)
    {
      printf("*** Camera Parameter resized from %d, %d. ***\n",
	     cparam.xsize, cparam.ysize);
      arParamChangeSize(&cparam, xsize, ysize, &cparam);
    }

  #ifdef DEBUG
  printf("*** Camera Parameter ***\n");
  arParamDisp(&cparam);
  #endif

  if ((*cparamLT_p = arParamLTCreate(&cparam,
				     AR_PARAM_LT_DEFAULT_OFFSET)) == NULL)
    {
      fprintf(stderr, "ACS_SetupCamera(): Error: arParamLTCreate.\n");
      return AC_FALSE;
    }

  if((*arhandle = arCreateHandle(*cparamLT_p)) == NULL)
    {
      fprintf(stderr, "ACS_SetupCamera(): Error: arCreateHandle.\n");
      return AC_FALSE;
    }

  if(arSetPixelFormat(*arhandle, pixFormat) < 0)
    {
      fprintf(stderr, "ACS_SetupCamera(): Error: arSetPixelFormat.\n");
      return AC_FALSE;
    }

  if(arSetDebugMode(*arhandle, AR_DEBUG_DISABLE) < 0)
    {
      fprintf(stderr, "ACS_SetupCamera(): Error: arSetDebugMode.\n");
      return AC_FALSE;
    }

  if((*ar3dhandle = ar3DCreateHandle(&cparam)) == NULL)
    {
      fprintf(stderr, "ACS_SetupCamera(): Error: ar3DCreateHandle.\n");
      return AC_FALSE;
    }

  if( arVideoCapStart() != 0)
    {
      fprintf(stderr, "ACS_SetupCamera(): Unable to begin camera data capture.\n");
      return AC_FALSE;
    }

  return AC_TRUE;
}

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
		    ARPattHandle **pattHandle_p)
{

  if ((*pattHandle_p = arPattCreateHandle()) == NULL)
    {
      
      ARLOGe("setupMarker(): Error: arPattCreateHandle.\n");
      return (FALSE);
    }



  if ((*patt_id_gen1 = arPattLoad(*pattHandle_p, patt_gen1)) < 0)
    {
      ARLOGe("setupMarker(): Error loading gen1 pattern file %s.\n",
	     patt_gen1);

      arPattDeleteHandle(*pattHandle_p);
      return (FALSE);
    }

  if ((*patt_id_gen2 = arPattLoad(*pattHandle_p, patt_gen2)) < 0)
    {

      ARLOGe("setupMarker(): Error loading gen2 pattern file %s.\n",
	     patt_gen2);
      
      arPattDeleteHandle(*pattHandle_p);
      return (FALSE);
    }

  if ((*patt_id_gen3 = arPattLoad(*pattHandle_p, patt_gen3)) < 0)
    {
      
      ARLOGe("setupMarker(): Error loading gen3 pattern file %s.\n",
	     patt_gen3);

      arPattDeleteHandle(*pattHandle_p);

      return (FALSE);
    }

  if ((*patt_id_gen4 = arPattLoad(*pattHandle_p, patt_gen4)) < 0) {

    ARLOGe("setupMarker(): Error loading gen4 pattern file %s.\n", patt_gen4);

    arPattDeleteHandle(*pattHandle_p);

    return (FALSE);

  }

  if ((*patt_id_post1 = arPattLoad(*pattHandle_p, patt_post1)) < 0) {

    ARLOGe("setupMarker(): Error loading post1 pattern file %s.\n", patt_post1);

    arPattDeleteHandle(*pattHandle_p);

    return (FALSE);

  }

  if ((*patt_id_post2 = arPattLoad(*pattHandle_p, patt_post2)) < 0) {

    ARLOGe("setupMarker(): Error loading post2 pattern file %s.\n", patt_post2);

    arPattDeleteHandle(*pattHandle_p);

    return (FALSE);

  }

  if ((*patt_id_player1 = arPattLoad(*pattHandle_p, patt_player1)) < 0) {

    ARLOGe("setupMarker(): Error loading player1 pattern file %s.\n", patt_player1);

    arPattDeleteHandle(*pattHandle_p);

    return (FALSE);

  }

  if ((*patt_id_player2 = arPattLoad(*pattHandle_p, patt_player2)) < 0) {

    ARLOGe("setupMarker(): Error loading player2 pattern file %s.\n", patt_player2);

    arPattDeleteHandle(*pattHandle_p);

    return (FALSE);

  }

  if ((*patt_id_player3 = arPattLoad(*pattHandle_p, patt_player3)) < 0) {

    ARLOGe("setupMarker(): Error loading player3 pattern file %s.\n", patt_player3);

    arPattDeleteHandle(*pattHandle_p);

    return (FALSE);

  }

  if ((*patt_id_player4 = arPattLoad(*pattHandle_p, patt_player4)) < 0) {

    ARLOGe("setupMarker(): Error loading player4 pattern file %s.\n", patt_player4);

    arPattDeleteHandle(*pattHandle_p);

    return (FALSE);

  }

  arPattAttach(arhandle, *pattHandle_p);
  return (TRUE);
}

static void *get_in_addr(struct sockaddr *sa)
{
  return sa->sa_family == AF_INET
    ? (void *) &(((struct sockaddr_in *) sa) -> sin_addr)
    : (void *) &(((struct sockaddr_in6 *) sa) -> sin6_addr);
}

