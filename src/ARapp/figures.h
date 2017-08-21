#ifndef FIGURES_H
#define FIGURES_H

#include <GL/glut.h>

void
DrawHUD(float gDrawRotateAngle);

void
DrawNull(float gDrawRotateAngle);

void
DrawHUDUpdate(float timeDelta, float *gDrawRotateAngle, int gDrawRotate);

#endif
