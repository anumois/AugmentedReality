/*
 * 2017 Individual Study Augmented Reality Drone application
 *
 */

// Header Files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <GL/glut.h>
#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/gsub_lite.h>

// Headers for Network Configuration
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Header for multithreaded programming
#include <pthread.h>

#include "setup.h"
#include "figures.h"
#include "client.h"
#include "sharedData.h"

// Constants
#define NETWORK_PORT "8080"
#define VIEW_SCALEFACTOR  1.0
#define VIEW_DISTANCE_MIN 40.0
#define VIEW_DISTANCE_MAX 10000.0

//
// Globlal Variables
//
static int windowed = TRUE;
static int windowWidth = 640;
static int windowHeight = 480;
static int windowDepth = 32;
static int windowRefresh = 0;

// Image acquisition
static ARUint8 *gARTImage = NULL;
static int gARTImageSavePlease = FALSE;

// Marker detection
static ARHandle *gARHandle = NULL;
static ARPattHandle *gARPattHandle = NULL;
static long gCallCountMarkerDetect = 0;

// Transformation matrix retrieval
static AR3DHandle *gAR3DHandle = NULL;
static ARdouble gPatt_width = 80.0;
static ARdouble gPatt_trans[3][4];
static int gPatt_found = FALSE;
static int gPatt_id;

// Drawing
static ARParamLT *gCparamLT = NULL;
static ARGL_CONTEXT_SETTINGS_REF gArglSettings = NULL;
static int gShowMode = 1;
float gDrawRotateAngle = 0;
static int gDrawRotate = TRUE;

// Network Variables
int *nSockfd;
struct addrinfo nHints, *nServinfo, *nPointer;
int nRv;
char ns[INET6_ADDRSTRLEN];

// Game related variables
int gain = 0;
int sent = 0;

// Functions
static void cleanup(void);
static void Display(void);
static void Visibility(int visible);
static void Keyboard(unsigned char key, int x, int y);
static void Reshape(int width, int height);
static void mainLoop(void);
static void UpdateAngle(float timeDelta);
static int NetworkInit(char *hostname);

static void *get_in_addr(struct sockaddr *sa);

int main(int argc, char **argv)
{
  char glutGamemode[32];
  //File that holds calibration data of the webcam
  char *cparam_name = "Data/camera_para.dat";
  char *vconf = "";
  //File that holds the information of the marker
  char *patt_name = "Data/hiro.patt";

  //variables for multithread programming
  pthread_t tid;
  
  //Network setup
  if(argc != 2)
    {
      fprintf(stderr,"usage: client hostname\n");
      exit(1);
    }

  NetworkInit(argv[1]);
  sema_init();
  DEBUGprintItem();
  pthread_create(&tid, NULL, ClientThread, (void *)nSockfd); 
  //Hardware setup
  printf("thread created\n");
  glutInit(&argc, argv);
  if(!setupCamera(cparam_name, vconf, &gCparamLT, &gARHandle, &gAR3DHandle))
    {
      fprintf(stderr, "main(): Unable to set up AR camera.\n");
      exit(-1);
     }

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  if(!windowed)
    {
      if(windowRefresh)
	sprintf(glutGamemode, "%ix%i: %i@%i",windowWidth, windowHeight,
		windowDepth, windowRefresh);
      else
	sprintf(glutGamemode, "%ix%i:%i", windowWidth, windowHeight,
		windowDepth);
      glutGameModeString(glutGamemode);
      glutEnterGameMode();
    }

  else
    {
      glutInitWindowSize(windowWidth, windowHeight);
      glutCreateWindow(argv[0]);
    }

  //Setting up GL context for OpenGL to draw
  if((gArglSettings = arglSetupForCurrentContext(&(gCparamLT->param),
						 arVideoGetPixelFormat())) == NULL)
    {
      fprintf(stderr,"main(): arglSetupForCurrentContext() returned error.\n");
      cleanup();
      exit(-1);
    }

  arglSetupDebugMode(gArglSettings, gARHandle);
  arUtilTimerReset();

  if(!setupMarker(patt_name, &gPatt_id, gARHandle, &gARPattHandle))
    {
      fprintf(stderr,"main(): Unable to set up AR marker\n");
      cleanup();
      exit(-1);
    }

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutVisibilityFunc(Visibility);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();
  return 0;
}

static void cleanup(void)
{
  close(*nSockfd);
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

static void Display(void)
{
  ARdouble p[16];
  ARdouble m[16];

  glDrawBuffer(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  arglPixelBufferDataUpload(gArglSettings, gARTImage);
  arglDispImage(gArglSettings);

  gARTImage = NULL;

  arglCameraFrustumRH(&(gCparamLT->param), VIEW_DISTANCE_MIN,
		      VIEW_DISTANCE_MAX, p);

  glMatrixMode(GL_PROJECTION);
#ifdef ARDOUBLE_IS_FLOAT
  glLoadMatrixf(p);
#else
  glLoadMatrixd(p);
#endif

  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);

  //Viewing transformation
  glLoadIdentity();

  if(gPatt_found)
    {
      arglCameraViewRH((const ARdouble(*)[4])gPatt_trans, m, VIEW_SCALEFACTOR);
      #ifdef ARDOUBLE_IS_FLOAT
      glLoadMatrixf(m);
      #else
      glLoadMatrixd(m);
      #endif

      sema_down();
     
      if(isMarkerValid(0))
	{
	  
	  DrawHUD(gDrawRotateAngle);
	  if(gain == 1)
	    {
	      uint8_t sending[NUMBER_OF_MARKERS] = {0, };
	      sending[0] = 1;
	      struct MsgTemplate *senderMsg = FillMsg(sending);
	      
	      if(send(*nSockfd, (uint8_t *)senderMsg, sizeof(struct MsgTemplate), 0) == -1)
		{
		  fprintf(stderr,"send\n");
		  sema_up();
		  exit(-1);
		}
	    }	
	}
      else
	{
	  
	  DrawNull(gDrawRotateAngle);
	}
	  
      //Draw something here
      
      sema_up();
      
    }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, (GLdouble) windowWidth, 0, (GLdouble)windowHeight, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  glutSwapBuffers();
}

static void Reshape(int width, int height)
{
  windowWidth = width;
  windowHeight = height;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

static void Visibility(int visible)
{
  if(visible == GLUT_VISIBLE)
    glutIdleFunc(mainLoop);
  else
    glutIdleFunc(NULL);
}

static void mainLoop(void)
{
  static int imageNumber = 0;
  static int ms_prev;
  int ms;
  float s_elapsed;
  ARUint8 *image;
  ARdouble err;
  int j, k;

  ms = glutGet(GLUT_ELAPSED_TIME);
  s_elapsed = (float)(ms - ms_prev) * 0.001f;
  if(s_elapsed < 0.01f) return; //Ignore more than 100Hz

  ms_prev = ms;
  
  //update drawing
  UpdateAngle(s_elapsed);
  
  //Fetch a video frame
  if((image = arVideoGetImage()) != NULL)
    {
      gARTImage = image; //save fetched image
      
      if(gARTImageSavePlease)
	{
	  char imageNumberText[15];
	  sprintf(imageNumberText, "image-%04d.jpg", imageNumber++);
	  if(arVideoSaveImageJPEG(gARHandle->xsize, gARHandle->ysize,
				  gARHandle->arPixelFormat, gARTImage,
				  imageNumberText, 75, 0) < 0 )
	    fprintf(stderr,"Error saving video image.\n");
	  
	  gARTImageSavePlease = FALSE;
	}
      
      gCallCountMarkerDetect++;
      
      //Detecting markers in the video frame
      if(arDetectMarker(gARHandle, gARTImage) < 0)
	exit(-1);
      
      k = -1;
      
      for(j = 0; j < gARHandle->marker_num; j++)
	{
	  if(gARHandle->markerInfo[j].id == gPatt_id)
	    {
	      if(k == -1) k = j;
	      else if(gARHandle->markerInfo[j].cf > gARHandle->markerInfo[k].cf)
		k = j;
	    }
	}
      
      if(k != -1)
	{
	  err = arGetTransMatSquare(gAR3DHandle, &(gARHandle->markerInfo[k]),
				    gPatt_width, gPatt_trans);
	  gPatt_found = TRUE;
	}
      else
	gPatt_found = FALSE;

      glutPostRedisplay();
    }
}
	  
static void UpdateAngle(float timeDelta)
{
  if(gDrawRotate)
    {
      gDrawRotateAngle += timeDelta * 45.0f;

      if(gDrawRotateAngle > 360.0f)
	gDrawRotateAngle -= 360.0f;
    }
} 

static void *get_in_addr(struct sockaddr *sa)
{
  if(sa->sa_family == AF_INET)
    {
      return &(((struct sockaddr_in *) sa) -> sin_addr);
    }

  return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

static int NetworkInit(char *hostname)
{
  nSockfd = (int *) malloc(sizeof(int));
  memset(&nHints, 0, sizeof(nHints));
  nHints.ai_family = AF_UNSPEC;
  nHints.ai_socktype = SOCK_STREAM;

  if((nRv = getaddrinfo(hostname, NETWORK_PORT, &nHints, &nServinfo)) != 0)
    {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(nRv));
      return FALSE;
    }

  for (nPointer = nServinfo; nPointer != NULL; nPointer = nPointer -> ai_next)
    {
      if((*nSockfd = socket(nPointer->ai_family, nPointer->ai_socktype,
			   nPointer->ai_protocol)) == -1)
	{
	  fprintf(stderr, "client: socket\n");
	  continue;
	}

      if(connect(*nSockfd, nPointer->ai_addr, nPointer->ai_addrlen) == -1)
	{
	  close(*nSockfd);
	  fprintf(stderr, "client: connect\n");
	  continue;
	}

      break;
    }

  if(nPointer == NULL)
    {
      fprintf(stderr, "client: failed to connect\n");
      return FALSE;
    }

  inet_ntop(nPointer->ai_family,
	    get_in_addr((struct sockaddr *)nPointer->ai_addr),
	    ns, sizeof(ns));

  printf("client: connecting to %s\n", ns);

  freeaddrinfo(nServinfo);

  return TRUE;
}

static void Keyboard(unsigned char key, int x, int y)
{
  switch(key)
    {
    case 'a':
      gain = 1;
      break;
    case 's':
      gain = 0;
      break;
    }
  char info[128];
  sprintf(info, "gain = %d", gain);
  glutSetWindowTitle(info);
  glutPostRedisplay();
}
