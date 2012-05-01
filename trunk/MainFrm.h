// MainFrm.h : interface of the CMainFrame class
//

#include "MessagePump.h"
#include "QueueBar.h"
#include "LogView.h"
#include "MDIClientWnd.h"
#include "PAnDA2Doc.h"
#include "PAnDA2View.h"

#pragma once

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
  CQueueBar   m_wndQueueBar;
  CMDIClientWnd m_wndMDIClient;
  CLogView    m_wndLogView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //afx_msg BOOL OnEraseBkgnd(CDC *pDC);
  afx_msg LRESULT SingleDone(WPARAM,LPARAM);
  afx_msg LRESULT AddToLog(WPARAM,LPARAM);
  afx_msg LRESULT ChangeLogFile(WPARAM,LPARAM);
  afx_msg LRESULT OnFileNew(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

  //MH: My functions
  void OnLogWindow();
  void OnQueueUp();
  void OnQueueDown();
  void OnQueueDelete();
  void SubmitToQueue();
  void StartQueue();
  //void OnFileNew();

  //MH: My variables/classes
  bool bLogWindow;
  cMessagePump mPump;
  TCHAR cwd[MAX_PATH]; //current working directory

  //For the menu?
  afx_msg void OnClickLogView(CCmdUI *pCmdUI);
};


