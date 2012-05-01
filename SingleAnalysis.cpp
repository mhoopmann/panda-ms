#include "SingleAnalysis.h"

//declare static variables

void CSingleAnalysis::SetDLL(cDLLHandler *d){
  dll=d;
}

void CSingleAnalysis::SetParams(Parameters &paramObj){
  params=paramObj;

  //Get Parameters info
  params.GetSharedParams(sharedParams);
  params.GetSingleParams(singleParams);
  params.GetHKParams(hardklorParams);
}

void CSingleAnalysis::SetCWD(TCHAR *tc){
  _tcscpy(appCWD,tc);
}

void CSingleAnalysis::Setup(){
  TCHAR cwd[MAX_PATH];
  TCHAR db[MAX_PATH];
  char pathA[MAX_PATH];
  CString logFile;

  dll->PostMessage("Setting up the PAnDA analysis.");

  //Get the database name in TCHAR format
  MultiByteToWideChar(CP_ACP,0,sharedParams.DatabaseName,-1,db,sizeof(db)/sizeof(TCHAR));

  //Create subfolder for analysis in data directory
  //::GetCurrentDirectory(sizeof(cwd)-1,cwd);
  _tcscpy(cwd,appCWD);
  _tcscat(cwd,_T("\\Data\\"));
  _tcscat(cwd,db);
  ::CreateDirectory(cwd,NULL);
  WideCharToMultiByte(CP_ACP,0,cwd,-1,pathA,sizeof(pathA)/sizeof(char),NULL,NULL);

  //Set the log file
  logFile=cwd;
  logFile+="\\";
  logFile+=sharedParams.DatabaseName;
  logFile+=".log";
  dll->ChangeLogFile(logFile);
  
  //Set the full database path in panda object
  pandaObj.SetDatabase(pathA,sharedParams.DatabaseName);

  //Set any additional parameters
  pandaObj.SetParameters(singleParams.singleRTWin);
  iteration=1;
  EditSequence();
}


void CSingleAnalysis::EditMethod(){
  HRESULT hr;
  vector<MassList> vList;
  MassList ml;
  unsigned int i;
  FILE *f;
  CString str;
  //TCHAR cwd[MAX_PATH];
  char cwdA[MAX_PATH];
  char mList[MAX_PATH];
  char strA[128];

  //Get the current working directory
  //::GetCurrentDirectory(sizeof(cwd)-1,cwd);
  //_tcscpy(cwd,appCWD);
  WideCharToMultiByte(CP_ACP,0,appCWD,-1,cwdA,sizeof(cwdA)/sizeof(char),NULL,NULL);
  strcat(cwdA,"\\Data\\");
  strcat(cwdA,sharedParams.DatabaseName);

  //open mass list
  sprintf(strA,"\\%slist-%d.txt",sharedParams.DatabaseName,iteration-1);
  strcpy(mList,cwdA);
  strcat(mList,strA);
  str="Reading mass list: ";
  str+=mList;
  dll->PostMessage(str);
  f=fopen(mList,"rt");
  while(!feof(f)){
    fscanf(f,"%lf\t%f\t%f\n",&ml.mz,&ml.firstRT,&ml.lastRT);
    vList.push_back(ml);
  }
  fclose(f);

  //Add mass list to the method file
  dll->PostMessage("Generating new method file...");
  CString meth(sharedParams.MethodTemplate);
  hr=dll->methodPtr->Open((LPWSTR)(LPCTSTR)meth);
  dll->methodPtr->DataDepUseGlobalMassLists=1;
  dll->methodPtr->put_NumMassTimeWindows(0,vList.size());
  for(i=0;i<vList.size();i++){
    hr=dll->methodPtr->SetMassTimeWindow(0,i,vList[i].firstRT,vList[i].lastRT,vList[i].mz);
  }
  hr=dll->methodPtr->Save();
  hr=dll->methodPtr->Close();

  dll->PostMessage("Done!");
}

void CSingleAnalysis::EditSequence(){
  HRESULT hr;
  CString meth;
  CString newSeq;
  CString rawName;
  CString path;
  //TCHAR cwd[MAX_PATH];
  char tmp[32];
  int i;

  dll->PostMessage("Setting up sequence...");

  //Set up method name
  meth=sharedParams.MethodTemplate;
  i=meth.ReverseFind('.');
  meth.Delete(i,5);

  //Set up raw file name
  sprintf(tmp,"raw-%d",iteration);
  rawName=sharedParams.DatabaseName;
  rawName+=tmp;

  //Edit the path
  path=appCWD;
  path+="\\Data\\";
  path+=sharedParams.DatabaseName;
  //AfxMessageBox(path);

  //Make name for new sequence file
  //::GetCurrentDirectory(sizeof(cwd)-1,cwd);
  newSeq=appCWD;
  newSeq+="\\Data\\";
  newSeq+=sharedParams.DatabaseName;
  newSeq+="\\PAnDA.sld";

  //Open and modify the sequence template
  CString seqTemp(sharedParams.SequenceTemplate);
  hr=dll->seqPtr->Open((LPWSTR)(LPCTSTR)seqTemp,0);
  dll->samplesPtr=dll->seqPtr->Samples;
  dll->samplePtr=dll->samplesPtr->Item(1);
  dll->samplePtr->put_FileName((LPWSTR)(LPCTSTR)rawName);
  dll->samplePtr->put_Path((LPWSTR)(LPCTSTR)path);
  dll->samplePtr->put_InstMeth((LPWSTR)(LPCTSTR)meth);
  hr=dll->seqPtr->SaveAs((LPWSTR)(LPCTSTR)newSeq,"User","No Comment",1);
  dll->seqPtr->Close();

  dll->PostMessage("Done!");
}

void CSingleAnalysis::RunMakeMS2(){
  /*
  //TCHAR cwd[MAX_PATH];
  char cwdA[MAX_PATH];
  char str[256];
  time_t start,stop;
  double zeit;
  int i;

  //For checking the MakeMS2 process
  DWORD pIDs[1024];
  DWORD dBytes,dBytes2;
  DWORD NumProcesses;
  HANDLE hProcess;
  HMODULE hMod;
  TCHAR procName[MAX_PATH]=TEXT("unknown");
  char procNameChar[MAX_PATH];
  bool bStillRunning=true;

  dll->PostMessage("Waiting 10 seconds for data acquisition to finalize.");
  time(&start);
  zeit=0;
  while(zeit<10){
    time(&stop);
    zeit=difftime(stop,start);
    DoEvents();
  }

  dll->PostMessage("Running MakeMS2:");

  //Get the current working directory
  //::GetCurrentDirectory(sizeof(cwd)-1,cwd);
  WideCharToMultiByte(CP_ACP,0,appCWD,-1,cwdA,sizeof(cwdA)/sizeof(char),NULL,NULL);
  strcat(cwdA,"\\Data\\");
  strcat(cwdA,sharedParams.DatabaseName);

  //Add the command line
  sprintf(str,"\\%sraw-%d.RAW -ms1 -ms2 -b -c -path %s\\",sharedParams.DatabaseName,iteration,cwdA);
  strcat(cwdA,str);

  //Run MakeMS2
  strcpy(str,"Apps\\MakeMS2.exe ");
  strcat(str,cwdA);

  dll->PostMessage(str);
  system(str);
  
  while(bStillRunning){
    bStillRunning=false;
    EnumProcesses(pIDs, sizeof(pIDs), &dBytes);
    NumProcesses = dBytes/sizeof(DWORD);
    for(i=0;i<NumProcesses;i++){
      hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pIDs[i]);
      if(hProcess==NULL) continue;
      if(EnumProcessModules(hProcess, &hMod, sizeof(hMod),&dBytes2)){
        GetModuleBaseName(hProcess, hMod, procName, sizeof(procName)/sizeof(TCHAR));
      }
      WideCharToMultiByte(CP_ACP,0,procName,-1,procNameChar,sizeof(procNameChar)/sizeof(char),NULL,NULL);
      //dll->PostMessage(procNameChar);
      if(strcmp(procNameChar,"MakeMS2.exe")==0) bStillRunning=true;
      CloseHandle(hProcess);
    }

    //Wait 5 seconds and check if process is still running
    time(&start);
    zeit=0;
    while(zeit<5){
      time(&stop);
      zeit=difftime(stop,start);
      DoEvents();
    }

  }
  */

  dll->PostMessage("Done with MakeMS2");
}

void CSingleAnalysis::RunHardklor(){
  char strp[256];
  char str[512];

  MakeHKConf();
  dll->PostMessage("Running Hardklor:");
  
  WideCharToMultiByte(CP_ACP,0,appCWD,-1,strp,sizeof(strp)/sizeof(char),NULL,NULL);
  strcpy(str,strp);
  strcat(str,"\\Apps\\hardklor -conf ");
  strcat(str,strp);
  strcat(str,"\\Data\\");
  strcat(str,sharedParams.DatabaseName);
  strcat(str,"\\PAnDA-hk.conf > ");

  strcat(str,strp);
  strcat(str,"\\Data\\");
  strcat(str,sharedParams.DatabaseName);
  strcat(str,"\\hardklor.log");

  dll->PostMessage(appCWD);
  dll->PostMessage(str);

  system(str);

  dll->PostMessage("Done with Hardklor");
}

void CSingleAnalysis::MakeHKConf(){
  FILE *out;
  //TCHAR cwd[MAX_PATH];
  char cwdA[MAX_PATH];
  char cwdA2[MAX_PATH];
  char inFile[256];
  char str[512];

  dll->PostMessage("Configuring Hardklor...");

  //Get the current working directory
  //::GetCurrentDirectory(sizeof(cwd)-1,cwd);
  WideCharToMultiByte(CP_ACP,0,appCWD,-1,cwdA,sizeof(cwdA)/sizeof(char),NULL,NULL);
  strcpy(cwdA2,cwdA);
  strcat(cwdA,"\\Data\\");
  strcat(cwdA,sharedParams.DatabaseName);

  //open the file
  strcpy(str,cwdA);
  strcat(str,"\\PAnDA-hk.conf");
  out=fopen(str,"wt");

  //Write the parameters
  //fprintf(out,"-hdat Apps\\hardklor.dat\n");
  strcpy(str,cwdA2);
  strcat(str,"\\Apps\\hardklor.dat\n");
  fprintf(out,"-hdat %s",str);

  //fprintf(out,"-mdat Apps\\ISOTOPE.dat\n");
  strcpy(str,cwdA2);
  strcat(str,"\\Apps\\ISOTOPE.dat\n");
  fprintf(out,"-mdat %s",str);

  fprintf(out,"-d %d\n",hardklorParams.Depth);
  fprintf(out,"-chMin %d\n",hardklorParams.Charge1);
  fprintf(out,"-chMax %d\n",hardklorParams.Charge2);
  fprintf(out,"-res %d %s\n",hardklorParams.Resolution,hardklorParams.Analyzer);
  fprintf(out,"-p %d\n",hardklorParams.MaxPeps);
  fprintf(out,"-sn %f\n",hardklorParams.SNRatio);
  if(hardklorParams.SNRatio>0) fprintf(out,"-snWin %f 1\n",hardklorParams.SNWin);
  fprintf(out,"-win %f\n",hardklorParams.WinSize);
  fprintf(out,"-corr %f\n",hardklorParams.Corr);
  fprintf(out,"-cdm Q\n");
  fprintf(out,"-i\n");
  fprintf(out,"-a FastFewestPeptides\n");
  fputs("\n",out);

  //write the files to analyze
  sprintf(str,"\\%sraw-%d.RAW\" ",sharedParams.DatabaseName,iteration);
  strcpy(inFile,"\"");
  strcat(inFile,cwdA);
  strcat(inFile,str);
  dll->PostMessage("Infile:");
  dll->PostMessage(inFile);
  
  sprintf(str,"\\%sraw-%d.hk\"",sharedParams.DatabaseName,iteration);
  strcat(inFile,"\"");
  strcat(inFile,cwdA);
  strcat(inFile,str);
  dll->PostMessage("Outfile:");
  dll->PostMessage(inFile);

  fputs(inFile,out);
  fclose(out);

  dll->PostMessage("Done!");

}

void CSingleAnalysis::DoEvents() {
	MSG msg;
  long sts;

 	do {
   	if (sts = PeekMessage(&msg, (HWND) NULL, 0, 0, PM_REMOVE)) {
 			TranslateMessage(&msg);
 			DispatchMessage(&msg);
		}
 	} while (sts);
}

void CSingleAnalysis::SubmitSequence(){
  LONG lRet;
  CString seq;
  TCHAR cwd[MAX_PATH];
  int startDevice=0;
  int i;
  int j;
  bool bMatch=false;
  CString strDevice;
  char msg[256];
  CString cmsg;

  //Set up the sequence name
  //::GetCurrentDirectory(sizeof(cwd)-1,cwd);
  //seq=cwd;
  seq=appCWD;
  seq+="\\Data\\";
  seq+=sharedParams.DatabaseName;
  seq+="\\PAnDA.sld"; 
  BSTR SeqName = seq.AllocSysString();

  //Check instrument status - make sure they are ready to go
  if(!dll->CheckStatus(true)){
    lRet = dll->conAC->ChangeDevicesOperatingMode(1);
    if (lRet == S_OK) {
      while(!dll->CheckStatus()) DoEvents();
      dll->PostMessage("All devices should be on.");
	  }	else {
      dll->PostMessage("Cannot turn on devices.");
	  }
  }

  strDevice=sharedParams.StartDevice;
  for(i=0;i<dll->vDevices.size();i++){
    if(dll->vDevices[i].Compare(strDevice)==0){
      cmsg="Starting Device: ";
      cmsg+=dll->vDevices[i];
      sprintf(msg,", number %d",i);
      cmsg+=msg;
      dll->PostMessage(cmsg);
      startDevice=i;
      bMatch=true;
      break;
    }
  }

  if(!bMatch) dll->PostMessage("WARNING: Starting Device not found! Using first available device instead.");

  //Submit sequence (many, many parameters)
  lRet = dll->conAC->SubmitSequence(
    (LPCTSTR)_T("PAnDAuser"), //LPCTSTR User name
    &SeqName, //BSTR* sequence name (the .sld)
    1, //short startrow
    1, //short stoprow
    FALSE, //bool priority
    TRUE, //bool startwhenready
    (LPCTSTR)_T(""), //LPCTSTR startupIM
    (LPCTSTR)_T(""), //LPCTSTR shutdownIM
    (LPCTSTR)_T(""), //LPCTSTR PreAcquisitionP
    (LPCTSTR)_T(""), //LPCTSTR PreAcquisitionP
    FALSE, //bool SynchronouslyRunPre
    FALSE, //bool SynchronouslyRunPost
    FALSE, //bool Quan
    FALSE, //bool Qual
    FALSE, //bool SampleReports
    FALSE, //bool SummaryReports
    FALSE, //bool Programs
    FALSE, //bool PrintMethods
    FALSE, //bool CreateSummary
    (LPCTSTR)dll->GetDeviceList(), //LPCTSTR DeviceNames
    startDevice, //short StartDevice
    2+1 //short PostSequenceOperatingState
    );

  //Check for success
  if (lRet == S_OK) {
    dll->PostMessage("Submitted Sequence.");
	}	else {
    dll->PostMessage("Failed to submit sequence.");
    sprintf(msg,"UserName,path,1,1,FALSE,TRUE,\"\",\"\",\"\",\"\",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,DeviceList,%d,2+1",startDevice);
    dll->PostMessage(msg);
    dll->PostMessage(seq);
	}

  /*
  i=0;
  while(true){
    if(AfxMessageBox(_T("Did device start?"),MB_YESNO)==IDYES) break;

    //Submit sequence (many, many parameters)
    lRet = dll->conAC->SubmitSequence(
    (LPCTSTR)_T("PAnDAuser"), //LPCTSTR User name
    &SeqName, //BSTR* sequence name (the .sld)
    1, //short startrow
    1, //short stoprow
    FALSE, //bool priority
    TRUE, //bool startwhenready
    (LPCTSTR)_T(""), //LPCTSTR startupIM
    (LPCTSTR)_T(""), //LPCTSTR shutdownIM
    (LPCTSTR)_T(""), //LPCTSTR PreAcquisitionP
    (LPCTSTR)_T(""), //LPCTSTR PreAcquisitionP
    FALSE, //bool SynchronouslyRunPre
    FALSE, //bool SynchronouslyRunPost
    FALSE, //bool Quan
    FALSE, //bool Qual
    FALSE, //bool SampleReports
    FALSE, //bool SummaryReports
    FALSE, //bool Programs
    FALSE, //bool PrintMethods
    FALSE, //bool CreateSummary
    (LPCTSTR)dll->GetDeviceList(), //LPCTSTR DeviceNames
    0+i, //short StartDevice
    2+1 //short PostSequenceOperatingState
    );
    i++;
  }

  //Check for success
  if (lRet == S_OK) {
    dll->PostMessage("Submitted Sequence.");
	}	else {
    dll->PostMessage("Failed to submit sequence.");
	}
  */


  //Clean up the memory
  SysFreeString(SeqName);
}

bool CSingleAnalysis::NextIteration(){
  if(iteration++<=singleParams.singleIterations) return true;
  return false;
}

void CSingleAnalysis::SetMessagePump(cMessagePump *p){
  mPump=p;
};

UINT CSingleAnalysis::ThreadProc(LPVOID obj){


  CSingleAnalysis *saObj =(CSingleAnalysis *)obj;
  saObj->SA();

  ///int PPID;
  //int Match;
  //char msg[256];
  
  //MH: Start thread code

  //Data analysis
  //RunMakeMS2();
  //RunHardklor();

  //PAnDA analysis
  //dll->PostMessage("Performing PAnDA analysis");
  //pandaObj.SingleAnalysis(iteration,singleParams.singleMSMSCoverage,PPID,Match);
  //sprintf(msg,"PAnDA complete: %d PPIDs, %d MS/MS Coverage",PPID,Match);
  //dll->PostMessage(msg);

  //MH: End thread code
  //cMessagePump *pump=(cMessagePump *)obj;
  //pump->SingleDone();
  //pump=NULL;

  return 0;
}

void CSingleAnalysis::SA(){

  int PPID=0;
  int Match=0;
  char msg[256];
  
  //MH: Start thread code

  //Data analysis
  //RunMakeMS2(); //Not needed anymore; Hardklor can read RAW files
  RunHardklor();

  //PAnDA analysis
  dll->PostMessage("Performing PAnDA analysis");
  pandaObj.SingleAnalysis(iteration,singleParams.singleMSMSCoverage,PPID,Match);
  sprintf(msg,"PAnDA complete: %d PPIDs, %d MS/MS Coverage",PPID,Match);
  dll->PostMessage(msg);

  //MH: End thread code
  //mPump->SingleDone();
  runEnded=true;
}

void CSingleAnalysis::StartThread(){
  runEnded=false;
  AfxBeginThread(ThreadProc,this);
}

/*
UINT CSingleAnalysis::ThreadProc(LPVOID obj){

  int PPID;
  int Match;
  char msg[256];
  
  //MH: Start thread code
  dll->PostMessage(_T("Testing DLL now."));
  dll->CheckStatus(true);

  //Get Parameters info
  params.GetSharedParams(sharedParams);
  params.GetSingleParams(singleParams);
  params.GetHKParams(hardklorParams);

  //Set up the analysis
  Setup();

  //Perform the analysis over and over and over and over and over
  while(iteration++<=singleParams.singleIterations){

    //Edit the Method and Sequence files
    if(iteration>1) EditMethod();
    EditSequence();

    //Wait for instrument to run data
    runEnded=false;
    SubmitSequence();
    while(!runEnded) DoEvents();

    //Data analysis
    RunMakeMS2();
    RunHardklor();

    //PAnDA analysis
    dll->PostMessage("Performing PAnDA analysis");
    pandaObj.SingleAnalysis(iteration,singleParams.singleMSMSCoverage,PPID,Match);
    sprintf(msg,"PAnDA complete: %d PPIDs, %d MS/MS Coverage",PPID,Match);
    dll->PostMessage(msg);

  }

  //Clean up - restore Method to default settings, etc.
  dll->ChangeLogFile("");

  //Release the dll (just in case)
  dll=NULL;

  //MH: End thread code
  CWnd *wnd=(CWnd *)obj;
  wnd->SendMessage(WM_SINGLE_DONE,0);

  return 0;
}
*/
