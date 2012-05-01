// mySharedPage.cpp : implementation file
//

#include "stdafx.h"
#include "PAnDA2.h"
#include "mySharedPage.h"


// mySharedPage dialog

IMPLEMENT_DYNAMIC(mySharedPage, CDialog)

mySharedPage::mySharedPage(CWnd* pParent /*=NULL*/)
	: CDialog(mySharedPage::IDD, pParent)
	, sharedSequence(_T(""))
	, sharedDbName(_T(""))
	, sharedAnalyzer("")
	, sharedMethod(_T(""))
  , sharedStartDevice(_T(""))
{

}

mySharedPage::~mySharedPage()
{
}

void mySharedPage::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT2, sharedSequence);
  DDX_Text(pDX, IDC_EDIT14, sharedDbName);
  DDX_CBString(pDX, IDC_COMBO2, sharedAnalyzer);
  DDX_Text(pDX, IDC_EDIT13, sharedMethod);
  DDX_CBString(pDX, IDC_COMBO3, sharedStartDevice);
  DDX_Control(pDX, IDC_COMBO3, sharedStartDeviceCtrl);
  DDX_Control(pDX, IDC_COMBO2, sharedAnalyzerCtrl);
}


BEGIN_MESSAGE_MAP(mySharedPage, CDialog)
  ON_BN_CLICKED(IDC_BUTTON1, &mySharedPage::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON3, &mySharedPage::OnBnClickedButton3)
END_MESSAGE_MAP()


// mySharedPage message handlers

void mySharedPage::OnBnClickedButton1()
{
  sharedSequence=GetFile(_T("SLD Files (*.sld)|*.sld"));
  UpdateData(false);
}

void mySharedPage::OnBnClickedButton3()
{
  sharedMethod=GetFile(_T("METH Files (*.meth)|*.meth"));
  UpdateData(false);
}

CString mySharedPage::GetFile(CString filter){
  CString str;
  OPENFILENAME ofn;

	CFileDialog cFD(true,NULL,NULL,0,filter);
	if(cFD.DoModal()==IDOK){
    ofn=cFD.GetOFN();
    str=ofn.lpstrFile;
  } else {
    str=_T("");
  }
  return str;
}

