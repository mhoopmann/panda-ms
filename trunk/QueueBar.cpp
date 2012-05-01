// QueueBar.cpp : implementation file
//

#include "stdafx.h"
#include "PAnDA2.h"
#include "QueueBar.h"

// QueueBar dialog

IMPLEMENT_DYNAMIC(CQueueBar, CDialogBar)

CQueueBar::CQueueBar():activeQueueText(_T("")){}
CQueueBar::~CQueueBar(){}

void CQueueBar::DoDataExchange(CDataExchange* pDX)
{
  CDialogBar::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT1, activeQueueCtrl);
  DDX_Control(pDX, IDC_LIST1, queueCtrl);
  DDX_Control(pDX, IDC_BUTTON1, upButtonCtrl);
  DDX_Control(pDX, IDC_BUTTON2, downButtonCtrl);
  DDX_Control(pDX, IDC_BUTTON3, deleteButtonCtrl);
  DDX_Text(pDX, IDC_EDIT1, activeQueueText);
  DDX_Control(pDX, IDC_ACQUISITIONCTRL1, conAC);
}

LONG CQueueBar::OnInitDialog (UINT wParam, LONG lParam){
  BOOL bRet=HandleInitDialog(wParam,lParam);
  upButtonCtrl.EnableWindow();
  if(!UpdateData(FALSE)){
    TRACE0("Warning: UpdateData failed during QueueBar init.\n");
  }
  dll.SetMessagePump(mPump);
  dll.SetAcquisitionControl(&conAC);
  dll.LoadDll();
  bPandaRunning=false;
  return bRet;
}

BEGIN_MESSAGE_MAP(CQueueBar, CDialogBar)
  ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
  //These are active in MainFrm.cpp
  //ON_BN_CLICKED(IDC_BUTTON1, &CQueueBar::OnBnClickedButton1)
  //ON_BN_CLICKED(IDC_BUTTON2, &CQueueBar::OnBnClickedButton2)
  //ON_BN_CLICKED(IDC_BUTTON3, &CQueueBar::OnBnClickedButton3)
END_MESSAGE_MAP()


// QueueBar message handlers

void CQueueBar::OnBnClickedButton1()
{
  CString str;
  Parameters tmp;
  int index=queueCtrl.GetCurSel();
  if(index>0 && queueCtrl.GetCount()>1){
    queueCtrl.GetText(index-1,str);
    queueCtrl.DeleteString(index-1);
    queueCtrl.InsertString(index,str);

    tmp=queue[index-1];
    queue[index-1]=queue[index];
    queue[index]=tmp;
  }

}

void CQueueBar::OnBnClickedButton2()
{
  CString str;
  Parameters tmp;
  int index=queueCtrl.GetCurSel();
  if(index<queueCtrl.GetCount()-1 && queueCtrl.GetCount()>1){
    queueCtrl.GetText(index+1,str);
    queueCtrl.DeleteString(index+1);
    queueCtrl.InsertString(index,str);

    tmp=queue[index];
    queue[index]=queue[index+1];
    queue[index+1]=tmp;
  }
}

void CQueueBar::OnBnClickedButton3()
{
  int index=queueCtrl.GetCurSel();
  if(index!=LB_ERR && queueCtrl.GetCount()>0){
    queueCtrl.DeleteString(index);
    queue.erase(queue.begin()+index);
  }
}


bool CQueueBar::AddToQueue(Parameters &queueObj){
  char str[64];
  CString cstr;
  SharedParamList sharedParams;

  queueObj.GetSharedParams(sharedParams);
  cstr=sharedParams.MassAnalyzer;
  if(cstr.Find(_T("FT"))==-1 && cstr.Find(_T("Orbitrap"))==-1) {
    MessageBox(_T("Currently PAnDA requires an FT or Orbitrap mass analyzer."));
    return false;
  }

  queue.push_back(queueObj);

  //This is roundabout because I may add additional information to the queue
  sprintf(str,"%s",sharedParams.DatabaseName);
  cstr=_T("");
  cstr+=str;
  queueCtrl.AddString(cstr);

  return true;
}

void CQueueBar::StartQueue(){
  CString buff;
  if(queue.size()<1) {
    activeQueueText="";
    UpdateData(false);
    bPandaRunning=false;
    MessageBox(_T("Queue Finished!"));
    return;
  }
  int i=0;
  
  //set the text
  queueCtrl.GetText(0,buff);
  activeQueueText=buff;
  /*
  for(int j=0;j<buff.GetLength();j++) {
    activeQueueText+=buff[j];
  }
  */
  
  //activeQueueText+=buff;

  //Get the type of analysis from the params
  activeQueue=queue.at(0);
  queue.erase(queue.begin());

  //MessageBox(activeQueueText);
  queueCtrl.SetCurSel(-1);
  queueCtrl.DeleteString(0);
  
  UpdateData(false);
  //MessageBox(activeQueueText);

  //switch(activeQueue.GetAnalysisType()){
  switch(i){
  case 0: //Single Analysis
    singleAnalysis.SetDLL(&dll);
    singleAnalysis.SetMessagePump(mPump);
    singleAnalysis.SetParams(activeQueue);
    singleAnalysis.Setup();
    singleAnalysis.SubmitSequence();
    //activeThread = AfxBeginThread(singleAnalysis.ThreadProc,GetParent());
    break;
  case 1:
  case 2:
  default:
    break;
  }

  bPandaRunning=true;

}

void CQueueBar::SetCWD(TCHAR *tc){
  //Set cwd for all objects here
  singleAnalysis.SetCWD(tc);
}

void CQueueBar::SetMessagePump(cMessagePump *mp){
  mPump=mp;
}

BEGIN_EVENTSINK_MAP(CQueueBar, CDialogBar)
ON_EVENT(CQueueBar, IDC_ACQUISITIONCTRL1, 8, CQueueBar::RunEndedAcquisitionctrl1, VTS_NONE)
ON_EVENT(CQueueBar, IDC_ACQUISITIONCTRL1, 25, CQueueBar::ProgramErrorAcquisitionctrl1, VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)
ON_EVENT(CQueueBar, IDC_ACQUISITIONCTRL1, 1, CQueueBar::SequenceChangeAcquisitionctrl1, VTS_NONE)
END_EVENTSINK_MAP()

void CQueueBar::RunEndedAcquisitionctrl1()
{
  if(!bPandaRunning) return;
  //activeThread = AfxBeginThread(singleAnalysis.ThreadProc,GetParent());
  //activeThread = AfxBeginThread(singleAnalysis.ThreadProc,&singleAnalysis);
  singleAnalysis.StartThread();
  CString sMessage = _T("Thread started.");
  mPump->SendMessage(sMessage);

  while(!singleAnalysis.runEnded)  DoEvents();
  CheckStatusCurrentQueue();
  
}

void CQueueBar::ProgramErrorAcquisitionctrl1(BOOL InstrumentError, long Flags, long ErrorCode, long VirDeviceID, long MsgTime, LPCTSTR Message, LPCTSTR Title)
{
  if(!bPandaRunning) return;
  CString sMessage = _T("");
	sMessage.Format(_T("Program Error!\n\nInstrumentError: %s\nFlags: %ld\nErrorCode: %ld\nVirDeviceID: %ld\nMsgTime: %ld\nMessage: %s"), 
		InstrumentError ? _T("TRUE") : _T("FALSE"), Flags, ErrorCode, VirDeviceID, MsgTime, Message);

  mPump->SendMessage(sMessage);
}

void CQueueBar::SequenceChangeAcquisitionctrl1()
{
  if(!bPandaRunning) return;
  CString sMessage = _T("Sequence changed.");
  mPump->SendMessage(sMessage);
}

bool CQueueBar::CheckStatusCurrentQueue(){
  if(singleAnalysis.NextIteration()){
    singleAnalysis.EditMethod();
    singleAnalysis.EditSequence();  
    singleAnalysis.SubmitSequence();
    return true;
  }
  return false;
}

bool CQueueBar::CheckQueueItem(char *itm){
  int i;
  SharedParamList sharedParams;

  for(i=0;i<queue.size();i++){
    queue[i].GetSharedParams(sharedParams);
    if(strcmp(sharedParams.DatabaseName,itm)==0) return true;
  }

  activeQueue.GetSharedParams(sharedParams);
  if(strcmp(sharedParams.DatabaseName,itm)==0) return true;

  return false;
}

void CQueueBar::DoEvents() {
	MSG msg;
  long sts;

 	do {
   	if (sts = PeekMessage(&msg, (HWND) NULL, 0, 0, PM_REMOVE)) {
 			TranslateMessage(&msg);
 			DispatchMessage(&msg);
		}
 	} while (sts);
}