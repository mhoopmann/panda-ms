//#pragma once
#ifndef _PANDA_H
#define _PANDA_H

#include "stdafx.h"
#include "sqlite3.h"
#include "cKronik.h"
#include "MSReader.h"
#include "Spectrum.h"
#include <vector>

using namespace std;

typedef struct PPID{
  int charge;
  float firstRT;
  float lastRT;
  float intensity;
  double monoMass;
  double BIP;
} PPID;

typedef struct MassList{
  float firstRT;
  float lastRT;
  double mz;
} MassList;

typedef struct MassList2{
  float firstRT;
  float lastRT;
  char mz[8];
} MassList2;

class cPAnDA{
public:
  cPAnDA();
  vector<MassList> vList;

  bool SingleAnalysis(int iteration, int MinCount, int &TotPPID, int &MatchSize);
  void SetDatabase(char *pth, char *dbName);
  void SetParameters(float RT);

protected:
private:

  char path[256];
  char base[256];
  char database[256];

  int logCounter;
  float RTCushion; //in minutes;

  bool AddKronik(char *hkFile);
  bool AddMS2(char *ms2File);
  bool CreateDB();
  bool GenerateList(char *listName, int MinCount, int &TotPPID, int &MatchSize);
  bool ReadDB();

  void DoEvents();
};

#endif