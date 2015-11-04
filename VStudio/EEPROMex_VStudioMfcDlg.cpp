
// EEPROMex_VStudioMfcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EEPROMex_VStudioMfc.h"
#include "EEPROMex_VStudioMfcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

EEPROMex_VStudioMfcDlg *EEPROMex_VStudioMfcDlg::pMainDialog;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL loopStarted;

/////////////////////////////////////////////////////////////////
// EEPROMex_VStudioMfcDlg dialog

EEPROMex_VStudioMfcDlg::EEPROMex_VStudioMfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(EEPROMex_VStudioMfcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (int i = 0; i < DEBUGLINES; i++)
		debugs[i][0] = 0;

	EEPROMex_VStudioMfcDlg::pMainDialog = this;
}

void EEPROMex_VStudioMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(EEPROMex_VStudioMfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDSTART, &EEPROMex_VStudioMfcDlg::OnBnClickedStart)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// EEPROMex_VStudioMfcDlg message handlers

BOOL EEPROMex_VStudioMfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Creates a 16-point-Courier-font
	font.CreatePointFont(200, _T("Fixedsys"));

	// Without a member variable
	GetDlgItem(IDC_LINES1)->SetFont(&font);
	GetDlgItem(IDC_LINES2)->SetFont(&font);
	GetDlgItem(IDC_LINES3)->SetFont(&font);
	GetDlgItem(IDC_LINES4)->SetFont(&font);
	GetDlgItem(IDC_LINES5)->SetFont(&font);

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	loopStarted = false;
	UINT_PTR myTimer = SetTimer(123, 100, 0); // one event every 100 ms = 0.1 s

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void EEPROMex_VStudioMfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void EEPROMex_VStudioMfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR EEPROMex_VStudioMfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void EEPROMex_VStudioMfcDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (loopStarted == TRUE)
		loop();
}

void EEPROMex_VStudioMfcDlg::OnBnClickedStart()
{
	loopStarted = true;
	setup();
}

void EEPROMex_VStudioMfcDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Arduino access methods

void EEPROMex_VStudioMfcDlg::Setup(byte inSizex, byte inSizey)
{
	GetDlgItem(IDC_LINES1)->SetWindowTextW(0);
	GetDlgItem(IDC_LINES2)->SetWindowTextW(0);
	GetDlgItem(IDC_LINES3)->SetWindowTextW(0);
	GetDlgItem(IDC_LINES4)->SetWindowTextW(0);
	GetDlgItem(IDC_LINES5)->SetWindowTextW(0);
}

void EEPROMex_VStudioMfcDlg::clear()
{
	GetDlgItem(IDC_LINES1)->SetWindowTextW(0);
	GetDlgItem(IDC_LINES2)->SetWindowTextW(0);
	GetDlgItem(IDC_LINES3)->SetWindowTextW(0);
	GetDlgItem(IDC_LINES4)->SetWindowTextW(0);
	GetDlgItem(IDC_LINES5)->SetWindowTextW(0);
}

void EEPROMex_VStudioMfcDlg::print(byte inLine, const char *inString)
{
	CString str(inString);

	if (inLine == 0)
		GetDlgItem(IDC_LINES1)->SetWindowTextW((LPCTSTR)str);
	if (inLine == 1)
		GetDlgItem(IDC_LINES2)->SetWindowTextW((LPCTSTR)str);
	if (inLine == 2)
		GetDlgItem(IDC_LINES3)->SetWindowTextW((LPCTSTR)str);
	if (inLine == 3)
		GetDlgItem(IDC_LINES4)->SetWindowTextW((LPCTSTR)str);
	if (inLine == 4)
		GetDlgItem(IDC_LINES5)->SetWindowTextW((LPCTSTR)str);
}

void EEPROMex_VStudioMfcDlg::getLine(byte inLine, char *inBuffer, int inLength)
{
	if (inLine == 0)
		::GetWindowTextA(GetDlgItem(IDC_LINES1)->m_hWnd, inBuffer, inLength);
	if (inLine == 1)
		::GetWindowTextA(GetDlgItem(IDC_LINES2)->m_hWnd, inBuffer, inLength);
	if (inLine == 2)
		::GetWindowTextA(GetDlgItem(IDC_LINES3)->m_hWnd, inBuffer, inLength);
	if (inLine == 3)
		::GetWindowTextA(GetDlgItem(IDC_LINES4)->m_hWnd, inBuffer, inLength);
	if (inLine == 4)
		::GetWindowTextA(GetDlgItem(IDC_LINES5)->m_hWnd, inBuffer, inLength);
}

void EEPROMex_VStudioMfcDlg::debug(const char *inString)
{
	for (int i = 0; i < DEBUGLINES-1; i++)
	{
		debugs[i][0] = 0;
		strcpy(debugs[i], debugs[i + 1]);
	}
	debugs[DEBUGLINES-1][0] = 0;
	strcpy(debugs[DEBUGLINES-1], inString);

	char str[DEBUGLINES * 200];
	memset(str, 0, DEBUGLINES * 200);

	str[0] = 0;
	for (int i = 0; i < DEBUGLINES; i++)
	{
		strcat(str, debugs[i]);
		if (i < DEBUGLINES-1)
			strcat(str, "\r\n");
	}

	CString cstr(str);
	GetDlgItem(IDC_EDITDEBUG)->SetWindowTextW((LPCTSTR)cstr);
}
