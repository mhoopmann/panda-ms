#ifndef _SINGLE_ANALYSIS_H
#define _SINGLE_ANALYSIS_H

#include "Parameters.h"
#include "PAnDA.h"
#include "stdafx.h"
#include "DLLHandler.h"
#include <psapi.h>

class CSingleAnalysis {
private:
	int iteration;
  
  Parameters params;
  SharedParamList sharedParams;
  SingleParamList singleParams;
  HKParamList hardklorParams;
  cPAnDA pandaObj;
  TCHAR appCWD[MAX_PATH];
  cDLLHandler *dll;
  cMessagePump *mPump;

  void DoEvents();
  void MakeHKConf();
  void RunMakeMS2();
  void RunHardklor();
  void SA();

  //char dataPathA[MAX_PATH];

protected:
public:
  volatile bool runEnded;

  bool NextIteration();

  void EditMethod();
  void EditSequence();
  void SetDLL(cDLLHandler *d);
  void SetMessagePump(cMessagePump *p);
  void SetParams(Parameters &paramObj);
  void SetCWD(TCHAR *tc);
  void Setup();
  void SubmitSequence();

  static UINT ThreadProc(LPVOID obj);  
  void StartThread();

  //Event Handlers are observed by the QueueBar which needs to see a few base functions
  //These base function will be in each analysis type.
  //void StartRun();
  //void RunEnded();

};

#endif