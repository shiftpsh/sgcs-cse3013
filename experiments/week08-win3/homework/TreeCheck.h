
// TreeCheck.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTreeCheckApp:
// �� Ŭ������ ������ ���ؼ��� TreeCheck.cpp�� �����Ͻʽÿ�.
//

class CTreeCheckApp : public CWinApp
{
public:
	CTreeCheckApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CTreeCheckApp theApp;