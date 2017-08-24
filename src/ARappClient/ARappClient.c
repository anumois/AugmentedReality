/**********************************************
 *                                            *
 * ARappClient.c                              *
 * 2017.8.4                                   *
 * Main function for the ARapp Client         *
 *                                            *
 **********************************************/

// Commonly used Headers

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

// Headers for Networking

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

// Headers for Graphics

#include <GL/glut.h>
#include <GL/glc.h>
#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/gsub_lite.h>

// Headers for Multithreaded Programming

#include <pthread.h>

// Local Headers

#include "ACSetup.h"
#include "ARappClient.h"
#include "ACSharedData.h"
#include "ACThread.h"
#include "ACFigures.h"

// Static Functions

static void Visibility(int visible);
static void Reshape(int w, int h);
static void Display(void);
static void mainLoop(void);
static void Timer(int value);
static void cleanup();
static void Keyboard(unsigned char key, int x, int y);

// Preferences

static int windowed = TRUE;
static int windowWidth = 640;
static int windowHeight = 480;
static int windowDepth = 32;
static int windowRefresh = 0;

// Image acquisition

static ARUint8 *gARTImage = NULL;
static int gARTImageSavePlease = FALSE;
static long gCallCountMarkerDetect = 0;
static ARHandle *gARHandle = NULL;
static ARPattHandle *gARPattHandle = NULL;
static int keyboard_input;
static int marker_gen1 = 0, marker_gen2 = 0, marker_gen3 = 0, marker_gen4 = 0;
static int marker_post1 = 0, marker_post2 = 0;
static int marker_player1 = 0, marker_player2 = 0, marker_player3 = 0, marker_player4 = 0;

// Transformation matrix retrieval

static AR3DHandle *gAR3DHandle = NULL;
static ARdouble gPatt_width_gen1 = 80.0;
static ARdouble gPatt_trans_gen1[3][4];
static int gPatt_found_gen1 = FALSE;
static int gPatt_id_gen1;

static ARdouble gPatt_width_gen2 = 80.0;
static ARdouble gPatt_trans_gen2[3][4];
static int gPatt_found_gen2 = FALSE;
static int gPatt_id_gen2;

static ARdouble gPatt_width_gen3 = 80.0;
static ARdouble gPatt_trans_gen3[3][4];
static int gPatt_found_gen3 = FALSE;
static int gPatt_id_gen3;

static ARdouble gPatt_width_gen4 = 80.0;
static ARdouble gPatt_trans_gen4[3][4];
static int gPatt_found_gen4 = FALSE;
static int gPatt_id_gen4;

static ARdouble gPatt_width_post1 = 80.0;
static ARdouble gPatt_trans_post1[3][4];
static int gPatt_found_post1 = FALSE;
static int gPatt_id_post1;

static ARdouble gPatt_width_post2 = 80.0;
static ARdouble gPatt_trans_post2[3][4];
static int gPatt_found_post2 = FALSE;
static int gPatt_id_post2;

static ARdouble gPatt_width_player1 = 80.0;
static ARdouble gPatt_trans_player1[3][4];
static int gPatt_found_player1 = FALSE;
static int gPatt_id_player1;

static ARdouble gPatt_width_player2 = 80.0;
static ARdouble gPatt_trans_player2[3][4];
static int gPatt_found_player2 = FALSE;
static int gPatt_id_player2;

static ARdouble gPatt_width_player3 = 80.0;
static ARdouble gPatt_trans_player3[3][4];
static int gPatt_found_player3 = FALSE;
static int gPatt_id_player3;

static ARdouble gPatt_width_player4 = 80.0;
static ARdouble gPatt_trans_player4[3][4];
static int gPatt_found_player4 = FALSE;
static int gPatt_id_player4;

// Drawing
static ARParamLT *gCparamLT = NULL;
static ARGL_CONTEXT_SETTINGS_REF gArglSettings = NULL;
static int gShowHelp = 0; //if 1 show help, 0 hide

static int gDrawRotate = TRUE;
static float gDrawRotateAngle = 0;
static float ARC_RotateAngle = 0;

static float ARC_fSize = 40.0f;
static float ARC_Ball_Size = 30;
static float ARC_Ball_Dis = 10;

static int get;
static int miss;
static int bounce = 0;

static int count = 0;
static int points = 0;
time_t start, end;
struct timeval before, after;

static float reshapeScale_w = 1;
static float reshapeScale_h = 1;

static float key_duration = 0.0f;

static int AC_Sockfd;

int main(int argc, char **argv)
{
  pthread_t AC_Tid;

  char glutGamemode[32];
  char cparam_name[] = "camera_para.dat";
  //  char vconf[] = "v4l2src device=/dev/video1 use-fixed-fps=false ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24,framerate=30/1 ! identity name=artoolkit sync=true ! fakesink";
  char vconf[] = "";
  char patt_name_gen1[] = "generator1.patt";
  char patt_name_gen2[] = "generator2.patt";
  char patt_name_gen3[] = "generator3.patt";
  char patt_name_gen4[] = "generator4.patt";
  char patt_name_post1[] = "goalpost1.patt"; 
  char patt_name_post2[] = "goalpost2.patt";
  char patt_name_player1[] = "player1.patt";
  char patt_name_player2[] = "player2.patt";
  char patt_name_player3[] = "player3.patt";
  char patt_name_player4[] = "player4.patt";
  
  if(argc != 2)
    {
      fprintf(stderr,"usage: client hostname\n");
      return AC_FALSE;
    }

  ACSD_Init();
  // Initialization Step
  if(!ACS_Network_Init(argv[1], &AC_Sockfd))
    {
      fprintf(stderr,"Network Init failed\n");
      return AC_FALSE;
    }

  glutInit(&argc, argv);

  // Video Setup
  if(!ACS_SetupCamera(cparam_name, vconf, &gCparamLT, &gARHandle, &gAR3DHandle))
    {
      fprintf(stderr,"main(): Unable to setup AR camera.\n");
      return AC_FALSE;
    }

  // Graphics Setup
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  if(!windowed)
    {
      if(windowRefresh)
	sprintf(glutGamemode, "%ix%i:%i@%i", windowWidth, windowHeight, windowDepth, windowRefresh);
      else
	sprintf(glutGamemode, "%ix%i:%i", windowWidth, windowHeight, windowDepth);
      glutEnterGameMode();
    }
  else
    {
      glutInitWindowSize(windowWidth, windowHeight);
      glutCreateWindow(argv[0]);
    }

  if((gArglSettings = arglSetupForCurrentContext(&(gCparamLT-> param),
						 arVideoGetPixelFormat())) == NULL)
    {
      fprintf(stderr,"main():arglSetupForCurrentContext() returned error.\n");
      cleanup();
      return AC_FALSE;
    }

  arglSetupDebugMode(gArglSettings, gARHandle);
  arUtilTimerReset();

  // Load Markers
  if(!ACS_SetupMarker(patt_name_gen1,
		      patt_name_gen2,
		      patt_name_gen3,
		      patt_name_gen4,
		      patt_name_post1,
		      patt_name_post2,
		      patt_name_player1,
		      patt_name_player2,
		      patt_name_player3,
		      patt_name_player4,
		      &gPatt_id_gen1,
		      &gPatt_id_gen2,
		      &gPatt_id_gen3,
		      &gPatt_id_gen4,
		      &gPatt_id_post1,
		      &gPatt_id_post2,
		      &gPatt_id_player1,
		      &gPatt_id_player2,
		      &gPatt_id_player3,
		      &gPatt_id_player4,
		      gARHandle, &gARPattHandle))
    {
      fprintf(stderr,"main(): Unable to set up AR marker.\n");
      cleanup();
      exit(-1);
    }
  
  pthread_create(&AC_Tid, NULL, ACT_Socket_Thread, (void *)&AC_Sockfd);

  glutDisplayFunc(Display);
  glutTimerFunc(1000, Timer, 1);
  glutReshapeFunc(Reshape);
  glutVisibilityFunc(Visibility);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();
  return AC_TRUE;
}

static void cleanup(void)
{
  arglCleanup(gArglSettings);
  gArglSettings = NULL;
  arPattDetach(gARHandle);
  arPattDeleteHandle(gARPattHandle);
  arVideoCapStop();
  ar3DDeleteHandle(&gAR3DHandle);
  arDeleteHandle(gARHandle);
  arParamLTFree(&gCparamLT);
  arVideoClose();
}

static void Timer(int value)
{
  ARC_RotateAngle = (ARC_RotateAngle + 10);
  if (ARC_RotateAngle > 360)
    ARC_RotateAngle -= 360;

  ARC_fSize += 2;
  
  glutPostRedisplay();
  glutTimerFunc(1000, Timer, 1);
}

static void Visibility(int visible)
{
  if (visible == GLUT_VISIBLE)
    {
      glutIdleFunc(mainLoop);
    }
  else
    {
      glutIdleFunc(NULL);
    }
}

static void Reshape(int w, int h)
{
  reshapeScale_w = (float)w / (float) WIDTH;
  reshapeScale_h = (float)h / (float) HEIGHT;

  windowWidth = w;
  windowHeight = h;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

static void Display(void)
{
  ARdouble p[16];
  ARdouble m[16];
  int found = AC_FALSE;
  glDrawBuffer(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  arglPixelBufferDataUpload(gArglSettings, gARTImage);
  arglDispImage(gArglSettings);
  gARTImage = NULL;

  // Projection transformation
  arglCameraFrustumRH(&(gCparamLT -> param),
		      VIEW_DISTANCE_MIN,
		      VIEW_DISTANCE_MAX,
		      p);

  glMatrixMode(GL_PROJECTION);

  #ifdef ARDOUBLE_IS_FLOAT
  glLoadMatrixf(p);
  #else
  glLoadMatrixd(p);
  #endif

  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  glLoadIdentity();

  if (gPatt_found_post1)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_post1,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACF_DrawGoal();

      if(keyboard_input == 's')
	{
	  ACF_DrawGoal();
	  if(miss == 1)
	    {
	      ACF_DrawMiss(ARC_Ball_Size, &ARC_Ball_Dis, &bounce);
	    }
	  else
	    {
	      ACM_Shoot(1,AC_Sockfd);
	      //ACF_DrawScore(ARC_Ball_Size, &ARC_Ball_Dis);
	    }
	}
    }

  if (gPatt_found_post2)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_post2,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACF_DrawGoal();

      if(keyboard_input == 's')
	{
	  ACF_DrawGoal();
	  if(miss == 1)
	    {
	      ACF_DrawMiss(ARC_Ball_Size, &ARC_Ball_Dis, &bounce);
	    }
	  else
	    {
	      ACM_Shoot(2, AC_Sockfd);
	      //ACF_DrawScore(ARC_Ball_Size, &ARC_Ball_Dis);
	    }
	}
    }

  if (gPatt_found_gen1)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_gen1,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACSD_Acquire_Lock();
      found = ACSD_Shared.Ball_Holder[AC_GEN1]; 
      ACSD_Release_Lock();

      if(found)
	{
	  ACF_DrawBall(ARC_Ball_Size, &get);
	}
      else
	{
	  ACF_DrawCube(ARC_Ball_Size, ARC_fSize);
	}
      if(keyboard_input == 'g')
	{
	  ACM_Get(1, AC_Sockfd);
	}
      found = AC_FALSE;
    }

  if (gPatt_found_gen2)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_gen2,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACSD_Acquire_Lock();
      found = ACSD_Shared.Ball_Holder[AC_GEN1]; 
      ACSD_Release_Lock();

      if(found)
	{
	  ACF_DrawBall(ARC_Ball_Size, &get);
	}
      else
	{
	  ACF_DrawCube(ARC_Ball_Size, ARC_fSize);
	}
      if(keyboard_input == 'g')
	{
	  ACM_Get(2, AC_Sockfd);
	}
      found = AC_FALSE;
    }

  if (gPatt_found_gen3)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_gen3,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACSD_Acquire_Lock();
      found = ACSD_Shared.Ball_Holder[AC_GEN1]; 
      ACSD_Release_Lock();

      if(found)
	{
	  ACF_DrawBall(ARC_Ball_Size, &get);
	}
      else
	{
	  ACF_DrawCube(ARC_Ball_Size, ARC_fSize);
	}
      if(keyboard_input == 'g')
	{
	  ACM_Get(3, AC_Sockfd);
	}
      found = AC_FALSE;
    }

  if (gPatt_found_gen4)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_gen4,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACSD_Acquire_Lock();
      found = ACSD_Shared.Ball_Holder[AC_GEN1]; 
      ACSD_Release_Lock();

      if(found)
	{
	  ACF_DrawBall(ARC_Ball_Size, &get);
	}
      else
	{
	  ACF_DrawCube(ARC_Ball_Size, ARC_fSize);
	}
      if(keyboard_input == 'g')
	{
	  ACM_Get(4, AC_Sockfd);
	}
      found = AC_FALSE;
    }

   if (gPatt_found_player1)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_player1,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACSD_Acquire_Lock();
      found = ACSD_Shared.Ball_Holder[AC_PLAYER1];
      ACSD_Release_Lock();

      if(found)
	{
	  ACF_DrawBall(ARC_Ball_Size, &get);
	}

      if(keyboard_input == 't')
	{
	  ACM_Tackle(1, AC_Sockfd);
	}
      found = AC_FALSE;
    }

   if (gPatt_found_player2)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_player2,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACSD_Acquire_Lock();
      found = ACSD_Shared.Ball_Holder[AC_PLAYER2];
      ACSD_Release_Lock();

      if(found)
	{
	  ACF_DrawBall(ARC_Ball_Size, &get);
	}

      if(keyboard_input == 't')
	{
	  ACM_Tackle(2, AC_Sockfd);
	}
      found = AC_FALSE;
    }

   if (gPatt_found_player3)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_player3,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACSD_Acquire_Lock();
      found = ACSD_Shared.Ball_Holder[AC_PLAYER3];
      ACSD_Release_Lock();

      if(found)
	{
	  ACF_DrawBall(ARC_Ball_Size, &get);
	}

      if(keyboard_input == 't')
	{
	  ACM_Tackle(3, AC_Sockfd);
	}
      found = AC_FALSE;
    }

   if (gPatt_found_player4)
    {
      arglCameraViewRH((const ARdouble (*)[4]) gPatt_trans_player4,
		       m,
		       VIEW_SCALEFACTOR);

      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      ACSD_Acquire_Lock();
      found = ACSD_Shared.Ball_Holder[AC_PLAYER4];
      ACSD_Release_Lock();

      if(found)
	{
	  ACF_DrawBall(ARC_Ball_Size, &get);
	}

      if(keyboard_input == 't')
	{
	  ACM_Tackle(4, AC_Sockfd);
	}
      found = AC_FALSE;
    }

  glDisable(GL_LIGHTING);
  ACF_DrawCrosshair(windowWidth, windowHeight,
		    gPatt_found_post1, gPatt_found_post2);

  //Any 2D overlays go here

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, (GLdouble) windowWidth, 0, (GLdouble) windowHeight, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  ACF_DisplayText(reshapeScale_w,
		  reshapeScale_h,
		  gShowHelp,
		  get,
		  windowWidth, windowHeight, miss,
		  gPatt_found_post1, gPatt_found_post2);
  glutSwapBuffers();
}
		      
static void mainLoop(void)
{
  static int imageNumber = 0;
  static int ms_prev;
  int ms;
  float s_elapsed;
  ARUint8 *image;
  ARdouble err;
  int j, gen1, gen2, gen3, gen4, post1, post2, player1, player2, player3, player4;

  // Find out how long since mainLoop() last ran
  ms = glutGet(GLUT_ELAPSED_TIME);
  s_elapsed = (float)(ms - ms_prev) * 0.001f;
  key_duration += (float)(ms - ms_prev) * 0.001f;
  if(key_duration > 1.0f) //Once pressed Key is available for 1 second
    {
      key_duration = 0;
      keyboard_input = ' ';
    }
  if(s_elapsed < 0.2f) return; //Don't update more often than 100 Hz

  ms_prev = ms;

  ACF_DrawCubeUpdate(s_elapsed,
		     gDrawRotate,
		     &gDrawRotateAngle,
		     &ARC_fSize);

  if((image = arVideoGetImage()) != NULL)
    {
      gARTImage = image;
    }

  if( (gARTImageSavePlease))
    {
      char imageNumberText[15];
      sprintf(imageNumberText,
	      "image-%04d.jpg",
	      imageNumber++);
      if(arVideoSaveImageJPEG(gARHandle -> xsize,
			      gARHandle -> ysize,
			      gARHandle -> arPixelFormat,
			      gARTImage,
			      imageNumberText, 75, 0) < 0)
	{
	  fprintf(stderr,"Error saving video image. \n");
	}

      gARTImageSavePlease = AC_FALSE;
    }

    gCallCountMarkerDetect++;

    // Detect the markers in the video frame
    if (arDetectMarker(gARHandle, gARTImage) < 0)
      {
	exit(-1);
      }
    
    gen1 = -1;
    gen2 = -1;
    gen3 = -1;
    gen4 = -1;
    post1 = -1;
    post2 = -1;
    player1 = -1;
    player2 = -1;
    player3 = -1;
    player4 = -1;

    for(j = 0; j < gARHandle -> marker_num; j++)
      {
	if(gARHandle -> markerInfo[j].id == gPatt_id_gen1)
	  {
	    if(gen1 == -1) gen1 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[gen1].cf)
	      gen1 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_gen2)
	  {
	    if(gen2 == -1) gen2 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[gen2].cf)
	      gen2 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_gen3)
	  {
	    if(gen3 == -1) gen3 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[gen3].cf)
	      gen3 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_gen4)
	  {
	    if(gen4 == -1) gen4 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[gen4].cf)
	      gen4 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_post1)
	  {
	    if(post1 == -1) post1 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[post1].cf)
	      post1 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_post2)
	  {
	    if(post2 == -1) post2 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[post2].cf)
	      post2 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_player1)
	  {
	    if(player1 == -1) player1 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[player1].cf)
	      player1 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_player2)
	  {
	    if(player2 == -1) player2 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[player2].cf)
	      player2 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_player3)
	  {
	    if(player3 == -1) player3 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[player3].cf)
	      player3 = j;
	  }

	 if (gARHandle -> markerInfo[j].id == gPatt_id_player4)
	  {
	    if(player4 == -1) player4 = j;
	    else if (gARHandle -> markerInfo[j].cf > gARHandle -> markerInfo[player4].cf)
	      player4 = j;
	  }
      }

    fprintf(stderr,
	    "Coordinate %f %f %f\n",
	    gPatt_trans_gen1[0][3],
	    gPatt_trans_gen1[1][3],
	    gPatt_trans_gen1[2][3]);

    //Get the transformation between the marker and the real camera into gPatt_trans
    
    if(gen1 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[gen1]),
				  gPatt_width_gen1,
				  gPatt_trans_gen1);

	if((gPatt_trans_gen1[0][3] > -AIM_SIZE) &&
	   (gPatt_trans_gen1[0][3] < AIM_SIZE))
	  {
	    gPatt_found_gen1 = TRUE;
	    marker_gen1 = 1;
	  }

	else
	  {
	    gPatt_found_gen1 = FALSE;
	    marker_gen1 = 0;
	  }
      }

    else
      {
	gPatt_found_gen1 = FALSE;
	marker_gen1 = 0;
      }

    if (gen2 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[gen2]),
				  gPatt_width_gen2,
				  gPatt_trans_gen2);
	gPatt_found_gen2 = TRUE;
	marker_gen2 = 1;
      }

    else
      {
	gPatt_found_gen2 = FALSE;
	marker_gen2 = 0;
      }

    if (gen3 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[gen3]),
				  gPatt_width_gen3,
				  gPatt_trans_gen3);
	gPatt_found_gen3 = TRUE;
	marker_gen3 = 1;
      }

    else
      {
	gPatt_found_gen3 = FALSE;
	marker_gen3 = 0;
      }

    if (gen4 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[gen4]),
				  gPatt_width_gen4,
				  gPatt_trans_gen4);
	gPatt_found_gen4 = TRUE;
	marker_gen4 = 1;
      }

    else
      {
	gPatt_found_gen4 = FALSE;
	marker_gen4 = 0;
      }

    if (post1 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[post1]),
				  gPatt_width_post1,
				  gPatt_trans_post1);
	gPatt_found_post1 = TRUE;
	marker_post1 = 1;
      }

    else
      {
	gPatt_found_post1 = FALSE;
	marker_post1 = 0;
      }

    if (post2 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[post2]),
				  gPatt_width_post2,
				  gPatt_trans_post2);
	gPatt_found_post2 = TRUE;
	marker_post2 = 1;
      }

    else
      {
	gPatt_found_post2 = FALSE;
	marker_post2 = 0;
      }

    if (player1 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[player1]),
				  gPatt_width_player1,
				  gPatt_trans_player1);
	gPatt_found_player1 = TRUE;
	marker_player1 = 1;
      }

    else
      {
	gPatt_found_player1 = FALSE;
	marker_player1 = 0;
      }

    if (player2 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[player2]),
				  gPatt_width_player2,
				  gPatt_trans_player2);
	gPatt_found_player2 = TRUE;
	marker_player2 = 1;
      }

    else
      {
	gPatt_found_player2 = FALSE;
	marker_player2 = 0;
      }

    if (player3 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[player3]),
				  gPatt_width_player3,
				  gPatt_trans_player3);
	gPatt_found_player3 = TRUE;
	marker_player3 = 1;
      }

    else
      {
	gPatt_found_player3 = FALSE;
	marker_player3 = 0;
      }

    if (player4 != -1)
      {
	err = arGetTransMatSquare(gAR3DHandle,
				  &(gARHandle -> markerInfo[player4]),
				  gPatt_width_player4,
				  gPatt_trans_player4);
	gPatt_found_player4 = TRUE;
	marker_player4 = 1;
      }

    else
      {
	gPatt_found_player4 = FALSE;
	marker_player4 = 0;
      }
}

static void Keyboard(unsigned char key, int x, int y)
{
  key_duration = 0.0f;
  switch(key)
    {
    case 'a':
      keyboard_input = 'a';
      break;
    case 't':
      keyboard_input = 't';
      break;
    case 's':
      keyboard_input = 's';
      srand(time(NULL));
      //70% chance to score
      if(rand() % 10 > 6)
	{
	  miss = 1;
	}
      else
	{
	  miss = 0;
	  points++;
	}
      ARC_Ball_Dis = 10;
      bounce = 0;
      break;
    case 'g':
      keyboard_input = 'g';
      break;
    case 'h':
      keyboard_input = 'h';
      gShowHelp++;
      if(gShowHelp > 1) gShowHelp = 0;
      break;
    case 'q':
    case 0x1B:
      cleanup();
      exit(0);
      break;
    }
}
