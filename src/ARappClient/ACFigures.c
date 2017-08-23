/********************************************
 *                                          *
 * ACFigures.c                              *
 *                                          *
 * Functions for drawing figures            *
 *                                          *
 ********************************************/

// Local Headers

#include "ACFigures.h"
#include "ARappClient.h"
#include "ACSharedData.h"

// For Drawing

#include <GL/glut.h>
#include <GL/glc.h>

// Common

#include <stdio.h>
#include <math.h>

void ACF_DrawCube(float gDrawRotateAngle, float fSize)
{
  int i;
  const GLfloat cube_vertices[8][3] = {
    /* +z */ {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
    /* -z */ {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f} };

  const GLubyte cube_vertex_colors[8][4] = {
    {255, 255, 255, 255},
    {255, 255,   0, 255},
    {  0, 255,   0, 255},
    {  0, 255, 255, 255},
    {255,   0, 255, 255},
    {255,   0,   0, 255},
    {  0,   0,   0, 255},
    {  0,   0, 255, 255} };

  const GLubyte cube_faces[6][4] = {
    {3, 2, 1, 0},
    {2, 3, 7, 6},
    {0, 1, 5, 4},
    {3, 0, 4, 7},
    {1, 2, 6, 5},
    {4, 5, 6, 7} };

  glPushMatrix();
  glRotatef(gDrawRotateAngle, 1.0f, 1.0f, 1.0f);

  glScalef(fSize, fSize, fSize);
  glTranslatef(0.0f, 0.0f, 0.5f);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, cube_vertex_colors);
  glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  for(i = 0; i < 6; i++)
    glDrawElements(GL_TRIANGLE_FAN,
		   4,
		   GL_UNSIGNED_BYTE,
		   &(cube_faces[i][0]));

  glDisableClientState(GL_COLOR_ARRAY);
  glColor4ub(0, 0, 0, 255);

  for(i = 0; i < 6; i++)
    {
      glDrawElements(GL_LINE_LOOP,
		     4,
		     GL_UNSIGNED_BYTE,
		     &(cube_faces[i][0]));
    }
  
  glPopMatrix();
}

void ACF_RenderText(int *font_id, char *family, char *face,
		    int scale, float r, float g, float b,
		    float x, float y, char *text, float angle,
		    int windowWidth, int windowHeight)
{
  glcContext(glcGenContext());
  *font_id = glcGenFontID();
  glcNewFontFromFamily(*font_id, family);
  glcFontFace(*font_id, face);
  glcFont(*font_id);

  glcScale(scale, scale);

  glColor3f(r, g, b);
  glcRotate(angle);

  glRasterPos2f(windowWidth -x, windowHeight - y);

  glcRenderString(text);
}

void ACF_DisplayText(float reshapeScale_w,
		     float reshapeScale_h,
		     int gShowHelp, int get,
		     int windowWidth, int windowHeight, int miss,
		     int gPatt_found_post1, int gPatt_found_post2)
{
  char *points_display_A = (char *)malloc(30);
  char *points_display_B = (char *)malloc(30);
  char *timer = (char *)malloc(30);
  int min, sec, milsec;
  double timed;
  int ball_font;
  int point_font_A, point_font_B;
  int score_font, timer_font, help_font;
  float scale = (reshapeScale_w + reshapeScale_h) / (float) 2;

  ACF_DrawBackground(430 * reshapeScale_w,
		     250 * reshapeScale_h,
		     0, 0,
		     windowWidth, windowHeight);

  ACF_DrawBackground(430 * reshapeScale_w,
		     250 * reshapeScale_h,
		     (WIDTH - 430) * reshapeScale_w,
		     0,
		     windowWidth, windowHeight);

  ACF_DrawBackground(650 * reshapeScale_w,
		     170 * reshapeScale_h,
		     600 * reshapeScale_w,
		     0,
		     windowWidth, windowHeight);

  if(get != 'y')
    {
      glColor3f(1.0, 1.0, 0.0);
      ACF_RenderText(&ball_font,
		     "Helvetica",
		     "bold",
		     50 * reshapeScale_w,
		     1.0, 1.0, 0.0,
		     250 * reshapeScale_w,
		     180 * reshapeScale_h,
		     "Ball", 0,
		     windowWidth, windowHeight);
      ACF_DrawFilledCircle((WIDTH - 340) * reshapeScale_w,
			   915 * reshapeScale_h,
			   25  * reshapeScale_w);
    }

  else
    {
      glColor3f(1.0, 1.0, 0.0);
      ACF_RenderText(&ball_font,
		     "Helvetica",
		     "bold",
		     50 * reshapeScale_w,
		     1.0, 1.0, 0.0,
		     (WIDTH - 120) * reshapeScale_w,
		     180 * reshapeScale_h,
		     "Ball", 0,
		     windowWidth, windowHeight);
      ACF_DrawFilledCircle(300 * reshapeScale_w,
			   915 * reshapeScale_h,
			   25  * reshapeScale_w);
    }


  ACSD_Acquire_Lock();
  sprintf(points_display_A, "Team A: %d", ACSD_Shared.TeamA_Score);
  sprintf(points_display_B, "%d :Team B", ACSD_Shared.TeamB_Score);
  ACSD_Release_Lock();
  
  ACF_RenderText(&point_font_A,
		 "FreeSerif",
		 "Bold",
		 70 * reshapeScale_w,
		 1.0, 0.0, 0.0,
		 (WIDTH - 50) * reshapeScale_w,
		 100 * reshapeScale_h,
		 points_display_A, 0,
		 windowWidth, windowHeight);

  ACF_RenderText(&point_font_B,
		 "FreeSerif",
		 "Bold",
		 70 * reshapeScale_w,
		 1.0, 0.0, 0.0,
		 380 * reshapeScale_w,
		 100 * reshapeScale_h,
		 points_display_B, 0,
		 windowWidth, windowHeight);

  if(miss == 1 &&(gPatt_found_post1 || gPatt_found_post2))
    {
      ACF_RenderText(&score_font,
		     "Helvetica",
		     "Bold",
		     80 * reshapeScale_w,
		     1.0, 1.0, 1.0,
		     1600 * reshapeScale_w,
		     600 * reshapeScale_h,
		     "YOU MISSED !!:(", 30,
		     windowWidth, windowHeight);
    }
  else if(miss == 0 && (gPatt_found_post1 || gPatt_found_post2))
    {
      ACF_RenderText(&score_font,
		     "Helvetica",
		     "Bold",
		     80 * reshapeScale_w,
		     1.0, 1.0, 1.0,
		     1600 * reshapeScale_w,
		     600 * reshapeScale_h,
		     "YOU SCORED !! :)", 30,
		     windowWidth, windowHeight);
    }

  //help key display
  ACF_RenderText(&help_font,
		 "Helvetica",
		 "Bold",
		 30 * reshapeScale_w,
		 1.0, 1.0, 1.0,
		 500 *reshapeScale_w,
		 1000 * reshapeScale_h,
		 "Press h for show/hide HELP", 0,
		 windowWidth, windowHeight);
  if(gShowHelp){
    ACF_PrintHelpKeys(reshapeScale_w, reshapeScale_h,
		      windowWidth, windowHeight);
  }
  glutSwapBuffers();
}

void ACF_DrawGoal(void)
{
  GLUquadric *pQuad;
  glPushMatrix();

  glColor3f(1, 0, 0);
  glScalef(40, 40, 40);
  glTranslatef(0.0f, 1.0f, 0.5f);
  glutSolidTorus(0.1, 1.0, 10, 50);

  glPopMatrix();

  glPushMatrix();
  glColor3f(1,0,0);
  glScalef(40, 40, 40);
  glTranslatef(0.0f, 0.0f, 0.5f);
  glRotatef(90.0, 1.0f, 0.0f ,0.0f);
  pQuad = gluNewQuadric();

  gluQuadricDrawStyle(pQuad, GLU_FILL);
  gluCylinder(pQuad, 0.1, 0.1, 3, 20, 20);
  glPopMatrix();
}

void ACF_DrawCrosshair(float windowWidth, float windowHeight,
		       int gPatt_found_post1, int gPatt_found_post2)
{
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, (double) windowWidth,
	  (double) windowHeight, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if(gPatt_found_post1 || gPatt_found_post2)
    {
      glColor3f(1.0, 0.0, 0.0);
    }
  else
    {
      glColor3f(0.0, 1.0, 0.0);
    }

  glLineWidth(4.0);

  int crossHair[8] =
    {
      windowWidth / 2 - 20, windowHeight/2, windowWidth/2 + 20, windowHeight/2,
      windowWidth/2, windowHeight/2 + 20, windowWidth/2, windowHeight/2 - 20 };

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_INT, 0, crossHair);
  glDrawArrays(GL_LINES, 0, 2);
  glDrawArrays(GL_LINES, 2, 2);

  glDisableClientState(GL_VERTEX_ARRAY);
  glPopMatrix();
}

void ACF_DrawScore(float ball_size, float *ball_dis)
{
  glPushMatrix();
  glColor3f(0.8, 0.8, 0);
  glScalef(ball_size, ball_size, ball_size);
  glTranslatef(0.0f, 1.5f, *ball_dis);

  *ball_dis -= 2;

  if(*ball_dis <= -20){
    glPopMatrix();
    return;
  }

  glutSolidSphere(0.7, 20, 20);
  glPopMatrix();

}

void ACF_DrawMiss(float ball_size, float *ball_dis, int *bounce)
{
  glPushMatrix();
  glColor3f(0.8, 0.8, 0);
  glScalef(ball_size, ball_size, ball_size);
  glTranslatef(0.0f, 1.5f, *ball_dis);
  if(*bounce == 0)
    {
      *ball_dis -= 1;
    }
  else
    {
      *ball_dis += 2;
    }

  if(*ball_dis <= -1)
    {
      *bounce = 1;
    }
  if(*ball_dis >= 20)
    {
      glPopMatrix();
      return;
    }

  glutSolidSphere(0.7, 20, 20);
  glPopMatrix();
}

void ACF_DrawBall(float ball_size, int *get)
{
  glPushMatrix();
  glColor3f(0.8, 0.8, 0);
  glScalef(ball_size, ball_size, ball_size);
  glTranslatef(0.0f, 0.0f, 0.5f);
  glutSolidSphere(0.7, 20, 20);
  glPopMatrix();
  *get = 'y';
}

void ACF_DrawFilledCircle(float x, float y, float radius)
{
  int i;
  int triangle_num = 20;
  float Pi = 3.14;
  glPushMatrix();
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y);
  for(i = 0; i <= triangle_num; i++)
    {
      glVertex2f(x + (radius * cos(i * 2 * Pi/triangle_num)),
		 y + (radius * sin(i * 2 * Pi/triangle_num)));
    }

  glEnd();
  glPopMatrix();
}

void ACF_DrawBackground(const float width, const float height,
			const float x, const float y,
			int windowWidth, int windowHeight)
{
  GLfloat vertices[4][2];

  vertices[0][0] = windowWidth  - width  -x;
  vertices[0][1] = windowHeight - height -y;
  vertices[1][0] = windowWidth  - x;
  vertices[1][1] = windowHeight - height -y;
  vertices[2][0] = windowWidth  - x;
  vertices[2][1] = windowHeight - y;
  vertices[3][0] = windowWidth  - width  -x;
  vertices[3][1] = windowHeight - y;

  glLoadIdentity();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_BLEND);
}

void ACF_DrawCubeUpdate(float timeDelta,
			float gDrawRotate,
			float *gDrawRotateAngle,
			float *fSize)
{
  if(gDrawRotate)
    {
      *gDrawRotateAngle += timeDelta * 45.0f;
      if(*gDrawRotateAngle > 360.0f)
	*gDrawRotateAngle -= 360.0f;
    }

  if(*fSize > 60)
    *fSize -= 1.0f;

  if(*fSize < 20)
    *fSize -= 1;
}

void ACF_PrintHelpKeys(float reshapeScale_w, float reshapeScale_h,
		       int windowWidth, int windowHeight)
{
  int i;
  char *helpText[] = {

    "Keys: \n",
    " h          : Show/hide this help.",
    " q or [esc] : Quit program.",
    " s          :  Shoot.",
    " t          :  Tackle.",
    " g          :  Get the ball."

  };

#define helpTextLineCount (sizeof(helpText)/sizeof(char *))
  int help_font[6];

  ACF_DrawBackground(650*reshapeScale_w,
		     700*reshapeScale_h,
		     850*reshapeScale_w,
		     400*reshapeScale_h,
		     windowWidth,
		     windowHeight);

  for (i = 0; i < helpTextLineCount; i++)
    {

      ACF_RenderText(&help_font[i],
		     "Helvetica",
		     "Bold",
		     40*reshapeScale_w,
		     1.0, 1.0, 1.0,
		     (WIDTH-500)*reshapeScale_w,
		     (500+100*i)*reshapeScale_h, helpText[i], 0,
		     windowWidth, windowHeight);

    }
}
