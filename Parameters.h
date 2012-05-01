#pragma once
#include "XMLreader.h"

typedef struct SharedParamList{
	char MethodTemplate[256];
	char SequenceTemplate[256];
	char DatabaseName[64];
	char MassAnalyzer[64];
  char StartDevice[64];
} SharedParamList;

typedef struct SingleParamList{
	int singleIterations;
	int singleMSMSCoverage;
	float singleRTWin;
} SingleParamList;

typedef struct HKParamList{
  char Analyzer[16];
  int Charge1;
  int Charge2;
  int Depth;
  int MaxPeps;
  int Resolution;
  float Corr;
  float WinSize;
  float SNRatio;
  float SNWin;
  BOOL SN;
} HKParamList;

typedef struct MScoutParamList{
	float rti;			//lower bound for retention time window
	float rtf;			//upper bound for retention time window
	float time;			//time in minutes for persistent signals
	double mz;			//approx m/z resolution of data;
	float sn;			//SnR threshold
	float order;		//order for intensity and S/N histograms
	bool skipzero;		//skip zeros when computing noise threshold
	bool compare;		//do you want to compare signals (set to 0 greyed out)
	bool centroid;		//does the input contain centroided data?
	bool histograms;	//generate intensity and S/N histograms?
	bool siglist;
}  MScoutParamList;

typedef struct FAIMScoutParamList{
	float cvi;			//lower bound for compensation voltage window
	float cvf;			//upper bound for compensation voltage window
	float cv;			//voltage for persistent signal
	double mz;			//approximate m/z resolution of instrument
	float order;		//binning for S/N and intensity histograms
	bool skipzero;		//skip zeros when computing noise threshold?
	bool centroid;		//is the data centroided already?
	bool siglist;		//generate a list of detected peptide signals?
}  FAIMScoutParamList;


class Parameters
{
public:
	Parameters(void);
	~Parameters(void);

	void SetSharedParams(char * n_MethodTemplate, char * n_SequenceTemplate, char * n_DatabaseName, char * n_MassAnalyzer, char *n_StartDevice);
	void SetSingleParams(int n_singleIterations, int n_singleMSMSCoverage, float n_singleRTWin);
	void SetHKParams(char * n_Analyzer, int n_Charge1, int n_Charge2, int n_Depth, int n_MaxPeps, int n_Resolution, float n_Corr, float n_WinSize, float n_SNRatio, float n_SNWin,
		BOOL SN);
	void SetMScoutParams(float n_rti, float n_rtf, float n_time, double n_mz, float n_sn, float n_order, bool n_skipzero, bool n_compare, bool n_centroid, bool n_histograms, bool n_siglist);
	void SetFAIMScoutParams(float n_cvi, float n_cvf, float n_cv, double n_mz, float n_order, bool n_skipzero, bool n_centroid, bool n_siglist);
	void SetAnalysisType(int type);
	bool WriteParameters(char * filename);
	bool ReadParameters(char * filename);

	bool GetSharedParams(SharedParamList &shared_params);
	bool GetSingleParams(SingleParamList &single_params);
	bool GetHKParams(HKParamList &HK_params);
	bool GetMScoutParams(MScoutParamList &MScout_params);
	bool GetFAIMScoutParams(FAIMScoutParamList &FAIMScout_params);

	bool GetSharedParams(char * n_MethodTemplate, char * n_SequenceTemplate, char * n_DatabaseName, char * n_MassAnalyzer, char * n_StartDevice);
	bool GetSingleParams(int &n_singleIterations, int &n_singleMSMSCoverage, float &n_singleRTWin);
	bool GetHKParams(char * n_Analyzer, int &n_Charge1, int &n_Charge2, int &n_Depth, int &n_MaxPeps, int &n_Resolution, float &n_Corr, float &n_WinSize, float &n_SNRatio, float &n_SNWin,
		BOOL &SN);
	bool GetMScoutParams(float &n_rti, float &n_rtf, float &n_time, double &n_mz, float &n_sn, float &n_order, bool &n_skipzero, bool &n_compare, bool &n_centroid, bool &n_histograms, bool &n_siglist);
	bool GetFAIMScoutParams(float &n_cvi, float &n_cvf, float &n_cv, double &n_mz, float &n_order, bool &n_skipzero, bool &n_centroid, bool &n_siglist);
	int GetAnalysisType();

	bool has_shared();
	bool has_single();
	bool has_HK();
	bool has_MScout();
	bool has_FAIMScout();


private:
	//Functions
	bool WriteAnalysisType(FILE * fout);
	bool WriteSharedParams(FILE * fout);
	bool WriteSingleParams(FILE * fout);
	bool WriteHKParams(FILE * fout);
	bool WriteMScoutParams(FILE * fout);
	bool WriteFAIMScoutParams(FILE * fout);

	//Variables
	bool v_has_shared;
	bool v_has_single;
	bool v_has_HK;
	bool v_has_MScout;
	bool v_has_FAIMScout;	
	int analysis_type;
	SharedParamList sharedParams;
	HKParamList hkParams;
	MScoutParamList mscoutParams;
	FAIMScoutParamList faimscoutParams;
	SingleParamList singleParams;
	XMLreader xmlreader;
};
