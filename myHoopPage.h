#pragma once


// myHoopPage dialog

class myHoopPage : public CDialog
{
	DECLARE_DYNAMIC(myHoopPage)

public:
	myHoopPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~myHoopPage();

// Dialog Data
	enum { IDD = IDD_HOOPPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CString hkAnalyzer;
  int hkResolution;
  float hkWinSize;
  int hkMaxPeps;
  int hkMaxDepth;
  int hkChargeMin;
  int hkChargeMax;
  float hkCorr;
  float hkSNRatio;
  float hkSNWin;
  BOOL hkSN;
  int singleIterations;
  int singleMSMSCoverage;
  float singleRTWin;
};
