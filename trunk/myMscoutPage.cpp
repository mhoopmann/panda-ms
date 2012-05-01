// myMscoutPage.cpp : implementation file
//

#include "stdafx.h"
#include "PAnDA2.h"
#include "myMscoutPage.h"


// myMscoutPage dialog

IMPLEMENT_DYNAMIC(myMscoutPage, CDialog)

myMscoutPage::myMscoutPage(CWnd* pParent /*=NULL*/)
	: CDialog(myMscoutPage::IDD, pParent)
	, singleIterations(0)
	, singleMSMSCoverage(0)
	, singleRTWin(0)
	, msRti(0)
	, msRtf(0)
	, msTime(0)
	, msMz(0)
	, msSn(0)
{

}

myMscoutPage::~myMscoutPage()
{
}

void myMscoutPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT10, singleIterations);
	DDX_Text(pDX, IDC_EDIT11, singleMSMSCoverage);
	DDX_Text(pDX, IDC_EDIT12, singleRTWin);
	DDX_Text(pDX, IDC_EDIT3, msRti);
	DDX_Text(pDX, IDC_EDIT4, msRtf);
	DDX_Text(pDX, IDC_EDIT5, msTime);
	DDX_Text(pDX, IDC_EDIT6, msMz);
	DDX_Text(pDX, IDC_EDIT7, msSn);
	DDX_Text(pDX, IDC_EDIT12, singleRTWin);
}


BEGIN_MESSAGE_MAP(myMscoutPage, CDialog)
END_MESSAGE_MAP()


// myMscoutPage message handlers
