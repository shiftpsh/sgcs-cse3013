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

#include "..\StdAfx.h"
#include "RubberBand.h"

// corner rect의 rows의 반
#define OFFSET			5

// global variable
CRect g_rcRB;
CRect g_rcPrevRB;
int g_nResizePos;					// true if band can be resized by dragging
int g_nDrawnRB;						// true if band is drawn

// static variable
static CWnd* m_pWnd;					// CWnd class in which the band will be drawn
static short m_nType;					// band type (1 : zoom(default), 2 : resizing)
static bool m_fDrawing;					// true if rubberband is drawing
static bool m_fResizing;				// true if resizing is enabled

// static functions
static void resizeRB(const POINT& pt);
static void drawCornerRect(CDC& dc);

// global functinos
bool initRB(CWnd* pWnd);
void clearRB();
int beginRB(const POINT& pt, int type);
int endRB(const POINT& pt);
void endResizableRB();
void redoRB();
void dragRB(const POINT& pt);
void drawRB();

bool initRB(CWnd* pWnd)
{
	if (NULL == pWnd)
		return false;
	m_pWnd = pWnd;
	m_nType = RBT_DEFAULT;
	m_fDrawing = m_fResizing = false;
	g_nResizePos = RB_POS_UKN;
	g_nDrawnRB = RB_NO_DRAWN;
	g_rcRB.left = g_rcRB.right = g_rcRB.top = g_rcRB.bottom = 0;
	return true;
}

void clearRB()
{
	m_nType = RBT_DEFAULT;
	m_fDrawing = m_fResizing = false;
	g_nResizePos = RB_POS_UKN;
	g_nDrawnRB = RB_NO_DRAWN;
	ReleaseCapture();
}

int beginRB(const POINT& pt, short type)
{
	CClientDC dc(m_pWnd);
	int nResult;

	m_nType = type;
	m_fDrawing = true;
	if (RBT_ZOOM == type && RB_IS_DRAWN == g_nDrawnRB)
	{
		// erase resizable band
		drawRB();
		// save band data
		g_nDrawnRB = RB_WAS_DRAWN;
		g_rcPrevRB = g_rcRB;
	}
	if (RB_IS_DRAWN == g_nDrawnRB)
		nResult = RBS_LOOP;
	else
	{
		// initialize
		m_fResizing = false;
		g_nResizePos = RB_POS_UKN;

		// draw rubber band
		g_rcRB.left = g_rcRB.right = pt.x;
		g_rcRB.top = g_rcRB.bottom = pt.y;
		dc.DrawFocusRect(g_rcRB);
		nResult = RBS_BEGIN;
	}
	m_pWnd->SetCapture();

	return nResult;
}

void dragRB(const POINT& pt)
{
	if (g_nResizePos && m_fDrawing)
	{
		resizeRB(pt);
		return;
	}
	if (m_fResizing && RB_IS_DRAWN == g_nDrawnRB)
	{
		if (pt.y >= g_rcRB.top - OFFSET && pt.y <= g_rcRB.top + OFFSET)
		{
			if (pt.x >= g_rcRB.left - OFFSET && pt.x <= g_rcRB.left + OFFSET)
				g_nResizePos = RB_POS_TL;
			else if (pt.x >= g_rcRB.right - OFFSET && pt.x <= g_rcRB.right + OFFSET)
				g_nResizePos = RB_POS_TR;
			else if (pt.x >= g_rcRB.left && pt.x <= g_rcRB.right)
				g_nResizePos = RB_POS_TE;
			else
				g_nResizePos = RB_POS_UKN;
		}
		else if (pt.y >= g_rcRB.bottom - OFFSET && pt.y <= g_rcRB.bottom + OFFSET)
		{
			if (pt.x >= g_rcRB.right - OFFSET && pt.x <= g_rcRB.right + OFFSET)
				g_nResizePos = RB_POS_BR;
			else if (pt.x >= g_rcRB.left - OFFSET && pt.x <= g_rcRB.left + OFFSET)
				g_nResizePos = RB_POS_BL;
			else if (pt.x >= g_rcRB.left && pt.x <= g_rcRB.right)
				g_nResizePos = RB_POS_BE;
			else
				g_nResizePos = RB_POS_UKN;
		}
		else if (pt.x >= g_rcRB.left - OFFSET && pt.x <= g_rcRB.left + OFFSET)
		{
			if (pt.y >= g_rcRB.top && pt.y <= g_rcRB.bottom)
				g_nResizePos = RB_POS_LE;
			else
				g_nResizePos = RB_POS_UKN;
		}
		else if (pt.x >= g_rcRB.right - OFFSET && pt.x <= g_rcRB.right + OFFSET)
		{
			if (pt.y >= g_rcRB.top && pt.y <= g_rcRB.bottom)
				g_nResizePos = RB_POS_RE;
			else
				g_nResizePos = RB_POS_UKN;
		}
		else
			g_nResizePos = RB_POS_UKN;
	}

	if (!m_fDrawing)
		return;

	// erase previous band
	drawRB();

	// 좌표 저장
//	CRect rect;
//	m_pWnd->GetClientRect(&rect);
//	g_rcRB.right = (pt.x < 0) ? 0 : (pt.x > rect.right) ? rect.right : pt.x;
//	g_rcRB.bottom = (pt.y < 0) ? 0 : (pt.y > rect.bottom) ? rect.bottom : pt.y;
	g_rcRB.right = pt.x;
	g_rcRB.bottom = pt.y;

	// resizing band를 그릴지 판단
	if (RBT_RESIZE == m_nType && (abs(g_rcRB.right - g_rcRB.left) > 20 || abs(g_rcRB.bottom - g_rcRB.top) > 20))
		m_fResizing = true;
	else
		m_fResizing = false;

	// draw new band
	drawRB();
}

int endRB(const POINT& pt)
// return 1 if zoom-in
//        2 if zoom-out
//        0 otherwise
{
	CClientDC dc(m_pWnd);
	int result = RBD_DEFAULT;

	ReleaseCapture();

	// drag가 일어나지 않은 경우
	if (!m_fDrawing)
		return result;
	m_fDrawing = false;

	if (RBT_RESIZE == m_nType && m_fResizing)
		g_nDrawnRB = RB_IS_DRAWN;
	else
		dc.DrawFocusRect(&g_rcRB);
	if (RBT_ZOOM == m_nType)
	{
		if (g_rcRB.left < g_rcRB.right && g_rcRB.top < g_rcRB.bottom)
			result = RBD_ZI;
		else if (g_rcRB.left > g_rcRB.right && g_rcRB.top > g_rcRB.bottom)
			result = RBD_ZO;
	}
	g_rcRB.NormalizeRect();

	return result;
}

void endResizableRB()
{
	drawRB();
	clearRB();
}

void redoRB()
{
	g_rcRB = g_rcPrevRB;
	g_nDrawnRB = RB_IS_DRAWN;
	m_fResizing = true;
	drawRB();
}

void drawRB()
{
	CClientDC dc(m_pWnd);
	CRect rc;

	rc = g_rcRB;
	if (RBT_ZOOM != m_nType || (RBT_ZOOM == m_nType && rc.left > rc.right && rc.top > rc.bottom))
		rc.NormalizeRect();
	dc.DrawFocusRect(&rc);
	if (m_fResizing)
		drawCornerRect(dc);
}

static void drawCornerRect(CDC& dc)
{
	int nOldMode = dc.SetROP2(R2_XORPEN);
	dc.Rectangle(g_rcRB.left - OFFSET, g_rcRB.top - OFFSET, g_rcRB.left + OFFSET, g_rcRB.top + OFFSET);
	dc.Rectangle(g_rcRB.left - OFFSET, g_rcRB.bottom - OFFSET, g_rcRB.left + OFFSET, g_rcRB.bottom + OFFSET);
	dc.Rectangle(g_rcRB.right - OFFSET, g_rcRB.top - OFFSET, g_rcRB.right + OFFSET, g_rcRB.top + OFFSET);
	dc.Rectangle(g_rcRB.right - OFFSET, g_rcRB.bottom - OFFSET, g_rcRB.right + OFFSET, g_rcRB.bottom + OFFSET);
	dc.SetROP2(nOldMode);
}

static void resizeRB(const POINT& pt)
{
	// erase previous band
	drawRB();

	// set band coordinate
	switch (g_nResizePos)
	{
	case RB_POS_TE:
		g_rcRB.top = pt.y;
		break;
	case RB_POS_BE:
		g_rcRB.bottom = pt.y;
		break;
	case RB_POS_LE:
		g_rcRB.left = pt.x;
		break;
	case RB_POS_RE:
		g_rcRB.right = pt.x;
		break;
	case RB_POS_UKN:
		break;
	default:
		if (g_nResizePos == RB_POS_TL || g_nResizePos == RB_POS_TR)
			g_rcRB.top = pt.y;
		else
			g_rcRB.bottom = pt.y;
		if (g_nResizePos == RB_POS_TL || g_nResizePos == RB_POS_BL)
			g_rcRB.left = pt.x;
		else
			g_rcRB.right = pt.x;
	}
	// draw new band
	drawRB();
}