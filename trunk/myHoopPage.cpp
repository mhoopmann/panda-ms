// myHoopPage.cpp : implementation file
//

#include "stdafx.h"
#include "PAnDA2.h"
#include "myHoopPage.h"


// myHoopPage dialog

IMPLEMENT_DYNAMIC(myHoopPage, CDialog)

myHoopPage::myHoopPage(CWnd* pParent /*=NULL*/)
	: CDialog(myHoopPage::IDD, pParent)
  , hkSN(FALSE)
  , singleIterations(2)
  , singleMSMSCoverage(1)
  , singleRTWin(0)
  , hkAnalyzer("FTICR")
  , hkResolution(50000)
  , hkWinSize(4.0)
  , hkMaxPeps(12)
  , hkMaxDepth(3)
  , hkChargeMin(1)
  , hkChargeMax(5)
  , hkCorr(0.90)
  , hkSNRatio(1.0)
  , hkSNWin(250)
{

}

myHoopPage::~myHoopPage()
{
}

void myHoopPage::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_CHECK1, hkSN);
  DDX_Text(pDX, IDC_EDIT10, singleIterations);
  DDX_Text(pDX, IDC_EDIT11, singleMSMSCoverage);
  DDX_Text(pDX, IDC_EDIT12, singleRTWin);
  DDX_Text(pDX, IDC_COMBO1, hkAnalyzer);
  DDX_Text(pDX, IDC_EDIT3, hkResolution);
  DDX_Text(pDX, IDC_EDIT4, hkWinSize);
  DDX_Text(pDX, IDC_EDIT5, hkMaxPeps);
  DDX_Text(pDX, IDC_EDIT6, hkMaxDepth);
  DDX_Text(pDX, IDC_EDIT8, hkChargeMin);
  DDX_Text(pDX, IDC_EDIT9, hkChargeMax);
  DDX_Text(pDX, IDC_EDIT7, hkCorr);
  DDX_Text(pDX, IDC_EDIT1, hkSNRatio);
  DDX_Text(pDX, IDC_EDIT2, hkSNWin);
}


BEGIN_MESSAGE_MAP(myHoopPage, CDialog)
END_MESSAGE_MAP()


// myHoopPage message handlers
