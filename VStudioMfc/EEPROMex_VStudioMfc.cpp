// EEPROMex_VStudioMfc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "conio.h"
#include "EEPROMex_VStudioMfc.h"
#include "EEPROMex_VStudioMfcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void setup();
void loop();

// EEPROMex_VStudioMfcApp

BEGIN_MESSAGE_MAP(EEPROMex_VStudioMfcApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CEEPROMex_VStudioMfcApp construction

EEPROMex_VStudioMfcApp::EEPROMex_VStudioMfcApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CEEPROMex_VStudioMfcApp object

EEPROMex_VStudioMfcApp theApp;


// CEEPROMex_VStudioMfcApp initialization

BOOL EEPROMex_VStudioMfcApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	EEPROMex_VStudioMfcDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	lastKeyPressed = 0;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL EEPROMex_VStudioMfcApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	// Check to make sure CPenWidthsDlg is up
	if (m_pMainWnd->m_hWnd != NULL)
	{
		if ((lpMsg->hwnd == m_pMainWnd->m_hWnd) ||
			::IsChild(m_pMainWnd->m_hWnd, lpMsg->hwnd))
			// Use ::IsChild to get messages that may be going
			// to the dialog's controls.  In the case of
			// WM_KEYDOWN this is required.
		{
			if (lpMsg->message == WM_CHAR)
				lastKeyPressed = lpMsg->wParam;
		}
	}
	// Default processing of the message.
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}

//-----------------------------------------
//			ino PART
//-----------------------------------------
#include "EEPROMextent.h"
#include "Serial.hpp"

struct Point
{
	bool exists;
	int number;
	unsigned long delay;
};

struct child
{
	byte id;
	char descr[10];
	int dummy;
};

struct parent
{
	byte id;
	int number;
	char text[10];
};

CircularBuffer cb;

EEPROM_ItemListClass ItemList;

byte save_parent(parent &aParent)
{
	byte p1_place = ItemList.GetFirstFreeSlot();
	// Save the standard prefix at first free place (p1_place).
	// 'P' because this item is a parent.
	// 255 means this item have no parent itself. So 254 is the maximum number of items which can be saved by this library!
	int pos = ItemList.SaveItemPrefix(p1_place, 'P', 255);
	// Then write the parent content (do not forget to update the position 'pos' for each field...):
	EEPROMextent.write(pos++, aParent.id);
	pos += EEPROMextent.writeAnything(pos, aParent.number);
	EEPROMextent.writeString(pos, aParent.text);
	pos += 10;

	if (pos - ItemList.GetItemPosRaw(p1_place) > ItemList.ItemSize)
	{
		Serial.print("Too long parent record : increase the item size to ");
		Serial.print(pos - ItemList.GetItemPosRaw(p1_place));
		Serial.println(" minimum !");
	}

	return p1_place;
}

byte save_child(child &aChild, byte aParent)
{
	byte c1_place = ItemList.GetFirstFreeSlot();
	// Save the standard prefix at first free place (c1_place).
	// 'C' because this item is a child.
	// p1_place means that the parent of this item is the one at p1_place.
	int pos = ItemList.SaveItemPrefix(c1_place, 'C', aParent);
	// Then write the child content (do not forget to update the position 'pos' for each field...):
	EEPROMextent.write(pos++, aChild.id);
	EEPROMextent.writeString(pos, aChild.descr);
	pos += 10; // maximum size of descr string as in its declaration.
	pos += EEPROMextent.writeAnything(pos, aChild.dummy);

	if (pos - ItemList.GetItemPosRaw(c1_place) > ItemList.ItemSize)
	{
		Serial.print("Too long child record : increase the item size to ");
		Serial.print(pos - ItemList.GetItemPosRaw(c1_place));
		Serial.println(" minimum !");
	}

	return c1_place;
}

void setup()
{
	// Clear the data area.
	EEPROMextent.clear(0, 100);

	// Writing data...
	Point p1;

	p1.delay = 12345;
	p1.exists = true;
	p1.number = 789;

	EEPROMextent.write(0, 10);
	EEPROMextent.write(1, 11);

	int size = EEPROMextent.writeAnything(10, p1);
	EEPROMextent.writeString(50, "Bonjour la France");

	// Reading the writen data.
	Point p2;
	char text[100];

	byte read0 = EEPROMextent.read(0);
	byte read1 = EEPROMextent.read(1);

	EEPROMextent.readAnything(10, p2);
	EEPROMextent.readString(50, text, 100);

	Serial.print("Should be 10 : ");
	Serial.println(read0);

	Serial.print("Should be 11 : ");
	Serial.println(read1);

	Serial.print("Should be 12345 : ");
	Serial.println(p2.delay);

	Serial.print("Should be 1 : ");
	Serial.println(p2.exists);

	Serial.print("Should be 789 : ");
	Serial.println(p2.number);

	Serial.print("Should be \"Bonjour la Fance\" : ");
	Serial.println(text);

	// Estimated size of 15 bytes for each parent and each child...
	// Arbitrary start position for writing in EEPROM.
	// Arbitrary EEPROM size, must be adapted according to your Arduino model.
	ItemList.Setup(10, 20, 1024);

	parent pa1;
	pa1.id = 0;
	pa1.number = 123;
	strcpy(pa1.text, "Bonjour");

	child c1;
	c1.id = 100;
	c1.dummy = 123;
	strcpy(c1.descr, "Child1");

	child c2;
	c2.id = 101;
	c2.dummy = 456;
	strcpy(c2.descr, "Child2");

	parent pa2;
	pa2.id = 1;
	pa2.number = 78;
	strcpy(pa2.text, "la France");

	child c3;
	c3.id = 102;
	c3.dummy = 89;
	strcpy(c3.descr, "Child3");

	child c4;
	c4.id = 103;
	c4.dummy = 1000;
	strcpy(c4.descr, "Child4");

	child c5;
	c5.id = 104;
	c5.dummy = 1001;
	strcpy(c5.descr, "Child5");

	// Clear the data area.
	ItemList.clear();

	// Writing data...
	// save the parent p1
	byte p1_place = save_parent(pa1);

	// save the first child c1
	save_child(c1, p1_place);

	// save the second child c2
	save_child(c2, p1_place);

	// save the parent p2
	byte p2_place = save_parent(pa2);

	// save the first child c2
	save_child(c3, p2_place);

	// save the second child c3
	save_child(c4, p2_place);

	// save the third child c5
	save_child(c5, p2_place);

	// Map of the items in EEPROM:
	// 
	// slots	0  1  2	 3	4  5  6   7(first free)
	// items	P1 C1 C2 P2 C3 C4 C5

	Serial.print("First free should be 7 : ");
	Serial.println(ItemList.GetFirstFreeSlot());

	Serial.print("Number of parents should be 2 : ");
	Serial.println(ItemList.CountItems('P'));

	Serial.print("Number of children should be 5 : ");
	Serial.println(ItemList.CountItems('C'));

	Serial.print("Number of children of p1 should be 2 : ");
	Serial.println(ItemList.CountOwnedItems(p1_place));

	Serial.print("Number of children of p2 should be 3 : ");
	Serial.println(ItemList.CountOwnedItems(p2_place));

	// Writing data...
	p1.delay = 12345;
	p1.exists = true;
	p1.number = 789;

	int totalsize = cb.Setup(10, sizeof(Point), 4);
	Serial.print("Total size of the buffer : ");
	Serial.println(totalsize);

	//cb.Clear();

	cb.Write(&p1);
	cb.printStatus();
	cb.Read(&p2);

	if (p2.number != 789 || p2.delay != 12345 || p2.exists != true)
		Serial.println("CB ERROR !!");

	p2.delay++;
	p2.exists = false;
	p2.number++;

	cb.Write(&p2);
	cb.printStatus();
	cb.Read(&p1);

	if (p1.number != 790 || p1.delay != 12346 || p1.exists != false)
		Serial.println("CB ERROR !!");
}

void loop()
{
}
