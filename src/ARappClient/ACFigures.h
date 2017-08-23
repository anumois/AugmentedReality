/**************************************************
 *                                                *
 * ACFigures.h                                    *
 *                                                *
 * Holds GL functions that draws custom figures   *
 *                                                *
 **************************************************/

#ifndef ACFIGURES_H
#define ACFIGURES_H

#include <sys/time.h>

void ACF_DrawFilledCircle(float x, float y, float radius);
void ACF_RenderText(int *font_id, char *family, char *face,
		    int scale, float r, float g, float b,
		    float x, float y, char *text, float angle,
		    int windowWidth, int windowHeight);

void ACF_DisplayText(float reshapeScale_w,
		     float reshapeScale_h,
		     int gShowHelp,
		     int get,
		     int windowWidth, int windowHeight, int miss, int first_start,
		     int gPatt_found_post1, int gPatt_found_post2, int gPatt_found_player2, int gPatt_found_player3, int gPatt_found_player4, int keyboard_input, struct timeval before, struct timeval after);
void ACF_DrawCube(float gDrawRotateAngle, float fSize);
void ACF_DrawGoal(void);
void ACF_DrawCrosshair(float windowWidth, float windowHeight,
		       int gPatt_found_post1, int gPatt_found_post2, int gPatt_found_player2, int gPatt_found_player3, int gPatt_found_player4);
void ACF_DrawScore(float ball_size, float *ball_dis);
void ACF_DrawPass(float ball_size, float *ball_dis_pass);
void ACF_DrawSteal(float ball_size, float *ball_dis_steal);
void ACF_DrawMiss(float ball_size, float *ball_dis, int *bounce);
void ACF_DrawBall(float ball_size, int *get);
void ACF_DrawBackground(const float width, const float height,
			const float x, const float y,
			int windowWidth, int windowHeight);

void ACF_DrawCubeUpdate(float timeDelta,
			float gDrawRotate,
			float *gDrawRotateAngle,
			float *fSize);

void ACF_PrintHelpKeys(float reshapeScale_w, float reshapeScale_h,
		       int windowWidth, int windowHeight);

#endif
