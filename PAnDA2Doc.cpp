// PAnDA2Doc.cpp : implementation of the CPAnDA2Doc class
//

#include "stdafx.h"
#include "PAnDA2.h"

#include "PAnDA2Doc.h"
#include "PAnDA2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPAnDA2Doc

IMPLEMENT_DYNCREATE(CPAnDA2Doc, CDocument)

BEGIN_MESSAGE_MAP(CPAnDA2Doc, CDocument)
END_MESSAGE_MAP()


// CPAnDA2Doc construction/destruction

CPAnDA2Doc::CPAnDA2Doc()
{
	// TODO: add one-time construction code here
}

CPAnDA2Doc::~CPAnDA2Doc()
{
}

BOOL CPAnDA2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CPAnDA2Doc serialization
/*
void CPAnDA2Doc::Serialize(CArchive& ar)
{
	CStringA file_path;
	file_path = ar.GetFile()->GetFilePath();
	//ar object automatically opens a file for read/writing, once we have this filename, we need to close it
	//so the parameters class can access it for reading or writing
	ar.GetFile()->Close();

	if (ar.IsStoring())
	{
		UpdateParams();
		m_parameters.WriteParameters(file_path.GetBuffer(file_path.GetLength()));
	}
	else
	{
		m_parameters.ReadParameters(file_path.GetBuffer(file_path.GetLength()));
	}
}
*/


Parameters * CPAnDA2Doc::GetParams()
{
	return &m_parameters;
}

void CPAnDA2Doc::UpdateParams()
{
	CPAnDA2View * viewPtr;
	viewPtr =(CPAnDA2View *)m_viewList.GetHead();
	CStringA temp;

	//shared parameters
	viewPtr->m_SharedPage.UpdateData(TRUE);
	char methodtemplate[256];
	char sequencetemplate[256];
	char databasename[64];
	char massanalyzer[64];
  char startdevice[64];

	temp = viewPtr->m_SharedPage.sharedMethod;
	strcpy(methodtemplate, temp);
	temp = viewPtr->m_SharedPage.sharedSequence;
	strcpy(sequencetemplate, temp);
	temp = viewPtr->m_SharedPage.sharedDbName;
	strcpy(databasename, temp);
	temp = viewPtr->m_SharedPage.sharedAnalyzer;
	strcpy(massanalyzer, temp);
  temp = viewPtr->m_SharedPage.sharedStartDevice;
	strcpy(startdevice, temp);

	m_parameters.SetSharedParams(methodtemplate, sequencetemplate, databasename, massanalyzer, startdevice);

	//Single Run parameters
	int * singleiterations;
	int * singlemsmscoverage;
	float * singlertwin;
  CString cstr;
  cstr=massanalyzer;
  if(cstr.Find(_T("FT")) < 0 && cstr.Find(_T("Orbitrap")) < 0 ){
		viewPtr->m_MscoutPage.UpdateData(TRUE);
		singleiterations = &(viewPtr->m_MscoutPage.singleIterations);
		singlemsmscoverage = &(viewPtr->m_MscoutPage.singleMSMSCoverage);
		singlertwin = &(viewPtr->m_MscoutPage.singleRTWin);
	}
	else{
		viewPtr->m_HoopPage.UpdateData(TRUE);
		singleiterations = &(viewPtr->m_HoopPage.singleIterations);
		singlemsmscoverage = &(viewPtr->m_HoopPage.singleMSMSCoverage);
		singlertwin = &(viewPtr->m_HoopPage.singleRTWin);
	}

	m_parameters.SetSingleParams(*singleiterations, *singlemsmscoverage, *singlertwin);

	//Hardklor parameters
	viewPtr->m_HoopPage.UpdateData(TRUE);
	char hkanalyzer[64];
	int * hkchargemin;
	int * hkchargemax;
	float * hkcorr;
	int * hkmaxdepth;
	int * hkmaxpeps;
	int * hkresolution;
	float * hksnratio;
	float * hksnwin;
	float * hkwinsize;
	BOOL * hksn;

	temp = viewPtr->m_HoopPage.hkAnalyzer;
	strcpy(hkanalyzer, temp);	
	hkchargemin = &(viewPtr->m_HoopPage.hkChargeMin);
	hkchargemax = &(viewPtr->m_HoopPage.hkChargeMax);
	hkcorr = &(viewPtr->m_HoopPage.hkCorr);
	hkmaxdepth = &(viewPtr->m_HoopPage.hkMaxDepth);
	hkmaxpeps = &(viewPtr->m_HoopPage.hkMaxPeps);
	hkresolution = &(viewPtr->m_HoopPage.hkResolution);
	hksnratio = &(viewPtr->m_HoopPage.hkSNRatio);
	hksnwin = &(viewPtr->m_HoopPage.hkSNWin);
	hkwinsize = &(viewPtr->m_HoopPage.hkWinSize);
	hksn = &(viewPtr->m_HoopPage.hkSN);

	m_parameters.SetHKParams(hkanalyzer, *hkchargemin, *hkchargemax, *hkmaxdepth,
		*hkmaxpeps, *hkresolution, *hkcorr, *hkwinsize, *hksnratio, *hksnwin, *hksn);

	//MScout Parameters
	viewPtr->m_MscoutPage.UpdateData(TRUE);
	float * msrti;
	float * msrtf;
	float * mstime;
	double * msmz;
	float * mssn;
	float * msorder;
	bool * msskipzero;
	bool * mscompare;
	bool * mscentroid;
	bool * mshistograms;
	bool * mssiglist;

	msrti = &(viewPtr->m_MscoutPage.msRti);
	msrtf = &(viewPtr->m_MscoutPage.msRtf);
	mstime = &(viewPtr->m_MscoutPage.msTime);
	msmz = &(viewPtr->m_MscoutPage.msMz);
	mssn = &(viewPtr->m_MscoutPage.msSn);
	msorder = &(viewPtr->m_MscoutPage.msOrder);
	msskipzero = &(viewPtr->m_MscoutPage.msSkipzero);
	mscompare = &(viewPtr->m_MscoutPage.msCompare);
	mscentroid = &(viewPtr->m_MscoutPage.msCentroid);
	mshistograms = &(viewPtr->m_MscoutPage.msHistograms);
	mssiglist = &(viewPtr->m_MscoutPage.msSiglist);

	m_parameters.SetMScoutParams(*msrti, *msrtf, *mstime, *msmz, *mssn, *msorder,
		*msskipzero, *mscompare, *mscentroid, *mshistograms, *mssiglist);
}

BOOL CPAnDA2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CStringA file_path(lpszPathName);
  //MH: Hang onto this line, still thinking about using it.
  //WideCharToMultiByte(CP_ACP,0,lpszPathName,-1,fpath,256,NULL,NULL);
	if (m_parameters.ReadParameters(file_path.GetBuffer(file_path.GetLength()))){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CPAnDA2Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CStringA file_path(lpszPathName);
  //MH: Hang onto this line, still thinking about using it.
  //WideCharToMultiByte(CP_ACP,0,lpszPathName,-1,fpath,256,NULL,NULL);
	UpdateParams();
	if (m_parameters.WriteParameters(file_path.GetBuffer(file_path.GetLength()))){
		return TRUE;
	}
	else{
		return FALSE;
	}
}


// CPAnDA2Doc diagnostics

#ifdef _DEBUG
void CPAnDA2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPAnDA2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPAnDA2Doc commands
