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

#ifndef __ZOOM_H__
#define __ZOOM_H__

#include "..\Util\Math.h"

// macro 선언
#define F_trans(x1, y1, x2, y2) {x2 = round(Tx1*x1+Tx2); y2 = round(Ty1*y1+Ty2); }
#define I_trans(x1, y1, x2, y2) {x2 = ITx1*x1+ITx2; y2 = ITy1 * y1 + ITy2; }
#define SWAP(x,y,t) {t=x;x=y;y=t;}

// flag
extern bool g_bZoom;
extern bool g_bZoomMode;
extern bool g_bFullView;

// zoom 관련
extern double g_xorg, g_yorg, g_xmax, g_ymax;			// original view size
extern double Pxorg, Pyorg, Pxmax, Pymax;              // Physical Window Dimention
extern double Fxorg, Fyorg, Fxmax, Fymax;				// view size + offset
extern double Dxmax, Dymax;                            // Device Window Dimention

extern int UpsideDown_Flag;

// 좌표변환식 상수
extern double Tx1, Tx2, Ty1, Ty2;                      // Physical to device coord transform constants
extern double ITx1, ITx2, ITy1, ITy2;                  // device to Physical coord transform constants

extern void setWindow(double xorg, double yorg, double xmax, double ymax, int updown);
extern void saveWindow();
extern void restoreWindow();
extern void viewFull();
extern void zoomIn(int left, int top, int right, int bottom);
extern void zoomOut(int left, int top, int right, int bottom);
extern void AdjustView();
extern void Coeff_calc();

#endif	// __ZOOM_H__