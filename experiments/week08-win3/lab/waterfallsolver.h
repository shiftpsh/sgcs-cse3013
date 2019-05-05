#ifndef __WATERFALLSOLVER_H__
#define __WATERFALLSOLVER_H__

#include "stdafx.h"
#include "define.h"

void init_data(void);
void data_read(LPCTSTR fname);				//data 초기화 반환 수는 점의 갯수
void waterfall_Solver(void);				//문제를 푸는 함수 입력은 해당

void drawBackground(CDC* pDC);				//백그라운드를 그린다.
void drawStartPoint(CDC* pDC);				//현재 시작점을 나타낸다.
void drawWaterflow(CDC* pDC);				//현재 시작점에서 물의 흐름을 그린다.

void change_start_point(void);				//start point를 바꾸는 함수
void enable_drawWaterflow(bool mEnable);	//drawWaterflow를 가동시키는 함수

#endif