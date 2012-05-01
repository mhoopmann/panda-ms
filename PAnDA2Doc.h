// PAnDA2Doc.h : interface of the CPAnDA2Doc class
//


#pragma once
#include "Parameters.h"


class CPAnDA2Doc : public CDocument
{
protected: // create from serialization only
	CPAnDA2Doc();
	DECLARE_DYNCREATE(CPAnDA2Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
//	virtual void Serialize(CArchive& ar);

//JE: Functions
public:
	Parameters * GetParams() ;
  //MH - Made this public
	void UpdateParams();		//Updates the parameters from the values in each dialog box in the view class by calling
						//Get ParamsFromDlg in the view class

//JE: Variables
public:
	Parameters m_parameters;

// Implementation
public:
	virtual ~CPAnDA2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


