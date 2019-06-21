// MFC_MainDoc.cpp : implementation of the CMFC_MainDoc class
//

#include "stdafx.h"
#include "MFC_Main.h"

#include "MFC_MainDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ExternView.h"
#include "ExternDoc.h"
#include "Graphics\DrawFunc.h"

extern bool readFile(const char* filename);
extern void freeMemory();
extern bool writeFile(const char* filename);
extern bool writeFile(const char* filename);
extern void DFS();
extern void BFS();

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainDoc

IMPLEMENT_DYNCREATE(CMFC_MainDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFC_MainDoc, CDocument)
	//{{AFX_MSG_MAP(CMFC_MainDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TOOLS_DFS, &CMFC_MainDoc::OnToolsDfs)
    ON_COMMAND(ID_TOOLS_BFS, &CMFC_MainDoc::OnToolsBfs)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_DFS, &CMFC_MainDoc::OnUpdateToolsDfs)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_BFS, &CMFC_MainDoc::OnUpdateToolsBfs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainDoc construction/destruction

CMFC_MainDoc::CMFC_MainDoc()
{
	// TODO: add one-time construction code here
	setDocument(this);
	m_bDataLoaded = false;
}

CMFC_MainDoc::~CMFC_MainDoc()
{
}

BOOL CMFC_MainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainDoc serialization

void CMFC_MainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainDoc diagnostics

#ifdef _DEBUG
void CMFC_MainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC_MainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFC_MainDoc commands

void CMFC_MainDoc::DeleteContents()
{
	// free memory for user data
	freeMemory();

	// set flags to default
	SetModifiedFlag(FALSE);
	m_bDataLoaded = false;
}

BOOL CMFC_MainDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// do not erase if you want to use MRU
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// read data file
	if (readFile(lpszPathName))
	{
		// if seccess, set flags and invalidate window
		m_bDataLoaded = true;
#ifdef GRAPHICS_BUFFERED
		g_pView->m_bRedraw = true;
#endif
		g_pView->Invalidate();
		return TRUE;
	}
	return FALSE;
}

BOOL CMFC_MainDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// write user data to a file
	return writeFile(lpszPathName);
}

void CMFC_MainDoc::OnFileClose() 
{
	// check if user data is modified
	if (!SaveModified())
		return;

	// delete user data
	DeleteContents();
	SetPathName("C:\\", FALSE);
	AfxGetApp()->m_pMainWnd->SetWindowText("제목없음 - MFC_Main");

	// erase client area
#if defined(GRAPHICS_BUFFERED)
	clearBuffer();
	showBuffer();
#elif defined(GRAPHICS_DIRECT)
	g_pView->Invalidate();
#endif
}

void CMFC_MainDoc::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	// if user data is loaded, enable close menu
	pCmdUI->Enable(m_bDataLoaded);
}

void CMFC_MainDoc::OnUpdateToolsDfs(CCmdUI* pCmdUI) {
    pCmdUI->Enable(m_bDataLoaded);
}


void CMFC_MainDoc::OnUpdateToolsBfs(CCmdUI* pCmdUI) {
    pCmdUI->Enable(m_bDataLoaded);
}


void CMFC_MainDoc::OnToolsDfs() {
    DFS();
#ifdef GRAPHICS_BUFFERED
    g_pView->m_bRedraw = true;
#endif // GRAPHICS_BUFFERED
    g_pView->Invalidate();
}


void CMFC_MainDoc::OnToolsBfs() {
    BFS();
#ifdef GRAPHICS_BUFFERED
    g_pView->m_bRedraw = true;
#endif // GRAPHICS_BUFFERED
    g_pView->Invalidate();
}
