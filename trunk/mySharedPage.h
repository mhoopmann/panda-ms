#pragma once
#include "afxwin.h"


// mySharedPage dialog

class mySharedPage : public CDialog
{
	DECLARE_DYNAMIC(mySharedPage)

public:
	mySharedPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~mySharedPage();

// Dialog Data
	enum { IDD = IDD_SHAREDPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //MH:
  CString GetFile(CString filter);

	DECLARE_MESSAGE_MAP()
public:
	CString sharedSequence;
	CString sharedDbName;
	CString sharedAnalyzer;
	CString sharedMethod;
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton3();
  CString sharedStartDevice;
  CComboBox sharedStartDeviceCtrl;
  CComboBox sharedAnalyzerCtrl;
};
