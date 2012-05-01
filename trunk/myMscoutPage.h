#pragma once


// myMscoutPage dialog

class myMscoutPage : public CDialog
{
	DECLARE_DYNAMIC(myMscoutPage)

public:
	myMscoutPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~myMscoutPage();

// Dialog Data
	enum { IDD = IDD_MSCOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int singleIterations;
	int singleMSMSCoverage;
	float msRti;
	float msRtf;
	float msTime;
	double msMz;
	float msSn;
	float msOrder;
	bool msSkipzero, msCompare, msCentroid, msHistograms, msSiglist;
	float singleRTWin;
};
