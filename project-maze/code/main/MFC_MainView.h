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

// MFC_MainView.h : interface of the CMFC_MainView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFC_MAINVIEW_H__E415686D_7D9E_11D4_8F2D_006097DCA644__INCLUDED_)
#define AFX_MFC_MAINVIEW_H__E415686D_7D9E_11D4_8F2D_006097DCA644__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMFC_MainView : public CView
{
protected: // create from serialization only
	CMFC_MainView();
	DECLARE_DYNCREATE(CMFC_MainView)

// Attributes
public:
	CMFC_MainDoc* GetDocument();
	CPoint m_ptCursor;
	bool m_bRedraw;
	bool m_bZoomMode;
protected:
	HCURSOR m_hZoomCursor;
	CFont m_printerFont;
	int m_cyPrinter;
	int m_nPageWidth;
	int m_nPageHeight;
	int m_nPageTop;
	int m_xPrinterOffset;

	int m_xScrRes;
	int m_yScrRes;
	int m_xPrinterRes;
	int m_yPrinterRes;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFC_MainView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFC_MainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void PrintPageHeader(CDC* pDC);

// Generated message map functions
protected:
	//{{AFX_MSG(CMFC_MainView)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateViewZoom(CCmdUI* pCmdUI);
	afx_msg void OnViewFullview();
	afx_msg void OnUpdateViewFullview(CCmdUI* pCmdUI);
	afx_msg void OnViewZoom();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnUpdateCoord(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MFC_MainView.cpp
inline CMFC_MainDoc* CMFC_MainView::GetDocument()
   { return (CMFC_MainDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_MAINVIEW_H__E415686D_7D9E_11D4_8F2D_006097DCA644__INCLUDED_)
