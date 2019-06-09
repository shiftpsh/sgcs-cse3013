#include "StdAfx.h"

CDocument* g_pDoc = NULL;

void setDocument(CDocument* pDoc)
{
	ASSERT(pDoc);
	g_pDoc = pDoc;
}

/**************************************************************
 * function : void setModifiedFlag(bool modified)
 *
 * argument : bool modified - true if data are modified; otherwise false
 * remark   : call to check if data are modified
 **************************************************************/
void setModifiedFlag(bool modified)
{
	g_pDoc->SetModifiedFlag(modified);
}