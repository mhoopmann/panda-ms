// PAnDA2View.cpp : implementation of the CPAnDA2View class
//

#include "stdafx.h"
#include "PAnDA2.h"

#include "PAnDA2Doc.h"
#include "PAnDA2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPAnDA2View

IMPLEMENT_DYNCREATE(CPAnDA2View, CFormView)

BEGIN_MESSAGE_MAP(CPAnDA2View, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CPAnDA2View::OnTcnSelchangeTab2)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CPAnDA2View construction/destruction

CPAnDA2View::CPAnDA2View()
	: CFormView(CPAnDA2View::IDD)
{
	// TODO: add construction code here

}

CPAnDA2View::~CPAnDA2View()
{
}

void CPAnDA2View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOBOXEX1, myLogBox);
	DDX_Control(pDX, IDC_TAB2, myTabCtrl);
}

BOOL CPAnDA2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CPAnDA2View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_SharedPage.Create(IDD_SHAREDPAGE, this);
	m_HoopPage.Create(IDD_HOOPPAGE, this);
	//m_JarPage.Create(IDD_JARPAGE, this);
	m_MscoutPage.Create(IDD_MSCOUT, this);

	TCITEM tabItem;
	tabItem.mask = TCIF_TEXT;

	tabItem.pszText = _T("PAnDA Setup");
	myTabCtrl.InsertItem(0, &tabItem);

	tabItem.pszText = _T("Single Sample Analysis");
	myTabCtrl.InsertItem(1, &tabItem);

	//tabItem.pszText = _T("Jarrett");
	//myTabCtrl.InsertItem(2, &tabItem);

	LoadDocVals();

	ShowTabPage(0);

	ResizeParentToFit();
}


// CPAnDA2View diagnostics

#ifdef _DEBUG
void CPAnDA2View::AssertValid() const
{
	CFormView::AssertValid();
}

void CPAnDA2View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CPAnDA2Doc* CPAnDA2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPAnDA2Doc)));
	return (CPAnDA2Doc*)m_pDocument;
}
#endif //_DEBUG


// CPAnDA2View message handlers

void CPAnDA2View::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_SharedPage.UpdateData(TRUE);
	ShowTabPage(myTabCtrl.GetCurFocus());
	pNMHDR=NULL;
	*pResult = 0;
}

void CPAnDA2View::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);
	if(bShow)
	{
		ShowTabPage(0);
	}
}

void CPAnDA2View::ShowTabPage(int number)
{
	int windowCount = 3;

	// Set the size and location of the child windows based on the tab control
	tabPageSize.left = 12;
	tabPageSize.top = 32;
	tabPageSize.right = tabPageSize.left + 443;
	tabPageSize.bottom = tabPageSize.top + 240;

	if((number >=0) && (number < windowCount))
	{
		CDialog *m_dPointer[3];

		if (m_SharedPage.sharedAnalyzer.Find(_T("FT")) < 0 && m_SharedPage.sharedAnalyzer.Find(_T("Orbitrap")) < 0 ){
			m_dPointer[1] = &m_MscoutPage;
			m_HoopPage.ShowWindow(SW_HIDE);
		}
		else{
			m_dPointer[1] = &m_HoopPage;
			m_MscoutPage.ShowWindow(SW_HIDE);
		}

		m_dPointer[0] = &m_SharedPage;
		m_dPointer[2] = &m_JarPage;

		// Hide every window except for the chosen one

		for (int count = 0; count < windowCount; count ++)
		{
			if (count !=number)
			{
				m_dPointer[count]->ShowWindow(SW_HIDE);
			}
			else if (count == number)
			{
				// Show the chosen window and set it's location

				m_dPointer[count]->SetWindowPos(&wndTop, tabPageSize.left,
					tabPageSize.top, tabPageSize.right,
					tabPageSize.bottom, SWP_SHOWWINDOW);

				myTabCtrl.SetCurSel(count);
			}
		}
	}
}

void CPAnDA2View::LoadDocVals()
{
	Parameters * doc_params = GetDocument()->GetParams();

	//Shared Parameters (mySharedPage)
	if (doc_params->has_shared()){
		CString temp;
		char t_sharedMethod[256];
		char t_sharedSequence[256];
		char t_sharedDbName[64];
		char t_sharedAnalyzer[64];
    char t_sharedStartDevice[64];
		doc_params->GetSharedParams(t_sharedMethod, t_sharedSequence,
			t_sharedDbName, t_sharedAnalyzer, t_sharedStartDevice);
		temp = t_sharedMethod;
		m_SharedPage.sharedMethod = temp;
		temp = t_sharedSequence;
		m_SharedPage.sharedSequence = temp;
		temp = t_sharedDbName;
		m_SharedPage.sharedDbName = temp;
		temp = t_sharedAnalyzer;
		m_SharedPage.sharedAnalyzer = temp;
    temp = t_sharedStartDevice;
    m_SharedPage.sharedStartDevice = temp;
		m_SharedPage.UpdateData(FALSE);
	}

	//Hardklor Parameters (myHoopPage)
	if (doc_params->has_HK()){
		char t_hkAnalyzer[64];
		CString temp;
		doc_params->GetHKParams(t_hkAnalyzer, m_HoopPage.hkChargeMin, m_HoopPage.hkChargeMax, m_HoopPage.hkMaxDepth, m_HoopPage.hkMaxPeps, m_HoopPage.hkResolution,
		m_HoopPage.hkCorr, m_HoopPage.hkWinSize, m_HoopPage.hkSNRatio, m_HoopPage.hkSNWin, m_HoopPage.hkSN);
		doc_params->GetSingleParams(m_HoopPage.singleIterations, m_HoopPage.singleMSMSCoverage, m_HoopPage.singleRTWin);
		temp = t_hkAnalyzer;
		m_HoopPage.hkAnalyzer = temp;
		m_HoopPage.UpdateData(FALSE);
	}

	//MScout Parameters (myMscoutPage)
	if (doc_params->has_MScout()){
		doc_params->GetMScoutParams(m_MscoutPage.msRti, m_MscoutPage.msRtf, m_MscoutPage.msTime, m_MscoutPage.msMz, m_MscoutPage.msSn, m_MscoutPage.msOrder, m_MscoutPage.msSkipzero, 
			m_MscoutPage.msCompare, m_MscoutPage.msCentroid, m_MscoutPage.msHistograms, m_MscoutPage.msSiglist);
		doc_params->GetSingleParams(m_MscoutPage.singleIterations, m_MscoutPage.singleMSMSCoverage, m_MscoutPage.singleRTWin);
		m_MscoutPage.UpdateData(FALSE);
	}

}