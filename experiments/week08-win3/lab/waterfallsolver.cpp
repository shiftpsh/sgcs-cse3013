#include "stdafx.h"
#include "mylist.h"
#include "waterfallsolver.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

Line *mLine = NULL;
Point *mPoint = NULL;
int mLine_num;
int mPoint_num;

mylist* m_flow_point = NULL;

bool init_state = false;
bool draw_state = false;
bool sele_state = false;
int  curr_point = 0;

// Calculate canvas-relative coordinates.
Point canvasCoords(Point p) {
	float x = gXmin + 20 * p.x;
	float y = gYmax - 20 * p.y;
	Point q = {x, y};
	return q;
}

void init_data(void){
	if(mLine!=NULL) free(mLine);
	if(mPoint!=NULL) free(mPoint);
	if(m_flow_point!=NULL) delete m_flow_point;
	mLine = NULL;
	mPoint = NULL;
	m_flow_point = NULL;

	mLine_num = 0;
	mPoint_num = 0;
	init_state = false;
	draw_state = false;
	sele_state = false;
	curr_point = 0;
}

void data_read(LPCTSTR fname){
	FILE* file;
	int i, tempx, tempy;

	file=fopen(fname,"r");
	/* line read */
	fscanf(file, "%d", &mLine_num);
	mLine = (Line*)malloc(sizeof(Line)*mLine_num);
	for(i=0; i<mLine_num; i++){
		fscanf(file, "%d %d %d %d\n", &(mLine[i].xl), &(mLine[i].yl), &(mLine[i].xr), &(mLine[i].yr));
		if(mLine[i].xl > mLine[i].xr){
				tempx = mLine[i].xl; tempy = mLine[i].yl;
				mLine[i].xl = mLine[i].xr; mLine[i].yl = mLine[i].yr;
				mLine[i].xr = tempx; mLine[i].yr = tempy;
		}
	}
		
	/* point read */
	fscanf(file, "%d", &(mPoint_num));
	mPoint = (Point*)malloc(sizeof(Point)*mPoint_num);
	for(i=0; i<mPoint_num; i++){
		fscanf(file, "%f %f\n", &(mPoint[i].x), &(mPoint[i].y));
	}
	fclose(file);
	init_state = true;
}

/* 문제는 물 그릴때 푼다. 아래의 drawWaterflow() 내에서 호출한다. */
void waterfall_Solver(void){
	m_flow_point = (mylist*) malloc(sizeof(mylist));
	
	if(init_state==false) return;
	
	Point P = mPoint[curr_point];
	m_flow_point->add(P);

	while (P.y > 0) {
		Line l;
		Point M = {0, 0};

		for (int i = 0; i < mLine_num; i++) {
			Line k = mLine[i];
			float kxmin = min(k.xl, k.xr);
			float kxmax = max(k.xl, k.xr);
			if (kxmin >= P.x || P.x >= kxmax) continue;

			float ratio = (P.x - k.xl) / (k.xr - k.xl);
			float Qy = ratio * (k.yr - k.yl) + k.yl;
			if (Qy < M.y) continue;
			if (Qy > P.y) continue;
			Point Q = {P.x, Qy};

			M = Q;
			l = k;
		}

		if (M.x == 0 && M.y == 0) {
			Point base = {P.x, 0};
			m_flow_point->add(base);
			break;
		} else {
			m_flow_point->add(M);
		}

		if (l.yl < l.yr) {
			Point p = {l.xl, l.yl};
			P = p;
		} else {
			Point p = {l.xr, l.yr};
			P = p;
		}

		m_flow_point->add(P);
	}
}

void drawBackground(CDC* pDC){
	int i; CPen MyPen; 

	if(init_state==false) return;

	// Boundaries
	MyPen.CreatePen(PS_SOLID, 10, RGB(0,0,154)); 
	pDC->SelectObject(MyPen);
	pDC->MoveTo(gXmin, gYmin); pDC->LineTo(gXmax, gYmin);
	pDC->MoveTo(gXmin, gYmax); pDC->LineTo(gXmax, gYmax);
	MyPen.Detach();

	// Points
	MyPen.CreatePen(PS_SOLID, 10, RGB(0, 0, 0)); 
	pDC->SelectObject(MyPen);
	for (int i = 0; i < mPoint_num; i++) {
		Point pt = canvasCoords(mPoint[i]);

		pDC->MoveTo(pt.x, pt.y); pDC->LineTo(pt.x, pt.y);
	}
	MyPen.Detach();

	// Lines
	MyPen.CreatePen(PS_SOLID, 3, RGB(180, 0, 0)); 
	pDC->SelectObject(MyPen);

	for (int i = 0; i < mLine_num; i++) {
		Line l = mLine[i];
		Point ls = {l.xl, l.yl};
		Point le = {l.xr, l.yr};

		Point start = canvasCoords(ls);
		Point end = canvasCoords(le);

		pDC->MoveTo(start.x, start.y); pDC->LineTo(end.x, end.y);
	}
	MyPen.Detach();
}

void drawStartPoint(CDC* pDC){
	CPen MyPen;
	if(init_state==false) return;
	if(sele_state==false) return;

	MyPen.CreatePen(PS_SOLID, 10, RGB(255, 0, 0)); 
	pDC->SelectObject(MyPen);

	Point pt = canvasCoords(mPoint[curr_point]);
	pDC->MoveTo(pt.x, pt.y); pDC->LineTo(pt.x, pt.y);

	MyPen.Detach();
}

void drawWaterflow(CDC* pDC){
	CPen MyPen;
	if(init_state==false) return;
	if(sele_state==false) return;
	if(draw_state==false) return;

	MyPen.CreatePen(PS_SOLID, 3, RGB(0, 255, 255)); 
	pDC->SelectObject(MyPen);

	node* temp;
	waterfall_Solver();

	if (m_flow_point->isEmpty()) return;
	temp = m_flow_point->move_first();

	Point curr = canvasCoords(*(temp->point));
	pDC->MoveTo(curr.x, curr.y);

	while ((temp = m_flow_point->move_next()) != NULL) {
		curr = canvasCoords(*(temp->point));
		pDC->LineTo(curr.x, curr.y);
	}

	MyPen.Detach();
}

void change_start_point(void){
	if (!sele_state) {
		curr_point = 0;
		sele_state = true;
	} else {
		curr_point++;
		curr_point %= mPoint_num;
	}
}

void enable_drawWaterflow(bool mEnable){
	if(init_state==false) return;
	draw_state = mEnable;
}