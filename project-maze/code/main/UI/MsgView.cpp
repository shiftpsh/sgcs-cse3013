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
#include <afxrich.h>

static CRichEditView* g_pMsgView = NULL;
static int g_nMsgCnt = 1;

void initMessage(CRichEditView* pView)
{
	g_pMsgView = pView;
	CRichEditCtrl& rich = g_pMsgView->GetRichEditCtrl();
	rich.SetReadOnly();
	rich.SetBackgroundColor(FALSE, RGB(200, 200, 200));
	rich.SetWindowText("0 : Ready\n");
}

void showMessage(const char* pszMsg)
{
	CRichEditCtrl& rich = g_pMsgView->GetRichEditCtrl();
	CString str, tmp;
	int length;

	rich.GetWindowText(str);
	tmp.Format("%d : %s\n", g_nMsgCnt, pszMsg);
	str += tmp;
	length = str.GetLength();
	rich.SetWindowText(str);
	rich.SetSel(length, length);
	g_nMsgCnt++;
}

void clearMessage()
{
	CRichEditCtrl& rich = g_pMsgView->GetRichEditCtrl();
	rich.SetWindowText("0 : Ready\n");
	g_nMsgCnt = 1;
}