// PAnDA2View.h : interface of the CPAnDA2View class
//


#pragma once
#include "afxcmn.h"

#include "mySharedPage.h"
#include "myHoopPage.h"
#include "myJarPage.h"
#include "myMscoutPage.h"
#include "Parameters.h"


class CPAnDA2View : public CFormView
{
protected: // create from serialization only
	CPAnDA2View();
	DECLARE_DYNCREATE(CPAnDA2View)

public:
	enum{ IDD = IDD_PANDA2_FORM };

// Attributes
public:
	CPAnDA2Doc* GetDocument() const;
	mySharedPage m_SharedPage;
	myHoopPage m_HoopPage;
	myJarPage m_JarPage;
	myMscoutPage m_MscoutPage;
private:
	CRect tabPageSize;

//JE: My functions
public:
//	GetParamsFromDlg(Parameters * params);	//modifies the parameters variable with parameter values in dialogs  TODO
private:
	void LoadDocVals();  //loads values from document into each dialog by callling update on each dlg   TODO
//	void GetHKParams(Parameters * params);	//get HK params from dialog, update params variable passed
										//calls params.SetHkParams, updates params from direct access to dlg values
//	GetFSParams  etc. etc.
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CPAnDA2View();
	void ShowTabPage(int number);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	CComboBoxEx myLogBox;
	CTabCtrl myTabCtrl;
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#ifndef _DEBUG  // debug version in PAnDA2View.cpp
inline CPAnDA2Doc* CPAnDA2View::GetDocument() const
   { return reinterpret_cast<CPAnDA2Doc*>(m_pDocument); }
#endif

