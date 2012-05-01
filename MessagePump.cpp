#include "stdafx.h"
#include "MessagePump.h"


CString cMessagePump::ReceiveMessage(){
  return message;
}

void cMessagePump::SendMessage(CString str){
  message=str;
  if(wnd!=NULL) wnd->SendMessage(WM_LOG_THIS,0);
}

void cMessagePump::ChangeLogFile(CString str){
  message=str;
  if(wnd!=NULL) wnd->SendMessage(WM_LOG_FILE,0);
}

void cMessagePump::SetParent(CWnd *w){
  wnd=w;
}

void cMessagePump::SingleDone(){
  if(wnd!=NULL) wnd->SendMessage(WM_SINGLE_DONE,0);
}