#ifndef _BASE_ANALYSIS_H
#define _BASE_ANALYSIS_H

#include "Parameters.h"
#include "PAnDA.h"
#include "stdafx.h"

class CBaseAnalysis {
private:
protected:
  Parameters params;
  cPAnDA pandaObj;
  
public:
  virtual void StartRun() {return;}; 
  virtual void RunEnded() {return;};
  
  void SetParams(char *file);
  void SetParams(Parameters &paramObj);

};

#endif