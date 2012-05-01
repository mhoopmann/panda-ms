#include "BaseAnalysis.h"

void CBaseAnalysis::SetParams(Parameters &paramObj){
  params=paramObj;
}

void CBaseAnalysis::SetParams(char *file){
  params.ReadParameters(file);
}