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

#ifndef __DRAW_FUNC_H__
#define __DRAW_FUNC_H__

#include "..\StdAfx.h"
#include "Graphics_DS.h"

extern bool initBuffer();
extern void freeBuffer();
extern void clearBuffer();
extern void showBuffer();
extern void printBuffer(CDC* pDC, int cx, int cy, int rows, int height);
extern void refresh();
extern int setROP2(int nOpCode);
extern unsigned char setAlpha(unsigned char a);

extern void DrawFont_I(double x1, double y1, double x2, double y2, COLORREF crColor, LPTSTR szFont, LPTSTR szStr);
extern void DrawText_I(double x1, double y1, PTSTR szStr);
extern void DrawLine_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor);
extern void DrawScaledLine_I(POINTDBL* pt, int nNum, double rows, COLORREF crLineColor);
extern void DrawBox_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor, int nType);
extern void DrawBox_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor);
extern void DrawSolidBox_I(double x1, double y1, double x2, double y2, 
				  int nLineWidth, COLORREF nLineColor, COLORREF nFillColor);
extern void DrawHatchBox_I(double x1, double y1, double x2, double y2, int nLineWidth, COLORREF nLineColor, 
				  int nType, COLORREF nFillColor);
extern void DrawPatternBox_I(double x1, double y1, double x2, double y2, 
					  int nWidth, COLORREF crLineColor, COLORREF crFillColor, int nPatternSize, char* pPattern);
extern void DrawPolyline_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor);
extern void DrawPolygon_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor);
extern void DrawHatchPolygon_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, int type, COLORREF crFillColor);
extern void DrawPatternPolygon_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor, int nSize, char* pPattern);

extern void DrawLine(CDC* pDC, double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor);
extern void DrawBox(CDC* pDC, double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor);
extern void DrawSolidBox(CDC* pDC, double x1, double y1, double x2, double y2, 
				  int nLineWidth, COLORREF nLineColor, COLORREF nFillColor);
extern void DrawHatchBox(CDC* pDC, double x1, double y1, double x2, double y2, int nLineWidth, COLORREF nLineColor, 
				  int nType, COLORREF nFillColor);
extern void DrawPolyline(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor);
extern void DrawPolygon(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor);
extern void DrawHatchPolygon(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, int type, COLORREF crFillColor);
extern void DrawFont(CDC* pDC, double x1, double y1, double x2, double y2, COLORREF crColor, LPTSTR szFont, LPTSTR szStr);

#endif	// __DRAW_FUNC_H__
