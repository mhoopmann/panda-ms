#pragma once


// myJarPage dialog

class myJarPage : public CDialog
{
	DECLARE_DYNAMIC(myJarPage)

public:
	myJarPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~myJarPage();

// Dialog Data
	enum { IDD = IDD_JARPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
