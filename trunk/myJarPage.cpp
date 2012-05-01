// myJarPage.cpp : implementation file
//

#include "stdafx.h"
#include "PAnDA2.h"
#include "myJarPage.h"


// myJarPage dialog

IMPLEMENT_DYNAMIC(myJarPage, CDialog)

myJarPage::myJarPage(CWnd* pParent /*=NULL*/)
	: CDialog(myJarPage::IDD, pParent)
{

}

myJarPage::~myJarPage()
{
}

void myJarPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(myJarPage, CDialog)
END_MESSAGE_MAP()


// myJarPage message handlers
