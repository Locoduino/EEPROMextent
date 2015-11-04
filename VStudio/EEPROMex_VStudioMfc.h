
// EEPROMex_VStudioMfc.h : main header file for the PROJECT_NAME application
//

#pragma once


#include "stdafx.h"		// main symbols
#include "resource.h"		// main symbols
#include "EEPROMex_VStudioMfcDlg.h"

// EEPROMex_VStudioMfcApp:
// See EEPROMex_VStudioMfc.cpp for the implementation of this class
//

class EEPROMex_VStudioMfcApp : public CWinApp
{
public:
	char lastKeyPressed;

	EEPROMex_VStudioMfcApp();

// Overrides
public:
	virtual BOOL InitInstance();
	EEPROMex_VStudioMfcDlg *GetDialog() { return (EEPROMex_VStudioMfcDlg *)this->m_pMainWnd; }
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern EEPROMex_VStudioMfcApp theApp;

void setup();
void loop();