/**************************************************************************
***    
***  Copyright (c) 2000 Design Automation Lab at the Sogang University,
***
***  Affiliation:   DA Lab, Sogang University, Computer Science Department,
***                 Seoul, Korea
***
***  Permission is hereby granted, free of charge, to any person obtaining 
***  a copy of this software and associated documentation files (the
***  "Software"), to deal in the Software without restriction, including
***  without limitation 
***  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
***  and/or sell copies of the Software, and to permit persons to whom the 
***  Software is furnished to do so, subject to the following conditions:
***
***  The above copyright notice and this permission notice shall be included
***  in all copies or substantial portions of the Software.
***
*** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
*** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
*** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
*** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
*** THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***
***
***************************************************************************/

#include "..\stdafx.h"
#include "..\ExternView.h"
#include "..\Util\Math.h"
#include "Scroll.h"

#define MARGIN_RATIO	0.1				// Size + Size * MARGIN_RATIO will be drawn in the display
#define ZOOM_OUT_LIMIT	10					// 실물의 10%크기 이하로는 축소 불가
const double ZOOM_IN_LIMIT = 1.0e-5;		// Physical window의 가로(세로) 확대 최대 범위

#define F_trans(x1, y1, x2, y2) {x2 = round(Tx1*x1+Tx2); y2 = round(Ty1*y1+Ty2); }
#define I_trans(x1, y1, x2, y2) {x2 = ITx1*x1+ITx2; y2 = ITy1 * y1 + ITy2; }
#define SWAP(x,y,t) {t=x;x=y;y=t;}

typedef struct
{
	double wndMinX, wndMinY, wndMaxX, wndMaxY;
	double viewMinX, viewMinY, viewMaxX, viewMaxY;
	double devWidth, devHeight;
	double Tx1, Tx2, Ty1, Ty2;
	double ITx1, ITx2, ITy1, ITy2;
	double xOffset, yOffset;
	short coordType;
} SCALABLEVIEW;

//extern int UpsideDown_Flag;	// 0: normal, 1: window-like (OnDraw_Main.cpp)
int UpsideDown_Flag = 0;

// zoom 관련 변수
double g_xorg = 0, g_yorg = 0, g_xmax = 0, g_ymax = 0;	// original view size
double Pxorg, Pyorg, Pxmax, Pymax;							// Physical Window Dimention
double Fxorg, Fyorg, Fxmax, Fymax;							// view size + offset
double Dxmax, Dymax;										// Device Window Dimention

// 좌표변환식 상수
double Tx1, Tx2, Ty1, Ty2;                      // Physical to device coord transform constants
double ITx1, ITx2, ITy1, ITy2;                  // device to Physical coord transform constants

static SCALABLEVIEW g_scaleView;

// function prototype
void AdjustView();
void Coeff_calc();
void setWindow(double xorg, double yorg, double xmax, double ymax, int updown);
void saveWindow();
void restoreWindow();
void viewFull();
void zoomIn(int x1, int y1, int x2, int y2);
void zoomOut(int x1, int y1, int x2, int y2);

void setWindow(double xorg, double yorg, double xmax, double ymax, int updown)
{
	// physical window의 크기 저장
	g_xorg = xorg;
	g_yorg = yorg;
	g_xmax = xmax;
	g_ymax = ymax;

	double Offset_x = (MARGIN_RATIO * (xmax - xorg)) / 2.0;
	double Offset_y = (MARGIN_RATIO * (ymax - yorg)) / 2.0;
	
	// Set original view size
	Fxorg = xorg - Offset_x;
	Fyorg = yorg - Offset_y;
	Fxmax = xmax + Offset_x;
	Fymax = ymax + Offset_y;

	// Set org and window size for drawing
	Pxorg = Fxorg;
	Pyorg = Fyorg;
	Pxmax = Fxmax;
	Pymax = Fymax;

	UpsideDown_Flag = updown;
	AdjustView ( );	// Give offset to fit into the current device window
	Coeff_calc();
}

void saveWindow()
{
	g_scaleView.wndMinX = Fxorg;
	g_scaleView.wndMinY = Fyorg;
	g_scaleView.wndMaxX = Fxmax;
	g_scaleView.wndMaxY = Fymax;
	g_scaleView.viewMinX = Pxorg;
	g_scaleView.viewMinY = Pyorg;
	g_scaleView.viewMaxX = Pxmax;
	g_scaleView.viewMaxY = Pymax;
	g_scaleView.devWidth = Dxmax;
	g_scaleView.devHeight = Dymax;
	g_scaleView.coordType = UpsideDown_Flag;
}

void restoreWindow()
{
	Fxorg = g_scaleView.wndMinX;
	Fyorg = g_scaleView.wndMinY;
	Fxmax = g_scaleView.wndMaxX;
	Fymax = g_scaleView.wndMaxY;
	Pxorg = g_scaleView.viewMinX;
	Pyorg = g_scaleView.viewMinY;
	Pxmax = g_scaleView.viewMaxX;
	Pymax = g_scaleView.viewMaxY;
	Dxmax = g_scaleView.devWidth;
	Dymax = g_scaleView.devHeight;
	UpsideDown_Flag = g_scaleView.coordType;
}

void AdjustView()
{
	double Delta;
	double Pwidth, Pheight;
	
	Pwidth = Pxmax - Pxorg;
	Pheight = Pymax - Pyorg;

	// Adjust view setting
	if (Dxmax * Pheight < Dymax * Pwidth)
	{
		// Adjust y size
		Delta = Pwidth * Dymax / Dxmax - Pheight;
		Pyorg -= Delta / 2.0;
		Pymax += Delta / 2.0;
	}
	else if (Dxmax * Pheight > Dymax * Pwidth)
	{
	    // Adjust x size
		Delta = Dxmax * Pheight / Dymax - Pwidth;
		Pxorg -= Delta / 2.0;
		Pxmax += Delta / 2.0;
	}
}

void Coeff_calc()
{
	double Pwidth, Pheight;

	Pwidth = Pxmax - Pxorg;
	Pheight = Pymax - Pyorg;

	Tx1 = Dxmax / Pwidth;
	Tx2 = -Pxorg * Dxmax / Pwidth;
	ITx1 = Pwidth / Dxmax;
	ITx2 = Pxorg;

	if (UpsideDown_Flag == 0)
	{	// 수학 좌표계
		Ty1 = -Dymax / Pheight;
		Ty2 = Pymax * Dymax / Pheight;
		ITy1 = -Pheight / Dymax;
		ITy2 = Pymax;
	}
	else if (UpsideDown_Flag == 1)
	{
		// 윈도우 좌표계(MM_TEXT)
		Ty1 = Dymax / Pheight;
		Ty2 = -Pyorg * Dymax / Pheight;
		ITy1 = Pheight / Dymax;
		ITy2 = Pyorg;
	}
}


void zoomIn(int left, int top, int right, int bottom)
{
	double px1, py1, px2, py2;	// physical coords for x1, y1, x2, y2
	double t;

	I_trans(left, top, px1, py1);
	I_trans(right, bottom, px2, py2);

	if (UpsideDown_Flag == 1)
		SWAP(py1, py2, t);
	if ( px1 < px2 && py1 > py2 && (px2 - px1) > ZOOM_IN_LIMIT && (py1 - py2) > ZOOM_IN_LIMIT)
	{
		Pxorg = px1;
		Pxmax = px2;
		Pyorg = py2;
		Pymax = py1;

		AdjustView();

		Coeff_calc();
		setScroll();
	}
}

void zoomOut(int left, int top, int right, int bottom)
{
	const double Max_Zoom = 5.0; // min zoom is 1.0

	double wdt, hgt;
	double cx, cy;
	double icx, icy, iwdt, ihgt;
	double zrx, zry;

	// get the current rows and height
	wdt = Pxmax - Pxorg;
	hgt = Pymax - Pyorg;

	// mouse로 선택된 영역의 rows, height를 구한다
	iwdt = right - left; ihgt = bottom - top;

	icx = left + iwdt / 2.0;
	icy = top + ihgt / 2.0;
	I_trans(icx, icy, cx, cy);

	// Get the ratio(0.0 < zr <1.0)
	zrx = iwdt / Dxmax;
	zry = ihgt / Dymax;
	zrx = Max_Zoom * zrx + 1.0;
	zry = Max_Zoom * zry + 1.0;

	// Get new rows and height
	wdt = wdt * zrx;
	hgt = hgt * zry;

	if (wdt / ZOOM_OUT_LIMIT < Fxmax - Fxorg && hgt / ZOOM_OUT_LIMIT < Fymax - Fyorg)
	{
		// 수학 좌표계
		Pymax = cy + hgt / 2.0;
		Pyorg = cy - hgt / 2.0;
		Pxmax = cx + wdt / 2.0;
		Pxorg = cx - wdt / 2.0;

		AdjustView();
		Coeff_calc();

		setScroll();

		// 100% 이하로 축소되었을 때 scroll bar를 없앤다
		if (Pxmax - Pxorg > Fxmax - Fxorg)
			g_pView->SetScrollRange(SB_HORZ, 0, 0);
		if (Pymax - Pyorg > Fymax - Fyorg)
			g_pView->SetScrollRange(SB_VERT, 0, 0);
	}
}

void viewFull() 
{
	Pxorg = Fxorg;
	Pyorg = Fyorg;
	Pxmax = Fxmax;
	Pymax = Fymax;

	AdjustView();	
	Coeff_calc();

	// scroll bar를 없앤다
	g_pView->SetScrollRange(SB_VERT, 0, 0);
	g_pView->SetScrollRange(SB_HORZ, 0, 0);
}
