#include "figures.h"
#include <stdio.h>

GLfloat Alpha = 0.5f;
GLenum Src = GL_SRC_ALPHA;
GLenum Dest = GL_ONE_MINUS_SRC_ALPHA;

void
DrawHUD(float gDrawRotateAngle)
{
  int i;
  float fSize = 40.0f;

  glPushMatrix();

  glRotatef(gDrawRotateAngle, 0.0f, 0.0f, 1.0f);
  glScalef(fSize, fSize, fSize);
  glTranslatef(0.0f, 0.0f, 1.5f);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(Src, Dest);

  glColor4f(0, 1, 0, Alpha);
  glBegin(GL_QUADS);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, -0.5);
  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5, 0.5);
  glEnd();
  glPopMatrix();
  glDisable(GL_BLEND);
}

void
DrawNull(float gDrawRotateAngle)
{
  int i;
  float fSize = 40.0f;
  glPushMatrix();

  glRotatef(gDrawRotateAngle, 0.0f, 0.0f, 1.0f);
  glScalef(fSize, fSize, fSize);
  glTranslatef(0.0f, 0.0f, 1.5f);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(Src, Dest);

  glColor4f(1, 0, 0, Alpha);
  glBegin(GL_QUADS);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, -0.5);
  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5, 0.5);
  glEnd();
  glPopMatrix();
  glDisable(GL_BLEND);
}
void
DrawHUDUpdate(float timeDelta, float *gDrawRotateAngle, int gDrawRotate)
{
  if(gDrawRotate)
    {
      *gDrawRotateAngle += timeDelta * 45.0f;
      if(*gDrawRotateAngle > 360.0f) *gDrawRotateAngle -= 360.0f;
    }
}
