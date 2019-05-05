
// WaterfallView.cpp : CWaterfallView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Waterfall.h"
#endif

#include "WaterfallDoc.h"
#include "WaterfallView.h"

#include "waterfallsolver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaterfallView

IMPLEMENT_DYNCREATE(CWaterfallView, CView)

BEGIN_MESSAGE_MAP(CWaterfallView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWaterfallView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CWaterfallView ����/�Ҹ�

CWaterfallView::CWaterfallView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CWaterfallView::~CWaterfallView()
{
}

BOOL CWaterfallView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CWaterfallView �׸���

void CWaterfallView::OnDraw(CDC* pDC)
{
	CWaterfallDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: add draw code for native data here
	/* ����� �Է��Ұ� ���� */
	drawBackground(pDC); // F4
	drawStartPoint(pDC); // F5
	drawWaterflow(pDC);  // F6
	/* ����� �Է��Ұ� ��   */
}


// CWaterfallView �μ�


void CWaterfallView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWaterfallView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CWaterfallView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CWaterfallView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CWaterfallView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWaterfallView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWaterfallView ����

#ifdef _DEBUG
void CWaterfallView::AssertValid() const
{
	CView::AssertValid();
}

void CWaterfallView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWaterfallDoc* CWaterfallView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaterfallDoc)));
	return (CWaterfallDoc*)m_pDocument;
}
#endif //_DEBUG


// CWaterfallView �޽��� ó����
