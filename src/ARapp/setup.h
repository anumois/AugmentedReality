#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/gsub_lite.h>


int setupCamera(const char *cparam_name, char *vconf, ARParamLT **cparamLT_p, ARHandle **arhandle, AR3DHandle **ar3dhandle);

int setupMarker(const char *patt_name, int *patt_id, ARHandle *arhandle,
		ARPattHandle **pattHandle_p);
