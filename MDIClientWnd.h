#pragma once

#include "afxstr.h"
#include "atlimage.h"

// CMDIClientWnd

class CMDIClientWnd : public CWnd
{
	DECLARE_DYNAMIC(CMDIClientWnd)

public:
	CMDIClientWnd();
	virtual ~CMDIClientWnd();

protected:
  CBrush m_brush;
  CImage m_image;
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  int todaysPic;

  int CheckDate();

	DECLARE_MESSAGE_MAP()
};


