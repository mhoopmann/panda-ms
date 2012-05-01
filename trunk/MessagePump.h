#pragma once
#include "afxwin.h"

#define WM_SINGLE_DONE (WM_USER + 5)
#define WM_LOG_THIS (WM_USER + 2)
#define WM_LOG_FILE (WM_USER + 3)

class cMessagePump{
public:
  CString ReceiveMessage();
  void ChangeLogFile(CString str);
  void SendMessage(CString str);
  void SetParent(CWnd *w);
  void SingleDone();

protected:
private:
  CString message;
  CWnd *wnd;
};