
// TreeCheckDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CTreeCheckDlg ��ȭ ����
class CTreeCheckDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CTreeCheckDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TREECHECK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnFileOpen();
	afx_msg void OnCheckTree();
	afx_msg void OnExit();
	CEdit m_Result;
};
