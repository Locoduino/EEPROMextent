
// EEPROMex_VStudioMfcDlg.h : header file
//

#pragma once

#include "stdafx.h"		// main symbols
#include "resource.h"		// main symbols

#define DEBUGLINES	10

// EEPROMex_VStudioMfcDlg dialog
class EEPROMex_VStudioMfcDlg : public CDialogEx
{
// Construction
public:
	EEPROMex_VStudioMfcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EEPROMex_VSTUDIOMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CFont font;
	char debugs[DEBUGLINES][200];
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void Setup(byte inSizex, byte inSizey);
	void clear();
	void print(byte inLine, const char *inString);
	void getLine(byte inLine, char *inBuffer, int inLength);
	void debug(const char *inString);
	afx_msg void OnBnClickedStart();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	static EEPROMex_VStudioMfcDlg *pMainDialog;
};
