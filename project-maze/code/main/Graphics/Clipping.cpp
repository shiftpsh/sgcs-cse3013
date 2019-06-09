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
#pragma warning(disable:4786)

#include <deque>
#include "..\UI\Zoom.h"
#include "Graphics_DS.h"

#define CT_LEFT		1
#define CT_TOP		2
#define CT_RIGHT	3
#define CT_BOTTOM	4

// for polygon clippling
static std::deque<POINTDBL> vec1;
static bool g_fBoundCheck;
double minx, miny, maxx, maxy;
std::deque<POINTDBL> vec2;

static int EdgeClipping(const POINTDBL* inVec, int inLen, std::deque<POINTDBL>& outVec, double edge, int type);
static int EdgeClipping(const std::deque<POINTDBL>& inVec, int inLen, std::deque<POINTDBL>& outVec, double edge, int type);
static void Intersect(const POINTDBL& inPt, const POINTDBL& outPt, POINTDBL& newPt, double, int type);
static bool Inside(const POINTDBL& pt, double, int type);
static void Output(std::deque<POINTDBL>& v, int& len, const POINTDBL& newPt);
bool ClipLine(double x1, double y1, double x2, double y2, 
               double *xx1, double *yy1, double *xx2, double *yy2);
bool ClipBox(double x1, double y1, double x2, double y2);
void CheckBoxPos(long* dx1, long* dy1, long* dx2, long* dy2);
int ClipPolygon(POINTDBL* inArr, int inLen);

// 주어진 좌표가 Zoom에 정의된 physical area내부에 존재하는지 확인
bool ClipLine(double x1, double y1, double x2, double y2, 
               double *xx1, double *yy1, double *xx2, double *yy2)
{
	double xorg, xmax, yorg, ymax;
	int c1L = 0, c1R = 0, c1T = 0, c1B = 0, c2L = 0, c2R = 0, c2T = 0, c2B = 0;
	bool vis = false;
	int in1 = 0, in2 = 0;
	double tx, ty, m; // m is the slope
	int    tc;

	// clipping area 설정
	xorg = Pxorg > g_xorg ? Pxorg : g_xorg;
	xmax = Pxmax < g_xmax ? Pxmax : g_xmax;
	yorg = Pyorg > g_yorg ? Pyorg : g_yorg;
	ymax = Pymax < g_ymax ? Pymax : g_ymax;

	while(1) 
	{
		c1L = c1R = c1T = c1B = c2L = c2R = c2T = c2B = in1 = in2 = 0;
		// encode c1 and c2
		if ( x1 < xorg )
			c1L = 1;
		if ( x1 > xmax )
			c1R = 1;
		if ( y1 < yorg )
			c1B = 1;
		if ( y1 > ymax )
			c1T = 1;
		if ( x2 < xorg )
			c2L = 1;
		if ( x2 > xmax )
			c2R = 1;
		if ( y2 < yorg )
			c2B = 1;
		if ( y2 > ymax )
			c2T = 1;

		if ( c1L == 0 && c1T == 0 && c1R == 0 && c1B == 0 )
			in1 = 1;
		if ( c2L == 0 && c2T == 0 && c2R == 0 && c2B == 0 )
			in2 = 1;
		if ( in1 == 1 && in2 == 1 ) 
		{
			vis = true;
			break;
		}
		else if ( (c1L==1 && c2L==1) || (c1R==1 && c2R==1) ||
              (c1T==1 && c2T==1) || (c1B==1 && c2B==1)    )
		{
			vis = false;
			break;
		}
		else 
		{
			if ( in1 == 1 ) 
			{
				SWAP(x1,x2,tx); SWAP(y1,y2,ty);
				SWAP(c1L,c2L,tc); SWAP(c1R,c2R,tc); SWAP(c1T,c2T,tc); SWAP(c1B,c2B,tc);
			}
			if ( x1 == x2 ) // Line is vertical
			{ 
				if ( c1T == 1 )
					y1 = ymax;
				else if ( c1B == 1 )
					y1 = yorg;
			}
			else	// Not a vertical Line
			{
				m = (y2-y1)/(x2-x1);
				if ( c1L == 1 ) 
				{
					y1 += (xorg - x1)*m; x1 = xorg;
				}
				else if ( c1R == 1 )
				{
					y1 += (xmax - x1)*m; x1 = xmax;
				}
				else if ( c1B == 1 ) 
				{
					x1 += (yorg - y1)/m; y1 = yorg;
				}
				else if ( c1T == 1 ) 
				{
					x1 += (ymax - y1)/m; y1 = ymax;
				}
			}
		}
	}
	*xx1 = x1; *yy1 = y1; *xx2 = x2; *yy2 = y2;
	return ( vis );
}

bool ClipBox(double x1, double y1, double x2, double y2)
{
	double xorg, xmax, yorg, ymax;

	xorg = Pxorg > g_xorg ? Pxorg : g_xorg;
	xmax = Pxmax < g_xmax ? Pxmax : g_xmax;
	yorg = Pyorg > g_yorg ? Pyorg : g_yorg;
	ymax = Pymax < g_ymax ? Pymax : g_ymax;

	// 두 점의 x좌표가 모두 최소보다 작거나, 최대보다 크면 view의 밖에 있다
	if (x1 < xorg && x2 < xorg)
		return false;
	if (x1 > xmax && x2 > xmax)
		return false;
	// y좌표도 마찬가지
	if (y1 < yorg && y2 < yorg)
		return false;
	if (y1 > ymax && y2 > ymax)
		return false;

	// 위의 조건에 맞지 않으면 viewport에 걸쳐있다
	return true;
}

void CheckBoxPos(long* dx1, long* dy1, long* dx2, long* dy2)
{
	if (*dx1 < 0)
		*dx1 = -5;
	else if (*dx1 > Dxmax)
		*dx1 = round(Dxmax) + 5;
	if (*dx2 < 0)
		*dx2 = -5;
	else if (*dx2 > Dxmax)
		*dx2 = round(Dxmax) + 5;
	if (*dy1 < 0)
		*dy1 = -5;
	else if (*dy1 > Dymax)
		*dy1 = round(Dymax) + 5;
	if (*dy2 < 0)
		*dy2 = -5;
	else if (*dy2 > Dymax)
		*dy2 = round(Dymax) + 5;
}

// for polygon clipping
static void Output(std::deque<POINTDBL>& v, int& len, const POINTDBL& newPt)
{
	v.push_back(newPt);
	len++;
	// polygon의 min, max영역 check
	if (g_fBoundCheck)
	{
		if (newPt.x > maxx)
			maxx = newPt.x;
		if (newPt.x < minx)
			minx = newPt.x;
		if (newPt.y > maxy)
			maxy = newPt.y;
		if (newPt.y < miny)
			miny = newPt.y;
	}
}

static bool Inside(const POINTDBL& pt, double edge, int type)
{
	bool flag = false;

	switch(type)
	{
	case CT_LEFT:
		flag = (edge <= pt.x);
		break;
	case CT_RIGHT:
		flag = (pt.x <= edge);
		break;
	case CT_TOP:
		flag = (pt.y <= edge);
		break;
	case CT_BOTTOM:
		flag = (edge <= pt.y);
		break;
	}
	return flag;
}

static void Intersect(const POINTDBL& inPt, const POINTDBL& outPt, POINTDBL& newPt, double edge, int type)
{
	double m;

	if (inPt.x != outPt.x)
		m = (inPt.y - outPt.y) / (inPt.x - outPt.x);
	switch(type)
	{
	case CT_LEFT:
	case CT_RIGHT:
		newPt.x = edge;
		newPt.y = outPt.y + (edge - outPt.x) * m;
		break;
	case CT_BOTTOM:
	case CT_TOP:
		newPt.y = edge;
		if (inPt.x != outPt.x)
			newPt.x = outPt.x + (edge - outPt.y) / m;
		else
			newPt.x = outPt.x;
		break;
	}
}

static int EdgeClipping(const std::deque<POINTDBL>& inVec, int inLen, std::deque<POINTDBL>& outVec, double edge, int type)
{
	POINTDBL s, p, v;
	int outLen = 0, j;

	if (inLen == 0)
		return 0;
	s = inVec.at(inLen - 1);

	for(j = 0; j < inLen; j++)
	{
		p = inVec.at(j);
		if (Inside(p, edge, type))
		{
			if (Inside(s, edge, type))
				Output(outVec, outLen, p);
			else
			{
				Intersect(p, s, v, edge, type);
				Output(outVec, outLen, v);
				Output(outVec, outLen, p);
			}
		}
		else if (Inside(s, edge, type))
		{
				Intersect(s, p, v, edge, type);
				Output(outVec, outLen, v);
		}
		s = p;
	}
	return outLen;
}

static int EdgeClipping(const POINTDBL* inVec, int inLen, std::deque<POINTDBL>& outVec, double edge, int type)
{
	POINTDBL s, p, v;
	int outLen = 0, j;

	if (inLen == 0)
		return false;
	s = inVec[inLen - 1];

	for(j = 0; j < inLen; j++)
	{
		p = inVec[j];
		if (Inside(p, edge, type))
		{
			if (Inside(s, edge, type))
				Output(outVec, outLen, p);
			else
			{
				Intersect(p, s, v, edge, type);
				Output(outVec, outLen, v);
				Output(outVec, outLen, p);
			}
		}
		else if (Inside(s, edge, type))
		{
				Intersect(s, p, v, edge, type);
				Output(outVec, outLen, v);
		}
		s = p;
	}
	return outLen;
}

// polygon clipping main function
int ClipPolygon(POINTDBL* inArr, int inLen)
{
	int vec1Len, vec2Len;

	g_fBoundCheck = false;
	if (!vec1.empty())
		vec1.clear();
	vec1Len = EdgeClipping(inArr, inLen, vec1, Pxorg, CT_LEFT);
	if (vec1Len == 0)
		return false;
	vec2.clear();
	vec2Len = EdgeClipping(vec1, vec1Len, vec2, Pxmax, CT_RIGHT);
	if ( vec2Len == 0)
		return false;
	vec1.clear();
	vec1Len = EdgeClipping(vec2, vec2Len, vec1, Pyorg, CT_BOTTOM);
	if (vec1Len == 0)
		return false;
	minx = Pxmax; miny = Pymax;
	maxx = Pxorg; maxy = Pyorg;
	g_fBoundCheck = true;
	vec2.clear();
	return EdgeClipping(vec1, vec1Len, vec2, Pymax, CT_TOP);
}
