// LogView.cpp : implementation file
//

#include "stdafx.h"
#include "PAnDA2.h"
#include "LogView.h"


// CLogView dialog

IMPLEMENT_DYNAMIC(CLogView, CDialog)

CLogView::CLogView(CWnd* pParent /*=NULL*/)
	: CDialog(CLogView::IDD, pParent)
{
  strcpy(logFile,"");
}

CLogView::~CLogView()
{
}

void CLogView::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST1, logControl);
}

void CLogView::AddLine(CString str){
  USES_CONVERSION;
  int i=logControl.AddString(str);
  logControl.SetCurSel(i);

  //Output log to file
  if(strlen(logFile)>0){
    FILE *f=fopen(logFile,"at");
    char *tmp=T2A(str.GetBuffer());
    fprintf(f,"%s\n",tmp);
    fclose(f);
  }
}

void CLogView::SetLogFile(char *str){
  strcpy(logFile,str);
}


BEGIN_MESSAGE_MAP(CLogView, CDialog)
END_MESSAGE_MAP()


// CLogView message handlers
