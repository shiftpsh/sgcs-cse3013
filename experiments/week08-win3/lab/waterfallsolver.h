#ifndef __WATERFALLSOLVER_H__
#define __WATERFALLSOLVER_H__

#include "stdafx.h"
#include "define.h"

void init_data(void);
void data_read(LPCTSTR fname);				//data �ʱ�ȭ ��ȯ ���� ���� ����
void waterfall_Solver(void);				//������ Ǫ�� �Լ� �Է��� �ش�

void drawBackground(CDC* pDC);				//��׶��带 �׸���.
void drawStartPoint(CDC* pDC);				//���� �������� ��Ÿ����.
void drawWaterflow(CDC* pDC);				//���� ���������� ���� �帧�� �׸���.

void change_start_point(void);				//start point�� �ٲٴ� �Լ�
void enable_drawWaterflow(bool mEnable);	//drawWaterflow�� ������Ű�� �Լ�

#endif