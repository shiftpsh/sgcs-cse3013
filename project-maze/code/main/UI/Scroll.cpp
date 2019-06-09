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

#include <math.h>
#include "..\StdAfx.h"
#include "..\ExternView.h"
#include "..\Util\Math.h"
#include "Zoom.h"

const int SCROLL_RANGE = 100;
double x_scale, y_scale;
double m_xPageSize, m_yPageSize, m_xLineSize, m_yLineSize;	// scroll시 page, line의 크기
double m_xScrollPos, m_yScrollPos;							// scroll 위치

// function prototype
void setScroll();
void handleHScroll(UINT nSBCode, UINT nPos);
void handleVScroll(UINT nSBCode, UINT nPos);
void handleSize(UINT nType, int cx, int cy);

void setScroll()
// Scroll을 위한 data setting
{
	SCROLLINFO si;
	double xmax, xorg, ymax, yorg;

	xmax = max(Pxmax, Fxmax);
	xorg = min(Pxorg, Fxorg);
	ymax = max(Pymax, Fymax);
	yorg = min(Pyorg, Fyorg);

	x_scale = (xmax - xorg) / SCROLL_RANGE;
	y_scale = (ymax - yorg) / SCROLL_RANGE;

	m_xPageSize = (Pxmax - Pxorg) / x_scale;
	m_yPageSize = (Pymax - Pyorg) / y_scale;
	m_xLineSize = m_xPageSize / 10.0;
	m_yLineSize = m_yPageSize / 10.0;
	m_xScrollPos = fabs(Pxorg - xorg) / x_scale;
	if (UpsideDown_Flag == 0)
		m_yScrollPos = fabs(Pymax - ymax) / y_scale;
	else	// window-like
		m_yScrollPos = fabs(Pyorg - yorg) / y_scale;

	// scroll info setting
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = SCROLL_RANGE;
	si.nPage = round(m_xPageSize);
	si.nPos = round(m_xScrollPos);
	g_pView->SetScrollInfo(SB_HORZ, &si);

	si.nPage = round(m_yPageSize);
	si.nPos = round(m_yScrollPos);
	g_pView->SetScrollInfo(SB_VERT, &si);
}

void handleVScroll(UINT nSBCode, UINT nPos)
{
	double nDelta;
	int nMaxPos = round(SCROLL_RANGE - m_yPageSize);

	switch(nSBCode)
	{
	case SB_LINEUP:
		if (m_yScrollPos <= 0)
			return;
		nDelta = -min(m_yLineSize, m_yScrollPos);
		break;
	case SB_PAGEUP:
		if (m_yScrollPos <= 0)
			return;
		nDelta = -min(m_yPageSize, m_yScrollPos);
		break;
	case SB_THUMBPOSITION:
		nDelta = (int) nPos - m_yScrollPos;
		break;
	case SB_PAGEDOWN:
		if (m_yScrollPos >= nMaxPos)
			return;
		nDelta = min(m_yPageSize, nMaxPos - m_yScrollPos);
		break;
	case SB_LINEDOWN:
		if (m_yScrollPos >= nMaxPos)
			return;
		nDelta = min(m_yLineSize, nMaxPos - m_yScrollPos);
		break;
	default:	// 다른 이동 표시줄 메세지를 무시한다
		return;
	}

	m_yScrollPos += nDelta;
	g_pView->SetScrollPos(SB_VERT, round(m_yScrollPos));

	nDelta *= y_scale;
	if (UpsideDown_Flag == 1)
		nDelta = -nDelta;
	Pyorg -= nDelta;
	Pymax -= nDelta;
	Coeff_calc();

	g_pView->Invalidate();
}

void handleHScroll(UINT nSBCode, UINT nPos)
{
	double nDelta;
	int nMaxPos = round(SCROLL_RANGE - m_xPageSize);

	switch(nSBCode)
	{
	case SB_LINELEFT:
		if (m_xScrollPos <= 0)
			return;
		nDelta = min(m_xLineSize, m_xScrollPos);
		break;
	case SB_PAGELEFT:
		if (m_xScrollPos <= 0)
			return;
		nDelta = min(m_xPageSize, m_xScrollPos);
		break;
	case SB_THUMBPOSITION:
		nDelta = m_xScrollPos - (int)nPos;
		break;
	case SB_PAGERIGHT:
		if (m_xScrollPos >= nMaxPos)
			return;
		nDelta = -min(m_xPageSize, nMaxPos - m_xScrollPos);
		break;
	case SB_LINERIGHT:
		if (m_xScrollPos >= nMaxPos)
			return;
		nDelta = -min(m_xLineSize, nMaxPos - m_xScrollPos);
		break;
	default:	// 다른 이동 표시줄 메세지를 무시한다
		return;
	}
	m_xScrollPos -= nDelta;
	g_pView->SetScrollPos(SB_HORZ, round(m_xScrollPos));

	// physical area에 적용하기 위해 scale factor를 곱한다.
	nDelta *= x_scale;
	Pxorg -= nDelta;
	Pxmax -= nDelta;
	Coeff_calc();

	g_pView->Invalidate();
}

void handleSize(UINT nType, int cx, int cy) 
{
	if (g_pView->GetSafeHwnd())
	{
		Dxmax = cx;
		Dymax = cy;
		AdjustView();
		Coeff_calc();
	}
}
