// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "PAnDA2.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

#define WM_SINGLE_DONE (WM_USER + 5)
#define WM_LOG_THIS (WM_USER + 2)
#define WM_LOG_FILE (WM_USER + 3)
#define WM_SET_DEVICE (WM_USER + 4)
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
  ON_BN_CLICKED(IDC_BUTTON1, &CMainFrame::OnQueueUp)
  ON_BN_CLICKED(IDC_BUTTON2, &CMainFrame::OnQueueDown)
  ON_BN_CLICKED(IDC_BUTTON3, &CMainFrame::OnQueueDelete)
  ON_COMMAND(ID_VIEW_LOGWINDOW, &CMainFrame::OnLogWindow)
  ON_BN_CLICKED(ID_QUEUE_SUBMIT, &CMainFrame::SubmitToQueue)
  ON_BN_CLICKED(ID_QUEUE_START, &CMainFrame::StartQueue)
  ON_MESSAGE(WM_SINGLE_DONE, &CMainFrame::SingleDone)
  ON_MESSAGE(WM_LOG_THIS, &CMainFrame::AddToLog)
  ON_MESSAGE(WM_LOG_FILE, &CMainFrame::ChangeLogFile)
  ON_MESSAGE(WM_SET_DEVICE, &CMainFrame::OnFileNew)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LOGWINDOW,&CMainFrame::OnClickLogView)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

/*
BOOL CMainFrame::OnEraseBkgnd(CDC *pDC){
  m_wndMDIClient.OnEraseBkgnd(pDC);
}
*/


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

  m_wndMDIClient.SubclassWindow(m_hWndMDIClient);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

  //MH:
  if(!m_wndLogView.Create(IDD_LOGVIEW,this)){
    TRACE0("Failed to create log window\n");
		return -1;      // fail to create
	}
  bLogWindow=true;

  mPump.SetParent(this);
  m_wndQueueBar.SetMessagePump(&mPump);
  if(!m_wndQueueBar.Create(this,IDD_QUEUEBAR,CBRS_LEFT,IDD_QUEUEBAR)) {
    TRACE0("Failed to create queue bar\n");
		return -1;      // fail to create
	}

  m_wndLogView.ShowWindow(SW_SHOW);

  //Should be current working directory of PAnDA application
  ::GetCurrentDirectory(sizeof(cwd) - 1, cwd);
  m_wndQueueBar.SetCWD(cwd);
  
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnLogWindow(){
  if(m_wndLogView.IsWindowVisible()) m_wndLogView.ShowWindow(SW_HIDE);
  else m_wndLogView.ShowWindow(SW_SHOW);
}

void CMainFrame::OnClickLogView(CCmdUI *pCmdUI){
  pCmdUI->SetCheck(m_wndLogView.IsWindowVisible());
  //pCmdUI->SetCheck(bLogWindow);
  //pCmdUI->Enable(true);
}

void CMainFrame::OnQueueUp(){
  m_wndQueueBar.OnBnClickedButton1();
}

void CMainFrame::OnQueueDown(){
  m_wndQueueBar.OnBnClickedButton2();
}

void CMainFrame::OnQueueDelete(){
  m_wndQueueBar.OnBnClickedButton3();
}

void CMainFrame::SubmitToQueue(){
  Parameters QueueObj;
  SharedParamList sharedParams;
  SingleParamList singleParams;
  CPAnDA2Doc *activeDoc;
  bool bParamsOK=true;

  //Get the Parameters
  activeDoc=(CPAnDA2Doc *)(this->MDIGetActive()->GetActiveDocument());
  if(activeDoc==NULL) MessageBox(_T("No Active Document Available"));
  activeDoc->UpdateParams();
  QueueObj=activeDoc->m_parameters;

  //Check the parameters for validity (all required fields have values).
  QueueObj.GetSharedParams(sharedParams);
  QueueObj.GetSingleParams(singleParams);
  if(strcmp(sharedParams.DatabaseName,"")==0) bParamsOK=false;
  if(strcmp(sharedParams.MassAnalyzer,"")==0) bParamsOK=false;
  if(strcmp(sharedParams.StartDevice,"")==0) bParamsOK=false;
  if(strcmp(sharedParams.MethodTemplate,"")==0) bParamsOK=false;
  if(strcmp(sharedParams.SequenceTemplate,"")==0) bParamsOK=false;
  if(singleParams.singleIterations<2) bParamsOK=false;
  if(singleParams.singleMSMSCoverage<1) bParamsOK=false;
  
  if(!bParamsOK) {
    MessageBox(_T("There is an error with your parameters. Please make sure all fields are filled in with appropriate values."));
    return;
  }

  //Check to see if object is already in the Queue
  if(m_wndQueueBar.CheckQueueItem(sharedParams.DatabaseName)){
    MessageBox(_T("A job of the same name has already been submitted to the queue. Please provide a new database name."));
    return;
  }

  m_wndQueueBar.AddToQueue(QueueObj);
  activeDoc=NULL;
}

void CMainFrame::StartQueue(){
  m_wndQueueBar.StartQueue();
}

LRESULT CMainFrame::SingleDone(WPARAM wParam,LPARAM lParam){
  if(!m_wndQueueBar.CheckStatusCurrentQueue()) m_wndQueueBar.StartQueue();
  return 0L;
}

LRESULT CMainFrame::AddToLog(WPARAM wParam,LPARAM lParam){
  m_wndLogView.AddLine(mPump.ReceiveMessage());
  return 0L;
}

LRESULT CMainFrame::ChangeLogFile(WPARAM wParam,LPARAM lParam){
  USES_CONVERSION;
  char *str;
  str=T2A(mPump.ReceiveMessage().GetBuffer());
  m_wndLogView.SetLogFile(str);
  return 0L;
}

LRESULT CMainFrame::OnFileNew(WPARAM wParam,LPARAM lParam){
  CPAnDA2View *activeView;
  int i;

  //Get the Parameters
  activeView=(CPAnDA2View *)(this->MDIGetActive()->GetActiveView());
  if(activeView==NULL) MessageBox(_T("WTF?"));
  activeView->m_SharedPage.sharedAnalyzerCtrl.Clear();
  for(i=0;i<m_wndQueueBar.dll.vDevices.size();i++){
    if(m_wndQueueBar.dll.vDevices[i].Find(_T("LTQ"))>-1){
      activeView->m_SharedPage.sharedAnalyzerCtrl.AddString(m_wndQueueBar.dll.vDevices[i]);
    }
  }
  activeView->m_SharedPage.sharedStartDeviceCtrl.Clear();
  for(i=0;i<m_wndQueueBar.dll.vDevices.size();i++){
    activeView->m_SharedPage.sharedStartDeviceCtrl.AddString(m_wndQueueBar.dll.vDevices[i]);
  }
  activeView=NULL;
  return 0L;
}

// CMainFrame message handlers



