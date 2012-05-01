// PAnDA2.h : main header file for the PAnDA2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CPAnDA2App:
// See PAnDA2.cpp for the implementation of this class
//

#define WM_SET_DEVICE (WM_USER + 4)

class CPAnDA2App : public CWinApp
{
public:
	CPAnDA2App();

  //MH functions
  void On_File_New();


// Overrides
public:
	virtual BOOL InitInstance();
  
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPAnDA2App theApp;