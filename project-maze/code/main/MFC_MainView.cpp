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

// MFC_MainView.cpp : implementation of the CMFC_MainView class
//

#include "stdafx.h"
#include "MFC_Main.h"

#include "MFC_MainDoc.h"
#include "MFC_MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ExternView.h"
#include "Graphics\DrawFunc.h"
#include "UI\RubberBand.h"
#include "UI\Scroll.h"
#include "UI\Zoom.h"

#define PRINTMARGIN			2
#define PRINT_OFFSET_RATIO	0.1

extern void drawMain(CDC* pDC);

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainView

IMPLEMENT_DYNCREATE(CMFC_MainView, CView)

BEGIN_MESSAGE_MAP(CMFC_MainView, CView)
	//{{AFX_MSG_MAP(CMFC_MainView)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, OnUpdateViewZoom)
	ON_COMMAND(ID_VIEW_FULLVIEW, OnViewFullview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLVIEW, OnUpdateViewFullview)
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_COORD, OnUpdateCoord)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainView construction/destruction

CMFC_MainView::CMFC_MainView()
{
	// TODO: add construction code here
	m_hZoomCursor = NULL;
	m_ptCursor.x = m_ptCursor.y = 0;
	m_bRedraw = m_bZoomMode = false;
}

CMFC_MainView::~CMFC_MainView()
{
	if (m_hZoomCursor)
		::DestroyCursor(m_hZoomCursor);
#ifdef GRAPHICS_BUFFERED
	// free graphic buffer
	freeBuffer();
#endif
}

BOOL CMFC_MainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainView drawing

void CMFC_MainView::OnDraw(CDC* pDC)
{
	CMFC_MainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_xScrRes = pDC->GetDeviceCaps(HORZRES);
	m_yScrRes = pDC->GetDeviceCaps(VERTRES);

	// if user data is loaded and redraw flag is set, draw something
#if defined (GRAPHICS_DIRECT)
	if (pDoc->m_bDataLoaded)
#elif defined (GRAPHICS_BUFFERED)
	if (pDoc->m_bDataLoaded && m_bRedraw)
#endif
	{
		// show wait cursor
		CWaitCursor wait;
//		pDC->SetMapMode(MM_LOMETRIC);
#ifdef GRAPHICS_BUFFERED
		// clear buffer before drawing something
		clearBuffer();
#endif

		// call main drawing function
		drawMain(pDC);

#ifdef GRAPHICS_BUFFERED
		// show buffer to screen
		showBuffer();
		// set redraw flag to false
		m_bRedraw = false;
#endif
	}

#ifdef GRAPHICS_BUFFERED
	// do not redraw, just show buffer to screen
	else
		refresh();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainView printing

BOOL CMFC_MainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFC_MainView::OnBeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	TEXTMETRIC tm;
	int height;

	// set variables for printing
	height = -((pDC->GetDeviceCaps(LOGPIXELSY) * 10) / 72);
	m_xPrinterOffset = int(pDC->GetDeviceCaps(HORZRES) / 2 * PRINT_OFFSET_RATIO);
	m_nPageWidth = pDC->GetDeviceCaps(HORZRES) - m_xPrinterOffset * 2;
	m_printerFont.CreateFont(height, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New");
	CFont* pOldFont = pDC->SelectObject(&m_printerFont);
	pDC->GetTextMetrics(&tm);
	m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
	m_nPageTop = m_cyPrinter * PRINTMARGIN + m_cyPrinter * 3;
	m_nPageHeight = pDC->GetDeviceCaps(VERTRES) - m_xPrinterOffset * 2 - m_nPageTop;
	pDC->SelectObject(pOldFont);
	
	// save viewport
	saveWindow();
}

void CMFC_MainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	m_printerFont.DeleteObject();
	restoreWindow();
#if defined(GRAPHICS_BUFFERED)
	m_bRedraw = true;
	g_pView->Invalidate();
#endif
}

void CMFC_MainView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	PrintPageHeader(pDC);
	int xRes, yRes;

	// change mapping mode
	m_xPrinterRes = xRes = pDC->GetDeviceCaps(HORZRES);
	m_yPrinterRes = yRes = pDC->GetDeviceCaps(VERTRES);
	if (abs(xRes - m_xScrRes) < abs(yRes - m_yScrRes))
	{
		yRes = int(m_yPrinterRes * m_xScrRes / double(m_xPrinterRes));
		xRes = m_xScrRes;
	}
	else
	{
		xRes = int(m_xPrinterRes * m_yScrRes / double(m_yPrinterRes));
		yRes = m_yScrRes;
	}
	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetWindowExt(xRes, yRes);
	pDC->SetViewportExt(m_xPrinterRes, m_yPrinterRes);

	Pxorg = Fxorg;
	Pyorg = Fyorg;
	Pxmax = Fxmax;
	Pymax = Fymax;
#if defined(GRAPHICS_DIRECT)
	Dxmax = xRes;
	Dymax = yRes;
#elif defined(GRAPHICS_BUFFERED)
	Dxmax = m_xScrRes;
	Dymax = m_yScrRes;
#endif
	AdjustView();
	Coeff_calc();

#if defined(GRAPHICS_BUFFERED)
	clearBuffer();
#endif
	drawMain(pDC);
#if defined(GRAPHICS_BUFFERED)
	printBuffer(pDC, int(xRes / 2 * PRINT_OFFSET_RATIO), int(m_cyPrinter * (PRINTMARGIN + 3) / pDC->GetDeviceCaps(VERTRES)),
			int(xRes * (1 - PRINT_OFFSET_RATIO)), int(yRes * (1 - PRINT_OFFSET_RATIO) - m_cyPrinter * (PRINTMARGIN + 3) / pDC->GetDeviceCaps(VERTRES)));
#endif

}

void CMFC_MainView::PrintPageHeader(CDC* pDC)
{
	CString header;
	CFont* pOldFont = pDC->SelectObject(&m_printerFont);
	UINT y = m_cyPrinter * PRINTMARGIN;
	header.Format("%s - %s", GetDocument()->GetPathName(), AfxGetAppName());
	pDC->TextOut(m_xPrinterOffset, y, header);
	y += (m_cyPrinter * 3) / 2;
	pDC->MoveTo(m_xPrinterOffset, y);
	pDC->LineTo(m_xPrinterOffset + m_nPageWidth, y);
	pDC->SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainView diagnostics

#ifdef _DEBUG
void CMFC_MainView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_MainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_MainDoc* CMFC_MainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_MainDoc)));
	return (CMFC_MainDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainView message handlers

BOOL CMFC_MainView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	if (CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext) == FALSE)
		return FALSE;

	// set global view pointer
	g_pView = this;
	
	// initialize rubber band
	if (initRB(this) == false)
		return FALSE;

#ifdef GRAPHICS_BUFFERED
	// initialize graphic buffer
	if (initBuffer() == false)
		return FALSE;
#endif

	// load zoom cursor
	m_hZoomCursor = AfxGetApp()->LoadCursor(IDC_ZOOM);
	// set default physical view
	setWindow(0, 0, 100, 100, 0);

	return TRUE;
}

BOOL CMFC_MainView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (HTCLIENT == nHitTest && m_bZoomMode)
	{
		// show zoom cursor
		::SetCursor(m_hZoomCursor);
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CMFC_MainView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	handleSize(nType, cx, cy);
#ifdef GRAPHICS_BUFFERED
	m_bRedraw = true;
#endif
}

void CMFC_MainView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	handleVScroll(nSBCode, nPos);
#ifdef GRAPHICS_BUFFERED
	m_bRedraw = true;
#endif
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMFC_MainView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	handleHScroll(nSBCode, nPos);
#ifdef GRAPHICS_BUFFERED
	m_bRedraw = true;
#endif
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMFC_MainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bZoomMode)
		beginRB(point, RBT_ZOOM);
	CView::OnLButtonDown(nFlags, point);
}

void CMFC_MainView::OnLButtonUp(UINT nFlags, CPoint point)
{
	int drag;

	if (m_bZoomMode)
	{
		drag = endRB(point);
		switch(drag)
		{
		case RBD_ZI:
			zoomIn(g_rcRB.left, g_rcRB.top, g_rcRB.right, g_rcRB.bottom);
			break;
		case RBD_ZO:
			zoomOut(g_rcRB.left, g_rcRB.top, g_rcRB.right, g_rcRB.bottom);
			break;
		}
		if (RBD_DEFAULT != drag)
		{
#ifdef GRAPHICS_BUFFERED
			m_bRedraw = true;
#endif
			Invalidate();
		}
	}
	CView::OnLButtonUp(nFlags, point);
}

void CMFC_MainView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bZoomMode)
		dragRB(point);
	m_ptCursor = point;

	CView::OnMouseMove(nFlags, point);
}

void CMFC_MainView::OnViewZoom() 
{
	if (GetDocument()->m_bDataLoaded)
		m_bZoomMode = !m_bZoomMode;
	else if (m_bZoomMode)
		m_bZoomMode = false;
}

void CMFC_MainView::OnUpdateViewZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_bDataLoaded);
	pCmdUI->SetCheck(m_bZoomMode);
}

void CMFC_MainView::OnUpdateCoord(CCmdUI* pCmdUI)
{
	CString str;
	double px, py;
	
	I_trans(m_ptCursor.x, m_ptCursor.y, px, py);
	str.Format("(x,y)=(%10.2f,%10.2f)", px, py);
	pCmdUI->SetText(str);
}

void CMFC_MainView::OnViewFullview() 
{
	viewFull();
#ifdef GRAPHICS_BUFFERED
	m_bRedraw = true;
#endif
	Invalidate();
}

void CMFC_MainView::OnUpdateViewFullview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_bDataLoaded);
}

BOOL CMFC_MainView::OnEraseBkgnd(CDC* pDC) 
{
#ifdef GRAPHICS_BUFFERED
	return FALSE;
#else
	return CView::OnEraseBkgnd(pDC);
#endif
}
