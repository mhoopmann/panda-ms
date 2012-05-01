#pragma once
#include "afxwin.h"
#include "acquisitionctrl1.h"
#include "MessagePump.h"
#include <vector>

#import "C:\Xcalibur\system\programs\XcaliburFiles.dll"
#import "C:\Xcalibur\system\LTQ\programs\LTMethod.dll"
//#import "LTMethod.dll"
#import "C:\Xcalibur\system\programs\XcaliburInstMethEdit.dll"

using namespace std;

class cDLLHandler{
public:
  int LoadDll();
  CString GetDeviceList();
  void GetDeviceNames();
  bool CheckStatus(bool out=false);
  CString StatusCodes(unsigned int code);

  void ChangeLogFile(CString str);
  void ChangeLogFile(char *str);
  void PostMessage(CString str);
  void PostMessage(char *str);
  void SetMessagePump(cMessagePump *mp);
  void SetAcquisitionControl(CAcquisitionctrl1 *ac);
  void ReleaseAcquisitionControl();

  //dll objects
  XCALIBURFILESLib::IXSequencePtr seqPtr;
  XCALIBURFILESLib::IXSample2Ptr samplePtr;
  XCALIBURFILESLib::IXSamplesPtr samplesPtr;
  LTMETHODLib::ILCQMethodPtr methodPtr;
  XCALIBURINSTMETHEDITLib::IXInstMethEditPtr methodPtr2;
  
  //Device control
  CAcquisitionctrl1 *conAC;

  //Variables
  vector<CString> vDevices;

private:
  //Message pump
  cMessagePump *mPump;

  //Variables
  CString sDeviceList;
  

protected:

};