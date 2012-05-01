// MDIClientWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PAnDA2.h"
#include "MDIClientWnd.h"
#include "afxstr.h"
#include "atlimage.h"


// CMDIClientWnd

IMPLEMENT_DYNAMIC(CMDIClientWnd, CWnd)

CMDIClientWnd::CMDIClientWnd()
{
  m_brush.CreateSolidBrush(RGB(128,128,128));
  todaysPic=CheckDate();
}

CMDIClientWnd::~CMDIClientWnd()
{
}

BOOL CMDIClientWnd::OnEraseBkgnd(CDC* pDC) 
{
  return FALSE;
}

void CMDIClientWnd::OnPaint(){
  CPaintDC dc(this);
  CRect rcClient;
  GetClientRect(rcClient);

  CDC dcMem1; // memory DC for double buffering
  dcMem1.CreateCompatibleDC(&dc);
  CBitmap bitmap1;
  bitmap1.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
  CBitmap* pOldBitmap1 = dcMem1.SelectObject(&bitmap1);
  dcMem1.FillRect(rcClient, &m_brush);

  CDC dcMem2; // memory DC for the bitmap to draw
  dcMem2.CreateCompatibleDC(&dc);
  CBitmap bitmap2;
  switch(todaysPic){
    case 0: bitmap2.LoadBitmap(IDB_BITMAP2); break;
    case 1: bitmap2.LoadBitmap(IDB_BITMAP1); break;
    default: bitmap2.LoadBitmap(IDB_BITMAP2); break;
  }
  CBitmap* pOldBitmap2 = dcMem2.SelectObject(&bitmap2);

  BITMAP bmp;
  bitmap2.GetBitmap(&bmp);
  int nWidth  = bmp.bmWidth;
  int nHeight = bmp.bmHeight;
  int x       = (rcClient.Width() / 2) - (nWidth / 2);
  int y       = (rcClient.Height() / 2) - (nHeight / 2);
  dcMem1.BitBlt(x, y, nWidth, nHeight, &dcMem2, 0, 0, SRCCOPY);

  CRect rcClip;
  dc.GetClipBox(rcClip);

  nWidth  = rcClip.Width();
  nHeight = rcClip.Height();
  x       = rcClip.left;
  y       = rcClip.top;
  // copy memory device context in client device context
  dc.BitBlt(x, y, nWidth, nHeight, &dcMem1, x, y, SRCCOPY); 

  dcMem1.SelectObject(pOldBitmap1);
  dcMem2.SelectObject(pOldBitmap2);
}

void CMDIClientWnd::OnSize(UINT nType, int cx, int cy){
  CWnd::OnSize(nType, cx, cy);
  Invalidate(FALSE);
}

int CMDIClientWnd::CheckDate(){
  CTime today = CTime::GetCurrentTime();
  if(today.GetMonth()==4 && today.GetDay()==22) return 1;
  return 0;
}

BEGIN_MESSAGE_MAP(CMDIClientWnd, CWnd)
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
  ON_WM_SIZE()
END_MESSAGE_MAP()



// CMDIClientWnd message handlers


