#include "PAnDA.h"

cPAnDA::cPAnDA(){
  vList.clear();
  RTCushion=0.0f;
}

bool cPAnDA::SingleAnalysis(int iteration, int MinCount, int &TotPPID, int &MatchSize){

  char hkFile[256];
  char ms2File[256];
  char massList[256];
  char str[256];

  //time the functions for optimization tests
  FILE *f;
  time_t start,stop;
  double zeit;

  //Set up database
  if(iteration==1) CreateDB();

  //Generate filenames
  f=fopen("debug.txt","at");
  strcpy(hkFile,path);
  sprintf(str,"\\%sraw-%d.hk",base,iteration);
  strcat(hkFile,str);
  fprintf(f,"Hardklor data file: %s\n",hkFile);

  strcpy(ms2File,path);
  sprintf(str,"\\%sraw-%d.RAW",base,iteration);
  strcat(ms2File,str);
  fprintf(f,"MS/MS data file: %s\n",ms2File);

  strcpy(massList,path);
  sprintf(str,"\\%slist-%d.txt",base,iteration);
  strcat(massList,str);
  fprintf(f,"Inclusion mass list file: %s\n",massList);
  fclose(f);

  //Add Kronik info to DB
  time(&start);
  AddKronik(hkFile);
  time(&stop);
  zeit=difftime(stop,start);
  f=fopen("time.txt","at");
  fprintf(f,"Add peptides to DB time: %lf seconds\n",zeit);
  fclose(f);

  //Add MS2 info to DB
  time(&start);
  AddMS2(ms2File);
  time(&stop);
  zeit=difftime(stop,start);
  f=fopen("time.txt","at");
  fprintf(f,"Add MS/MS to DB time: %lf seconds\n",zeit);
  fclose(f);


  //Generate Mass List
  time(&start);
  GenerateList(massList,MinCount,TotPPID,MatchSize);
  time(&stop);
  zeit=difftime(stop,start);
  f=fopen("time.txt","at");
  fprintf(f,"Generate list time: %lf seconds\n",zeit);
  fclose(f);

  return 0;
}

void cPAnDA::SetDatabase(char *pth, char *dbName){
  //set the path
  strcpy(path,pth);

  //set the base filename
  strcpy(base,dbName);

  //database
  strcpy(database,pth);
  strcat(database,"\\");
  strcat(database,dbName);
  strcat(database,".sqlite");
}

void cPAnDA::SetParameters(float RT){
  RTCushion=RT;
}

bool cPAnDA::CreateDB(){

  logCounter=0;

  sqlite3 *db;
  char *errmsg;

  sqlite3_open(database,&db);
  sqlite3_exec(db,"create table File(FileID INTEGER PRIMARY KEY, FileName TEXT)",0,0,&errmsg);
  sqlite3_exec(db,"create table Kronik(KronikID INTEGER PRIMARY KEY, FileID INTEGER, FirstScan INTEGER, LastScan INTEGER, Charge INTEGER, MonoMass DOUBLE PRECISION, BIP DOUBLE PRECISION, Intensity REAL, FirstRT REAL, LastRT REAL)",0,0,&errmsg);
  sqlite3_exec(db,"create table MSMS(MsmsID INTEGER PRIMARY KEY, FileID INTEGER, ScanNum INTEGER, RTime REAL, PrecursorMZ DOUBLE PRECISION, Peptide TEXT)",0,0,&errmsg);
  sqlite3_exec(db,"commit",0,0,&errmsg);
  sqlite3_close(db);

  return true;
}

bool cPAnDA::AddKronik(char *hkFile){

  FILE *f;

  //Generate Kronik data from Hardklor results
  cKronik kro;
  kro.readHK(hkFile);
  kro.findPeps(NULL);
  
  sqlite3 *db;
  char *errmsg;
  char str[256];
  int i;
  char FID[16];
  int iRow,iCol;
  char **result;
  
  sqlite3_open(database,&db);

  //Add Kronik file to File Table
  sprintf(str,"insert into File('FileName') values ('%s')\0",kro.at(0).file);
  if(sqlite3_exec(db,str,0,0,&errmsg)!=SQLITE_OK) {
    f=fopen("error.txt","at");
    fprintf(f,"Error: %s\nLine: %s\n\n",errmsg,str);
    fclose(f);
  }
  sqlite3_exec(db,"commit",0,0,&errmsg);

  //Get FileID from File Table for this kronik data
  sprintf(str,"select FileID from File where FileName='%s'\0",kro.at(0).file);
  sqlite3_get_table(db,str, &result, &iRow, &iCol,0);
  strcpy(FID,result[1]);

  //Add Kronik results to Kronik Table
  for(i=0;i<kro.size();i++){
    DoEvents();
    sprintf(str,"insert into Kronik('FileID','FirstScan','LastScan','Charge','MonoMass','BIP','Intensity','FirstRT','LastRT') values (%s,%d,%d,%d,%lf,%lf,%f,%f,%f)\0",
      FID,kro.at(i).lowScan,kro.at(i).highScan,kro.at(i).charge,kro.at(i).monoMass,kro.at(i).basePeak,kro.at(i).intensity,kro.at(i).firstRTime,kro.at(i).lastRTime);
    if(sqlite3_exec(db,str,0,0,&errmsg)!=SQLITE_OK) {
      f=fopen("error.txt","at");
      fprintf(f,"Error: %s\nLine: %s\n\n",errmsg,str);  
      fclose(f);
    }
  }
  sqlite3_exec(db,"commit",0,0,&errmsg);
  sqlite3_close(db);

  return true;
}

bool cPAnDA::AddMS2(char *ms2File){

  FILE *f;

  //Generate Kronik data from Hardklor results
  MSReader r;
  Spectrum s;
  
  sqlite3 *db;
  char *errmsg;
  char str[256];
  char FID[16];
  int iRow,iCol;
  char **result;
  
  sqlite3_open(database,&db);

  //Add Kronik file to File Table
  //the command to begin a transaction?
  sqlite3_exec(db,"begin",0,0,&errmsg);
  sprintf(str,"insert into File('FileName') values ('%s')\0",ms2File);
  if(sqlite3_exec(db,str,0,0,&errmsg)!=SQLITE_OK) {
    f=fopen("error.txt","at");
    fprintf(f,"Error: %s\nLine: %s\n\n",errmsg,str);
    fclose(f);
  }
  sqlite3_exec(db,"commit",0,0,&errmsg);

  //Get FileID from File Table for this kronik data
  sprintf(str,"select FileID from File where FileName='%s'\0",ms2File);
  sqlite3_get_table(db,str, &result, &iRow, &iCol,0);
  strcpy(FID,result[1]);

  //Add Kronik results to Kronik Table
  r.setFilter(MS2);
  r.readFile(ms2File,s);
  //the command to begin a transaction?
  sqlite3_exec(db,"begin",0,0,&errmsg);
  while(s.getScanNumber()!=0){
    DoEvents();
    sprintf(str,"insert into MSMS('FileID','ScanNum','RTime','PrecursorMZ') values (%s,%d,%f,%lf)\0",
      FID,s.getScanNumber(),s.getRTime(),s.getMZ());
    if(sqlite3_exec(db,str,0,0,&errmsg)!=SQLITE_OK) {
      f=fopen("error.txt","at");
      fprintf(f,"Error: %s\nLine: %s\n\n",errmsg,str);  
      fclose(f);
    }
    r.readFile(NULL,s);
  }
  sqlite3_exec(db,"commit",0,0,&errmsg);
  sqlite3_close(db);

  return true;
}

bool cPAnDA::GenerateList(char *listName, int MinCount, int &TotPPID, int &MatchSize){
  sqlite3 *db;

  FILE *f;

  int rc;
  int i;
  int j;
  int k;
  int iRow, iCol;
  char **result;
  char str[256];

  double mzLower;
  double mzUpper;
  float RTLower,RTUpper;

  vector<PPID> ppid;
  PPID p;

  printf("Generating Mass List\n");

  //Get all redundant PPIDs
  sqlite3_open(database,&db);
  rc = sqlite3_get_table(db,"select Charge,MonoMass,BIP,Intensity,FirstRT,LastRT from Kronik order by MonoMass", &result, &iRow, &iCol,0);
  for(i=1;i<=iRow;i++){
    p.charge = atoi(result[i*iCol]);
    p.monoMass = atof(result[i*iCol+1]);
    p.BIP = atof(result[i*iCol+2]);
    p.intensity = atof(result[i*iCol+3]);
    p.firstRT = atof(result[i*iCol+4]);
    p.lastRT = atof(result[i*iCol+5]);
    ppid.push_back(p);
  }

  f=fopen("error.txt","at");
  fprintf(f,"Starting PPID size: %d\n",ppid.size());
  fclose(f);
  DoEvents();

  //Generate the unique PPID list
  //PPIDs are sorted by size, so once mzUpper is exceeded, go to next PPID.
  //When combining results, keep monoMass of most intense signal
  //This might only need one pass to solve now...
  j=0;
  while(j!=ppid.size()){
    f=fopen("error.txt","at");
    fprintf(f,"PPID: %d\n",ppid.size());
    fclose(f);
    j=ppid.size();
    for(i=0;i<ppid.size()-1;i++){
      DoEvents();
      mzLower=ppid[i].monoMass-(ppid[i].monoMass*0.00001); // +/- 10 ppm
      mzUpper=ppid[i].monoMass+(ppid[i].monoMass*0.00001);
      RTLower=ppid[i].firstRT-RTCushion;
      RTUpper=ppid[i].lastRT+RTCushion;
      for(k=i+1;k<ppid.size();k++){
        if(ppid[k].monoMass>=mzLower && ppid[k].monoMass<=mzUpper && ppid[k].charge==ppid[i].charge &&
          ((ppid[k].lastRT>=RTLower && ppid[k].lastRT<=RTUpper) || (ppid[k].firstRT>=RTLower && ppid[k].firstRT<=RTUpper)) ) {
          if(ppid[i].intensity>=ppid[k].intensity){
            if(ppid[k].lastRT>ppid[i].lastRT) ppid[i].lastRT=ppid[k].lastRT;
            if(ppid[k].firstRT<ppid[i].firstRT) ppid[i].firstRT=ppid[k].firstRT;
            ppid.erase(ppid.begin()+k);
            k--;
          } else {
            if(ppid[i].lastRT>ppid[k].lastRT) ppid[k].lastRT=ppid[i].lastRT;
            if(ppid[i].firstRT<ppid[k].firstRT) ppid[k].firstRT=ppid[i].firstRT;
            ppid.erase(ppid.begin()+i);
            i--;
            break;
          }
        } else if(ppid[k].monoMass>mzUpper){
          break;
        }
      }
    }
  }

  f=fopen("error.txt","at");
  fprintf(f,"Unique PPID size: %d\n",ppid.size());
  fclose(f);

  //Once we have a minimized PPID list, check that each PPID is seen in more than one analysis...
  /*
  for(i=0;i<ppid.size();i++){
    DoEvents();
    mzLower=ppid[i].monoMass-(ppid[i].monoMass*0.00001); // +/- 10 ppm
    mzUpper=ppid[i].monoMass+(ppid[i].monoMass*0.00001);
    sprintf(str,"SELECT DISTINCT FileID FROM Kronik WHERE MonoMass>=%lf AND MonoMass<=%lf AND ((FirstRT>=%f AND FirstRT<=%f) OR (LastRT>=%f AND LastRT<=%f)) AND Charge=%d\0",mzLower,mzUpper,ppid[i].firstRT,ppid[i].lastRT,ppid[i].firstRT,ppid[i].lastRT,ppid[i].charge);
    rc = sqlite3_get_table(db,str, &result, &iRow, &iCol,0);
    if(iRow<2){
      ppid.erase(ppid.begin()+i);
      i--;
    }
  }
  */

  TotPPID=ppid.size();
  MatchSize=0;

  if(TotPPID==0) return true;

  //Match each PPID to MS2 Table. If no hits, put it on the masslist
  //FILE *f;
  //FILE *log;
  //char strx[32];
  //sprintf(strx,"log%d.txt",logCounter++);
  //log=fopen(strx,"wt");
  f=fopen(listName,"wt");
  for(i=0;i<ppid.size();i++){
    //DoEvents();
    //Cannot use 10 ppm because of lack of precision in MS2 isolation mz
    mzLower=ppid[i].BIP-0.01; //(ppid[i].BIP*0.00001);
    mzUpper=ppid[i].BIP+0.01; //(ppid[i].BIP*0.00001);
    RTLower=ppid[i].firstRT-RTCushion;
    RTUpper=ppid[i].lastRT+RTCushion;
    sprintf(str,"SELECT RTime,PrecursorMZ FROM MSMS WHERE RTime>=%f AND RTime<=%f AND PrecursorMZ>=%lf AND PrecursorMZ<=%lf\0",RTLower,RTUpper,mzLower,mzUpper);
    rc = sqlite3_get_table(db,str, &result, &iRow, &iCol,0);
    //fprintf(log,"%.4lf\t%.4lf\t%.2f\t%.2f\t%d matches\n",ppid[i].monoMass,ppid[i].BIP,ppid[i].firstRT,ppid[i].lastRT,iRow);
    if(iRow<MinCount){
      fprintf(f,"%.2lf\t%.2f\t%.2f\n",ppid[i].BIP,ppid[i].firstRT,ppid[i].lastRT);
    } else {
      MatchSize++;
    }
  }
  fclose(f);
  //fclose(log);

  //Trim redundancies from masslist after writing to file
  //Redundancies defined as same mz (at 2 decimal places) and overlapping RT with 0.02 minute tolerance
  vList.clear();
  vector<MassList2> vList2;
  MassList2 ml;
  f=fopen(listName,"rt");
  while(!feof(f)){
    fscanf(f,"%s\t%f\t%f\n",ml.mz,&ml.firstRT,&ml.lastRT);
    vList2.push_back(ml);
  }
  fclose(f);
  k=0;
  while(k!=vList2.size()){
    k=vList2.size();
    for(i=0;i<vList2.size()-1;i++){
      for(j=i+1;j<vList2.size();j++){
        DoEvents();
        //convert masses to strings because of floating point error?
        if( strcmp(vList2[i].mz,vList2[j].mz)==0 && ((vList2[i].firstRT>=vList2[j].firstRT-0.02 && vList2[i].firstRT<=vList2[j].lastRT+0.02)
          || (vList2[i].lastRT>=vList2[j].firstRT-0.02 && vList2[i].lastRT<=vList2[j].lastRT+0.02)) ){
            if(vList2[j].firstRT<vList2[i].firstRT) vList2[i].firstRT=vList2[j].firstRT;
            if(vList2[j].lastRT>vList2[i].lastRT) vList2[i].lastRT=vList2[j].lastRT;
            vList2.erase(vList2.begin()+j);
            j--;
        } else if( strcmp(vList2[i].mz,vList2[j].mz)==0 && vList2[i].firstRT <= vList2[j].firstRT && vList2[i].lastRT>=vList2[j].lastRT){
          vList2.erase(vList2.begin()+j);
          j--;
        }
      }
    }
  }

  f=fopen(listName,"wt");
  for(i=0;i<vList2.size();i++){
    fprintf(f,"%s\t%.2f\t%.2f\n",vList2[i].mz,vList2[i].firstRT,vList2[i].lastRT);
  }
  fclose(f);

  return true;
}

bool cPAnDA::ReadDB(){
  sqlite3 *db;

  int rc;
  int i;
  int j;
  int iRow, iCol;
  char **result;
  char str[256];

  int iRow2, iCol2;
  char **result2;

  printf("Reading db\n");

  sqlite3_open(database,&db);
  rc = sqlite3_get_table(db,"select * from Kronik", &result, &iRow, &iCol,0);
  for(i=0;i<=iRow && i<10;i++){
    for(j=0;j<iCol;j++) printf("%s\t",result[i*iCol+j]);
    printf("\n");
  }

  printf("\n\n");

  rc = sqlite3_get_table(db,"select * from MSMS", &result, &iRow, &iCol,0);
  for(i=0;i<=iRow && i<10;i++){
    for(j=0;j<iCol;j++) printf("%s\t",result[i*iCol+j]);
    printf("\n");
  }

  printf("\n\n");
  
  rc = sqlite3_get_table(db,"select * from File", &result, &iRow, &iCol,0);
  for(i=0;i<=iRow;i++){
    for(j=0;j<iCol;j++) printf("%s\t",result[i*iCol+j]);
    printf("\n");
  }
  
  printf("\n\n");
  rc = sqlite3_get_table(db,"select * from Kronik", &result, &iRow, &iCol,0);
  int k;
  for(k=1;k<=iRow;k++){
    double mzMin=atof(result[k*iCol+6]);
    double mzMax=mzMin+(mzMin*0.000001);
    mzMin=mzMin-(mzMin*0.000001);
    float firstRT=atof(result[k*iCol+8])-0.25;
    float lastRT=atof(result[k*iCol+9])+0.25;
    sprintf(str,"SELECT RTime,PrecursorMZ FROM MSMS WHERE RTime>=%f AND RTime<=%f AND PrecursorMZ>=%lf AND PrecursorMZ<=%lf\0",firstRT,lastRT,mzMin,mzMax);
    rc = sqlite3_get_table(db,str, &result2, &iRow2, &iCol2,0);
    if(rc!=SQLITE_OK) printf("Error\n");
    printf("%d\n",k);
    for(i=0;i<=iRow2;i++){
      for(j=0;j<iCol2;j++) printf("%s\t",result2[i*iCol2+j]);
      printf("\n");
    }
  }

  sqlite3_close(db);

  return true;
}

void cPAnDA::DoEvents() {
	MSG msg;
  long sts;

 	do {
   	if (sts = PeekMessage(&msg, (HWND) NULL, 0, 0, PM_REMOVE)) {
 			TranslateMessage(&msg);
 			DispatchMessage(&msg);
		}
 	} while (sts);
}
