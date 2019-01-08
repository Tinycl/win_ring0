
// wring0Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// Cwring0Dlg dialog
class Cwring0Dlg : public CDialogEx
{
// Construction
public:
	Cwring0Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WRING0_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	BOOL m_IsNT;
	void Refresh();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_Edit;
};
