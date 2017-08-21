#include "setup.h"
#include <stdio.h>

int setupMarker(const char *patt_name, int *patt_id, ARHandle *arhandle, ARPattHandle **pattHandle_p)
{
  if((*pattHandle_p = arPattCreateHandle()) == NULL)
    {
      fprintf(stderr,"setupMarker(): Error: arPattCreateHandle.\n");
      return -1;
    }

  if((*patt_id = arPattLoad(*pattHandle_p, patt_name)) < 0)
    {
      fprintf(stderr,"setupMarker(): Error loading pattern file %s.\n", patt_name);
      arPattDeleteHandle(*pattHandle_p);
      return -1;
    }

  arPattAttach(arhandle, *pattHandle_p);
  return 1;
}

int setupCamera(const char *cparam_name, char *vconf, ARParamLT **cparamLT_p, ARHandle **arhandle, AR3DHandle **ar3dhandle)
{
  ARParam cparam;
  int xsize, ysize;
  AR_PIXEL_FORMAT pixFormat;

  if(arVideoOpen(vconf) < 0)
    {
      fprintf(stderr,"setupCamera(): Unable to open connection to camera.\n");
      return -1;
    }

  if(arVideoGetSize(&xsize, &ysize) < 0)
    {
      fprintf(stderr, "setupCamera(): Unable to determine camera frame size.\n");
      arVideoClose();
      return -1;
    }

  printf("Camera image size (x,y) = %d, %d)\n", xsize, ysize);

  pixFormat = arVideoGetPixelFormat();
  if(pixFormat == AR_PIXEL_FORMAT_INVALID)
    {
      fprintf(stderr,"setupCamera(): Camera is using unsupported pixel format.");
      arVideoClose();
      return -1;
    }

  if(arParamLoad(cparam_name, 1, &cparam) < 0)
    {
      fprintf(stderr, "setupCamera(): Error loading parameter file %s for camera. \n", cparam_name);
      arVideoClose();
      return -1;
    }

  if(cparam.xsize != xsize || cparam.ysize != ysize)
    {
      printf("*** Camera Parameter resize from %d, %d. ***\n", cparam.xsize, cparam.ysize);
      arParamChangeSize(&cparam, xsize, ysize, &cparam);
    }

  printf("*** Camera Parameter ***\n");
  arParamDisp(&cparam);

  if((*cparamLT_p = arParamLTCreate(&cparam, AR_PARAM_LT_DEFAULT_OFFSET)) == NULL)
    {
      fprintf(stderr,"setupCamera(): Error, arParamLTCreate.\n");
      return -1;
    }

  if((*arhandle = arCreateHandle(*cparamLT_p)) == NULL)
    {
      fprintf(stderr,"setupCamera(): Error: arCreateHandle.\n");
      return -1;
    }

  if(arSetPixelFormat(*arhandle, pixFormat) < 0)
    {
      fprintf(stderr,"setupCamera(): Error: arSetPixelFormat.\n");
      return -1;
    }

  if(arSetDebugMode(*arhandle, AR_DEBUG_DISABLE) < 0)
    {
      fprintf(stderr,"setupCamera(): Error: arSetDebugMode.\n");
      return -1;
    }

  if((*ar3dhandle = ar3DCreateHandle(&cparam)) == NULL)
    {
      fprintf(stderr, "setupCamera(): Error: ar3DcreateHandle.\n");
      return -1;
    }

  if(arVideoCapStart() != 0)
    {
      fprintf(stderr, "setupCamera(): Unable to begin camera data capture.\n");
      return -1;
    }
  return 1;
    
      
} 
