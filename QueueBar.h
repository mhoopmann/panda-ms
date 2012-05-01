#pragma once
#include "afxwin.h"
#include "acquisitionctrl1.h"

//MH: My headers
#include "Parameters.h"
#include "SingleAnalysis.h"
#include "DLLHandler.h"
#include "MessagePump.h"
#include <vector>

using namespace std;

// QueueBar dialog

class CQueueBar : public CDialogBar
{
	DECLARE_DYNAMIC(CQueueBar)

public:
	CQueueBar();   // standard constructor
	virtual ~CQueueBar();

// Dialog Data
	enum { IDD = IDD_QUEUEBAR };

protected:
  //MH: Additional data members
  vector<Parameters> queue;
  Parameters activeQueue;
  bool bPandaRunning;

  //cDLLHandler dll;
  cMessagePump *mPump;

  //MH: For Single Analysis
  CSingleAnalysis singleAnalysis;
  CWinThread *activeThread;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  afx_msg LONG OnInitDialog(UINT,LONG);

	DECLARE_MESSAGE_MAP()
public:
  cDLLHandler dll;
  CEdit activeQueueCtrl;
  CListBox queueCtrl;
  CButton upButtonCtrl;
  CButton downButtonCtrl;
  CButton deleteButtonCtrl;
  CString activeQueueText;
  CAcquisitionctrl1 conAC;
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  afx_msg void OnBnClickedButton3();

  //MH: QueueBar functions
  bool AddToQueue(Parameters &queueObj);
  bool BumpDown(int index);
  bool BumpUp(int index);
  void DoEvents();
  bool CheckStatusCurrentQueue();
  bool CheckQueueItem(char *itm);
  bool DeleteFromQueue(int index);
  bool PopNext();
  void SetCWD(TCHAR *tc);
  void SetMessagePump(cMessagePump *mp);
  afx_msg void StartQueue();
  DECLARE_EVENTSINK_MAP()
  void RunEndedAcquisitionctrl1();
  void ProgramErrorAcquisitionctrl1(BOOL InstrumentError, long Flags, long ErrorCode, long VirDeviceID, long MsgTime, LPCTSTR Message, LPCTSTR Title);
  void SequenceChangeAcquisitionctrl1();
};
