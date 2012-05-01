#pragma once
#include "afxwin.h"


// CLogView dialog

class CLogView : public CDialog
{
	DECLARE_DYNAMIC(CLogView)

public:
	CLogView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogView();

// Dialog Data
	enum { IDD = IDD_LOGVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  //MH: My variables
  CListBox logControl;

  //My functions
  void AddLine(CString str);
  void SetLogFile(char *str);

protected:
  //MH: My variables
  char logFile[256];
};
