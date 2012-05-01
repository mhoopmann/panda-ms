#include "stdafx.h"
#include "DLLHandler.h"

int cDLLHandler::LoadDll(){
  
  int nRetCode;
  HRESULT hr;

  seqPtr=NULL;
  samplePtr=NULL;
  samplesPtr=NULL;
  methodPtr=NULL;
  methodPtr2=NULL;
   
  CoInitialize( NULL );		
	hr = seqPtr.CreateInstance("XSequence.XSequence.1");
	if (FAILED(hr)) {
    AfxMessageBox(_T("Failed to load XSequence (XcaliburFiles.dll)"));
    nRetCode = 2;
    return nRetCode;
  }
	hr = samplePtr.CreateInstance("XSample.XSample.1");
	if (FAILED(hr)) {
    AfxMessageBox(_T("Failed to load XSample (XcaliburFiles.dll)"));
    nRetCode = 2;
    return nRetCode;
  }
	hr = samplesPtr.CreateInstance("XSamples.XSamples.1");
	if (FAILED(hr)) {
    AfxMessageBox(_T("Failed to load XSamples (XcaliburFiles.dll)"));
    nRetCode = 2;
    return nRetCode;
  }
  hr = methodPtr.CreateInstance("LTMethod.LTMethod.1");
	if (FAILED(hr)) {
    AfxMessageBox(_T("Failed to load LTMethod (LTMethod.dll)"));
    nRetCode = 2;
    return nRetCode;
  }   
  hr = methodPtr2.CreateInstance(__uuidof(XCALIBURINSTMETHEDITLib::XInstMethEdit));
	if (FAILED(hr)) {
    AfxMessageBox(_T("Failed to load XInstMethEdit (XInstMethEdit.dll)"));
    nRetCode = 2;
    return nRetCode;
  }

  CString str;
  str+="DLL loaded ok.";
  mPump->SendMessageW(str);
  GetDeviceNames();
  CheckStatus(true);
  return 0;
}

CString cDLLHandler::GetDeviceList(){
  return sDeviceList;
}

void cDLLHandler::GetDeviceNames(){
	BSTR DeviceList(NULL);
	conAC->GetDeviceNames (&DeviceList);
	sDeviceList = DeviceList;
  CString str;
	
	if (!sDeviceList.IsEmpty())	{
		TCHAR *Token = _tcstok(DeviceList, _T(","));
		while (Token != NULL) {
      mPump->SendMessageW(Token);
      str=Token;
      vDevices.push_back(str);
			Token = _tcstok(NULL, _T(","));
		}
	}	else	{
    mPump->SendMessageW(_T("There are no configured devices"));
	}

	SysFreeString(DeviceList);
}

bool cDLLHandler::CheckStatus(bool out){

	VARIANT Array;
	ZeroMemory(&Array, sizeof(VARIANT));
	conAC->GetDeviceStatuses(&Array);
  unsigned long i;
  bool bStatus=true;

	if (Array.vt == (VT_ARRAY | VT_VARIANT) && Array.parray) {
		SAFEARRAY *pStatuses = Array.parray;
		VARIANT ElementName;
		VARIANT ElementStatus;
		ZeroMemory(&ElementName, sizeof(VARIANT));
		ZeroMemory(&ElementStatus, sizeof(VARIANT));
		long indexes[2];
		CString	sMessage;

		for (i=0; i < pStatuses->rgsabound[0].cElements; i++) {
			indexes[0] = 0;//this should be the name
			indexes[1] = i;
			SafeArrayGetElement(pStatuses, indexes, &ElementName);
			indexes[0]++;
			SafeArrayGetElement(pStatuses, indexes, &ElementStatus);

			if (ElementName.vt == VT_BSTR && ElementStatus.vt == VT_UI4) {
				CString Temp(ElementName.bstrVal); //this will convert from OLE to UNICODE or ASCII
				sMessage.Format(_T("%s - status: %s"), Temp, StatusCodes(ElementStatus.ulVal));
        if(ElementStatus.ulVal==11 || ElementStatus.ulVal==12) bStatus=false;
        if(out){
          mPump->SendMessageW(sMessage);
        }
			}
			(void) VariantClear(&ElementName);
			(void) VariantClear(&ElementStatus);
		}
	}	else	{
    if(out) {
      mPump->SendMessageW(_T("There are no configured devices"));
    }
	}
	(void) VariantClear(&Array);
  return bStatus;
}

CString cDLLHandler::StatusCodes(unsigned int code){
	CString	sStatusString = _T("");

	switch (code)	{
	case 0: sStatusString = _T("initialising");		break;
	case 1: sStatusString = _T("ready to download");		break;
	case 2:	sStatusString = _T("downloading");		break;
	case 3:	sStatusString = _T("preparing for run");		break;
	case 4:	sStatusString = _T("ready for run");		break;
	case 5:	sStatusString = _T("waiting for contact closure");	break;
	case 6:	sStatusString = _T("running");		break;
	case 7:	sStatusString = _T("post run");		break;
	case 8:	sStatusString = _T("error");		break;
	case 9:	sStatusString = _T("busy");		break;
	case 10: sStatusString = _T("not connected");		break;
	case 11: sStatusString = _T("standby");		break;
	case 12: sStatusString = _T("off");		break;
	case 13: sStatusString = _T("server failed");		break;
	case 14: sStatusString = _T("lamp warmup");		break;
	case 15: sStatusString = _T("not ready");		break;
	case 16: sStatusString = _T("direct control");		break;
	default: sStatusString = _T("Illegal device status value");		break;
  }
	return sStatusString;
}

void cDLLHandler::SetMessagePump(cMessagePump *mp){
  mPump=mp;
}

void cDLLHandler::SetAcquisitionControl(CAcquisitionctrl1 *ac){
  conAC=ac;
}

void cDLLHandler::ReleaseAcquisitionControl(){
  conAC=NULL;
}

void cDLLHandler::PostMessage(CString str){
  mPump->SendMessage(str);
}

void cDLLHandler::PostMessage(char *str){
  CString s;
  s=str;
  mPump->SendMessage(s);
}

void cDLLHandler::ChangeLogFile(CString str){
  mPump->ChangeLogFile(str);
}

void cDLLHandler::ChangeLogFile(char *str){
  CString s;
  s=str;
  mPump->ChangeLogFile(s);
}
