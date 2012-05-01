#pragma once

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CAcquisitionctrl1 wrapper class

class CAcquisitionctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CAcquisitionctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x7ADCDC5E, 0xD0B6, 0x11D1, { 0xAA, 0x76, 0x0, 0x20, 0x78, 0x13, 0x69, 0x98 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:


// Operations
public:

// _DAcquisition

// Functions
//

	void ShowHomePage(BOOL Visibility)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visibility);
	}
	long GetDeviceNames(BSTR * DeviceNames)
	{
		long result;
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DeviceNames);
		return result;
	}
	long SubmitSequence(LPCTSTR UserName, BSTR * SequenceName, short StartRow, short StopRow, BOOL Priority, BOOL StartWhenReady, LPCTSTR StartUpIM, LPCTSTR ShutDownIM, LPCTSTR PreAcquisitionP, LPCTSTR PostAcquisitionP, BOOL SynchronouslyRunPre, BOOL SynchronouslyRunPost, BOOL Quan, BOOL Qual, BOOL SampleReports, BOOL SummaryReports, BOOL Programs, BOOL PrintMethods, BOOL CreateSummary, LPCTSTR DeviceNames, short StartDevice, short PostSequenceOperatingState)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PBSTR VTS_I2 VTS_I2 VTS_BOOL VTS_BOOL VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BSTR VTS_I2 VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UserName, SequenceName, StartRow, StopRow, Priority, StartWhenReady, StartUpIM, ShutDownIM, PreAcquisitionP, PostAcquisitionP, SynchronouslyRunPre, SynchronouslyRunPost, Quan, Qual, SampleReports, SummaryReports, Programs, PrintMethods, CreateSummary, DeviceNames, StartDevice, PostSequenceOperatingState);
		return result;
	}
	long GetDeviceStatuses(VARIANT * DeviceStatuses)
	{
		long result;
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DeviceStatuses);
		return result;
	}
	long ChangeDevicesOperatingMode(short OperatingMode)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, OperatingMode);
		return result;
	}
	long SendAnalysisCommand(short Command)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Command);
		return result;
	}
	long CheckDiskSpace(LPCTSTR SequencePathName, long * BytesSpaceFree, short RowToCheck)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PI4 VTS_I2 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SequencePathName, BytesSpaceFree, RowToCheck);
		return result;
	}
	long ChangeOperatingModeByName(short OperatingMode, LPCTSTR DeviceName)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_BSTR ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, OperatingMode, DeviceName);
		return result;
	}
	long ChangeOperatingModeByType(short OperatingMode, short DeviceType)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, OperatingMode, DeviceType);
		return result;
	}
	CString GetRunManagerStatus()
	{
		CString result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL GetSeqQueuePaused()
	{
		BOOL result;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long SendAnalysisCommand2(short Command)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Command);
		return result;
	}
	long GetDeviceStatuses2(VARIANT * DeviceStatuses)
	{
		long result;
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DeviceStatuses);
		return result;
	}
	long SubmitSequence2(LPCTSTR UserName, BSTR * SequenceName, short StartRow, short StopRow, BOOL Priority, BOOL StartWhenReady, LPCTSTR StartUpIM, LPCTSTR ShutDownIM, LPCTSTR PreAcquisitionP, LPCTSTR PostAcquisitionP, BOOL SynchronouslyRunPre, BOOL SynchronouslyRunPost, BOOL Quan, BOOL Qual, BOOL SampleReports, BOOL SummaryReports, BOOL Programs, BOOL PrintMethods, BOOL CreateSummary, LPCTSTR DeviceNames, short StartDevice, short PostSequenceOperatingState)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PBSTR VTS_I2 VTS_I2 VTS_BOOL VTS_BOOL VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BSTR VTS_I2 VTS_I2 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UserName, SequenceName, StartRow, StopRow, Priority, StartWhenReady, StartUpIM, ShutDownIM, PreAcquisitionP, PostAcquisitionP, SynchronouslyRunPre, SynchronouslyRunPost, Quan, Qual, SampleReports, SummaryReports, Programs, PrintMethods, CreateSummary, DeviceNames, StartDevice, PostSequenceOperatingState);
		return result;
	}
	short GetDeviceInfoArray(VARIANT * DeviceInfoArray)
	{
		short result;
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I2, (void*)&result, parms, DeviceInfoArray);
		return result;
	}
	long GetThisDeviceInfo(VARIANT * DeviceInfo, LPCTSTR DeviceShortName)
	{
		long result;
		static BYTE parms[] = VTS_PVARIANT VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DeviceInfo, DeviceShortName);
		return result;
	}
	short GetDevicesByType(short DeviceType, VARIANT * ArrConfiguredDevices)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_PVARIANT ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I2, (void*)&result, parms, DeviceType, ArrConfiguredDevices);
		return result;
	}

// Properties
//



};
