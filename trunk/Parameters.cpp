#include "StdAfx.h"
#include "Parameters.h"

Parameters::Parameters(void)
{
	//Change these to false eventually, true is for debugging purposes -JE
	v_has_shared = false;
	v_has_HK = false;
	v_has_MScout = false;
	v_has_FAIMScout = false;
	v_has_single = false;
	analysis_type = 0;
	//default shared parameters
	strcpy(sharedParams.MethodTemplate, "");
	strcpy(sharedParams.SequenceTemplate, "");
	strcpy(sharedParams.DatabaseName, "PAnDA");
	strcpy(sharedParams.MassAnalyzer, "");
  strcpy(sharedParams.StartDevice, "");

	//default single analysis parameters
	singleParams.singleIterations = 2;
	singleParams.singleMSMSCoverage = 1;
	singleParams.singleRTWin = 0;

	//default Hardklor parameters
	strcpy(hkParams.Analyzer, "FTICR");
	hkParams.Charge1= 1;
	hkParams.Charge2= 5;
	hkParams.Depth = 3;
	hkParams.MaxPeps = 12;
	hkParams.Resolution = 50000;
	hkParams.Corr=0.90;
	hkParams.SNRatio=1.0;
	hkParams.SNWin = 250;
	hkParams.SN = false;
  hkParams.WinSize = 4.0;

	//default Mscout Parameters
	mscoutParams.rti = 0.0;
	mscoutParams.rtf = 0.0;
	mscoutParams.time = 0.2;
	mscoutParams.mz = 0.5;
	mscoutParams.sn = 3.0;
	mscoutParams.order = 0.01;
	mscoutParams.skipzero = true;
	mscoutParams.compare = false;
	mscoutParams.centroid = false;
	mscoutParams.histograms = false;
	mscoutParams.siglist = true;

	//default FAIMScout parameters
	faimscoutParams.cvi = 0.0;
	faimscoutParams.cvf = 0.0;
	faimscoutParams.cv = 4.0;
	faimscoutParams.mz = 0.5;
	faimscoutParams.order = 0.01;
	faimscoutParams.skipzero = true;
	faimscoutParams.centroid = false;
	faimscoutParams.siglist = true;
}

Parameters::~Parameters(void)
{
}


void Parameters::SetSharedParams(char * n_MethodTemplate, char * n_SequenceTemplate, char * n_DatabaseName, char * n_MassAnalyzer, char * n_StartDevice)
{
	v_has_shared = true;
	strcpy(sharedParams.MethodTemplate, n_MethodTemplate);
	strcpy(sharedParams.SequenceTemplate, n_SequenceTemplate);
	strcpy(sharedParams.DatabaseName, n_DatabaseName);
	strcpy(sharedParams.MassAnalyzer, n_MassAnalyzer);
  strcpy(sharedParams.StartDevice, n_StartDevice);
}

void Parameters::SetSingleParams(int n_singleIterations, int n_singleMSMSCoverage, float n_singleRTWin)
{
	v_has_single = true;
	singleParams.singleIterations = n_singleIterations;
	singleParams.singleMSMSCoverage = n_singleMSMSCoverage;
	singleParams.singleRTWin = n_singleRTWin;
}


void Parameters::SetHKParams(char * n_Analyzer, int n_Charge1, int n_Charge2, int n_Depth, int n_MaxPeps, 
							 int n_Resolution, float n_Corr, float n_WinSize, float n_SNRatio, float n_SNWin, 
							 BOOL n_SN)
{
	v_has_HK = true;
	strcpy(hkParams.Analyzer, n_Analyzer);
	hkParams.Charge1 = n_Charge1;
	hkParams.Charge2 = n_Charge2;
	hkParams.Depth = n_Depth;
	hkParams.MaxPeps = n_MaxPeps;
	hkParams.Resolution = n_Resolution;
	hkParams.Corr = n_Corr;
	hkParams.WinSize = n_WinSize;
	hkParams.SNRatio = n_SNRatio;
	hkParams.SNWin = n_SNWin;
	hkParams.SN = n_SN;
}

void Parameters::SetMScoutParams(float n_rti, float n_rtf, float n_time, double n_mz, float n_sn, float n_order, bool n_skipzero, bool n_compare, bool n_centroid, bool n_histograms, bool n_siglist)
{
	v_has_MScout = true;
	mscoutParams.rti = n_rti;
	mscoutParams.rtf = n_rtf;
	mscoutParams.time = n_time;
	mscoutParams.mz = n_mz;
	mscoutParams.sn = n_sn;
	mscoutParams.order = n_order;
	mscoutParams.skipzero = n_skipzero;
	mscoutParams.compare = n_compare;
	mscoutParams.centroid = n_centroid;
	mscoutParams.histograms = n_histograms;
	mscoutParams.siglist = n_siglist;
}

void Parameters::SetFAIMScoutParams(float n_cvi, float n_cvf, float n_cv, double n_mz, float n_order, bool n_skipzero, bool n_centroid, bool n_siglist)
{
	v_has_FAIMScout=true;
	faimscoutParams.cvi = n_cvi;
	faimscoutParams.cvf = n_cvf;
	faimscoutParams.cv = n_cv;
	faimscoutParams.mz = n_mz;
	faimscoutParams.order = n_order;
	faimscoutParams.skipzero = n_skipzero;
	faimscoutParams.centroid = n_centroid;
	faimscoutParams.siglist = n_siglist;
}

void Parameters::SetAnalysisType(int type){
	analysis_type = type;
}

bool Parameters::WriteParameters(char * filename)
{
	FILE * fout;
	fout = fopen(filename, "wt");
	WriteAnalysisType(fout);
	if (v_has_shared){
		WriteSharedParams(fout);
	}
	if (v_has_single){
		WriteSingleParams(fout);
	}
	if (v_has_HK){
		WriteHKParams(fout);
	}
	if (v_has_MScout){
		WriteMScoutParams(fout);
	}
	if (v_has_FAIMScout){
		WriteFAIMScoutParams(fout);
	}
	fclose(fout);

	return true;
}

bool Parameters::ReadParameters(char * filename)
{
	Element temp_element;
	xmlreader.OpenXML(filename);
	while(xmlreader.ReadElement(temp_element)){
		if(strcmp(temp_element.name, "analysis")==0){
			analysis_type = atoi(temp_element.attributes[CStringA("analysistype")]);
		}
		if(strcmp(temp_element.name, "shared_params")==0){
			v_has_shared = true;
			strcpy(sharedParams.DatabaseName, temp_element.attributes[CStringA("databasename")]);
			strcpy(sharedParams.MassAnalyzer, temp_element.attributes[CStringA("massanalyzer")]);
			strcpy(sharedParams.MethodTemplate, temp_element.attributes[CStringA("methodtemplate")]);
			strcpy(sharedParams.SequenceTemplate, temp_element.attributes[CStringA("sequencetemplate")]);
      strcpy(sharedParams.StartDevice, temp_element.attributes[CStringA("startdevice")]);
		}
		else if(strcmp(temp_element.name, "single_params")==0){
			v_has_single = true;
			singleParams.singleIterations = atoi(temp_element.attributes[CStringA("singleiterations")]);
			singleParams.singleMSMSCoverage = atoi(temp_element.attributes[CStringA("singlemsmscoverage")]);
			singleParams.singleRTWin = atof(temp_element.attributes[CStringA("singlertwin")]);
		}
		else if(strcmp(temp_element.name, "HK_params")==0){
			v_has_HK = true;
			strcpy(hkParams.Analyzer, temp_element.attributes[CStringA("analyzer")]);
			hkParams.Charge1 = atoi(temp_element.attributes[CStringA("charge1")]);
			hkParams.Charge2 = atoi(temp_element.attributes[CStringA("charge2")]);
			hkParams.Corr = atof(temp_element.attributes[CStringA("corr")]);
			hkParams.Depth = atoi(temp_element.attributes[CStringA("depth")]);
			hkParams.MaxPeps = atoi(temp_element.attributes[CStringA("maxpeps")]);
			hkParams.Resolution = atoi(temp_element.attributes[CStringA("resolution")]);
			hkParams.SNRatio = atof(temp_element.attributes[CStringA("snratio")]);
			hkParams.SNWin = atof(temp_element.attributes[CStringA("snwin")]);
			hkParams.WinSize = atof(temp_element.attributes[CStringA("winsize")]);
			hkParams.SN = (temp_element.attributes[CStringA("sn")] == "true");
		}
		else if(strcmp(temp_element.name, "MScout_params")==0){
			v_has_MScout = true;
			mscoutParams.centroid = (temp_element.attributes[CStringA("centroid")] == "true");
			mscoutParams.compare = (temp_element.attributes[CStringA("compare")] == "true");
			mscoutParams.histograms = (temp_element.attributes[CStringA("histograms")] == "true");
			mscoutParams.mz = atof(temp_element.attributes[CStringA("mz")]);
			mscoutParams.order = atof(temp_element.attributes[CStringA("order")]);
			mscoutParams.rtf = atof(temp_element.attributes[CStringA("rtf")]);
			mscoutParams.rti = atof(temp_element.attributes[CStringA("rti")]);
			mscoutParams.siglist = (temp_element.attributes[CStringA("siglist")] == "true");
			mscoutParams.skipzero = (temp_element.attributes[CStringA("skipzero")] == "true");
			mscoutParams.sn = atof(temp_element.attributes[CStringA("sn")]);
			mscoutParams.time = atof(temp_element.attributes[CStringA("time")]);
		}
		else if(strcmp(temp_element.name, "FAIMScout_params")==0){
			v_has_FAIMScout = true;
			faimscoutParams.centroid = (temp_element.attributes[CStringA("centroid")] == "true");
			faimscoutParams.cv = atof(temp_element.attributes[CStringA("cv")]);
			faimscoutParams.cvf = atof(temp_element.attributes[CStringA("cvf")]);
			faimscoutParams.cvi = atof(temp_element.attributes[CStringA("cvi")]);
			faimscoutParams.mz = atof(temp_element.attributes[CStringA("mz")]);
			faimscoutParams.order = atof(temp_element.attributes[CStringA("order")]);
			faimscoutParams.siglist = (temp_element.attributes[CStringA("siglist")] == "true");
			faimscoutParams.skipzero = (temp_element.attributes[CStringA("skipzero")] == "true");
		}
		temp_element.Clear();
	}
	xmlreader.CloseXML();
	return true;
}

bool Parameters::GetSharedParams(SharedParamList &shared_params)
{
	if(!v_has_shared){
		return false;
	}
	else{
		shared_params = sharedParams;
	}
	return true;
}

bool Parameters::GetSingleParams(SingleParamList &single_params)
{
	if(!v_has_single){
		return false;
	}
	else{
		single_params = singleParams;
	}
	return true;
}

bool Parameters::GetHKParams(HKParamList &HK_params)
{
	if(!v_has_HK){
		return false;
	}
	else{
		HK_params = hkParams;
	}
	return true;
}

bool Parameters::GetMScoutParams(MScoutParamList &MScout_params)
{
	if(!v_has_MScout){
		return false;
	}
	else{
		MScout_params = mscoutParams;	
	}
	return true;
}

bool Parameters::GetFAIMScoutParams(FAIMScoutParamList &FAIMScout_params)
{
	if(!v_has_FAIMScout){
		return false;
	}
	else{
		FAIMScout_params = faimscoutParams;
	}
	return true;
}

bool Parameters::GetSharedParams(char * n_MethodTemplate, char * n_SequenceTemplate, char * n_DatabaseName, char * n_MassAnalyzer, char * n_StartDevice)
{
	if(!v_has_shared){
		return false;
	}
	else{
		strcpy(n_MethodTemplate, sharedParams.MethodTemplate);
		strcpy(n_SequenceTemplate, sharedParams.SequenceTemplate);
		strcpy(n_DatabaseName, sharedParams.DatabaseName);
		strcpy(n_MassAnalyzer, sharedParams.MassAnalyzer);
    strcpy(n_StartDevice, sharedParams.StartDevice);
	}
	return true;
}

bool Parameters::GetSingleParams(int &n_singleIterations, int &n_singleMSMSCoverage, float &n_singleRTWin)
{
	if(!v_has_single){
		return false;
	}
	else{
		n_singleIterations = singleParams.singleIterations;
		n_singleMSMSCoverage = singleParams.singleMSMSCoverage;
		n_singleRTWin = singleParams.singleRTWin;
		return true;
	}
}

bool Parameters::GetHKParams(char * n_Analyzer, int &n_Charge1, int &n_Charge2, int &n_Depth, int &n_MaxPeps, 
							 int &n_Resolution, float &n_Corr, float &n_WinSize, float &n_SNRatio, float &n_SNWin, 
							 BOOL &n_SN)
{
	if(!v_has_HK){
		return false;
	}
	else{
		strcpy(n_Analyzer, hkParams.Analyzer);
		n_Charge1 = hkParams.Charge1;
		n_Charge2 = hkParams.Charge2;
		n_Depth = hkParams.Depth;
		n_MaxPeps = hkParams.MaxPeps;
		n_Resolution = hkParams.Resolution;
		n_Corr = hkParams.Corr;
		n_WinSize = hkParams.WinSize;
		n_SNRatio = hkParams.SNRatio;
		n_SNWin = hkParams.SNWin;
		n_SN = hkParams.SN;
	}
	return true;
}

bool Parameters::GetMScoutParams(float &n_rti, float &n_rtf, float &n_time, double &n_mz, float &n_sn, float &n_order, bool &n_skipzero, bool &n_compare, bool &n_centroid, bool &n_histograms, bool &n_siglist)
{
	if(!v_has_MScout){
		return false;
	}
	else{
		n_rti = mscoutParams.rti;
		n_rtf = mscoutParams.rtf;
		n_time = mscoutParams.time;
		n_mz = mscoutParams.mz;
		n_sn = mscoutParams.sn;
		n_order = mscoutParams.order;
		n_skipzero = mscoutParams.skipzero;
		n_compare = mscoutParams.compare;
		n_centroid = mscoutParams.centroid;
		n_histograms = mscoutParams.histograms;
		n_siglist = mscoutParams.siglist;
	}
	return true;
}
bool Parameters::GetFAIMScoutParams(float &n_cvi, float &n_cvf, float &n_cv, double &n_mz, float &n_order, bool &n_skipzero, bool &n_centroid, bool &n_siglist)
{
	if(!v_has_FAIMScout){
		return false;
	}
	else{
		n_cvi = faimscoutParams.cvi;
		n_cvf = faimscoutParams.cvf;
		n_cv = faimscoutParams.cv;
		n_mz = faimscoutParams.mz;
		n_order = faimscoutParams.order;
		n_skipzero = faimscoutParams.skipzero;
		n_centroid = faimscoutParams.centroid;
		n_siglist = faimscoutParams.siglist;
	}
	return true;
}

int Parameters::GetAnalysisType()
{
	return analysis_type;
}

bool Parameters::has_shared()
{
	return v_has_shared;
}

bool Parameters::has_single()
{
	return v_has_single;
}

bool Parameters::has_HK()
{
	return v_has_HK;
}

bool Parameters::has_MScout()
{
	return v_has_MScout;
}

bool Parameters::has_FAIMScout()
{
	return v_has_FAIMScout;
}

bool Parameters::WriteAnalysisType(FILE * fout){
	Element temp;
	strcpy(temp.name, "analysis");
	temp.length = 1;
	char tempc[8];
	sprintf(tempc, "%d", analysis_type);
	temp.attributes.insert(make_pair(CStringA("analysistype"), CStringA(tempc)));
	xmlreader.WriteElement(temp, fout);
	
	return true;
}

bool Parameters::WriteSharedParams(FILE * fout)
{	
	Element temp;
	strcpy(temp.name, "shared_params");
	temp.length = 4;
	temp.attributes.insert(make_pair(CStringA("databasename"), CStringA(sharedParams.DatabaseName)));
	temp.attributes.insert(make_pair(CStringA("massanalyzer"), CStringA(sharedParams.MassAnalyzer)));
	temp.attributes.insert(make_pair(CStringA("methodtemplate"), CStringA(sharedParams.MethodTemplate)));
	temp.attributes.insert(make_pair(CStringA("sequencetemplate"), CStringA(sharedParams.SequenceTemplate)));
  temp.attributes.insert(make_pair(CStringA("startdevice"),CStringA(sharedParams.StartDevice)));
	xmlreader.WriteElement(temp, fout);
	
	return true;
}

bool Parameters::WriteSingleParams(FILE * fout)
{
	Element temp;
	strcpy(temp.name, "single_params");
	temp.length = 3;
	char tempc[32];

	sprintf(tempc, "%d", singleParams.singleIterations);
	temp.attributes.insert(make_pair(CStringA("singleiterations"), CStringA(tempc)));
	sprintf(tempc, "%d", singleParams.singleMSMSCoverage);
	temp.attributes.insert(make_pair(CStringA("singlemsmscoverage"), CStringA(tempc)));
	sprintf(tempc, "%lf", singleParams.singleRTWin);
	temp.attributes.insert(make_pair(CStringA("singlertwin"), CStringA(tempc)));

	xmlreader.WriteElement(temp, fout);
	return true;
}

bool Parameters::WriteHKParams(FILE * fout){
	Element temp;
	strcpy(temp.name, "HK_params");
	temp.length = 11;
	char tempc[32];
	temp.attributes.insert(make_pair(CStringA("analyzer"), CStringA(hkParams.Analyzer)));
	sprintf(tempc, "%d", hkParams.Charge1);
	temp.attributes.insert(make_pair(CStringA("charge1"), CStringA(tempc)));
	sprintf(tempc, "%d", hkParams.Charge2);
	temp.attributes.insert(make_pair(CStringA("charge2"), CStringA(tempc)));
	sprintf(tempc, "%d", hkParams.Depth);
	temp.attributes.insert(make_pair(CStringA("depth"), CStringA(tempc)));
	sprintf(tempc, "%d", hkParams.MaxPeps);
	temp.attributes.insert(make_pair(CStringA("maxpeps"), CStringA(tempc)));
	sprintf(tempc, "%d", hkParams.Resolution);
	temp.attributes.insert(make_pair(CStringA("resolution"), CStringA(tempc)));
	sprintf(tempc, "%lf", hkParams.Corr);
	temp.attributes.insert(make_pair(CStringA("corr"), CStringA(tempc)));
	sprintf(tempc, "%lf", hkParams.WinSize);
	temp.attributes.insert(make_pair(CStringA("winsize"), CStringA(tempc)));
	sprintf(tempc, "%lf", hkParams.SNRatio);
	temp.attributes.insert(make_pair(CStringA("snratio"), CStringA(tempc)));
	sprintf(tempc, "%lf", hkParams.SNWin);
	temp.attributes.insert(make_pair(CStringA("snwin"), CStringA(tempc)));
	if (hkParams.SN){
		temp.attributes.insert(make_pair(CStringA("sn"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("sn"), CStringA("false")));
	}
	
	xmlreader.WriteElement(temp, fout);
	return true;
}
bool Parameters::WriteMScoutParams(FILE * fout){
	Element temp;
	strcpy(temp.name, "MScout_params");
	temp.length = 11;
	char tempc[32];

	sprintf(tempc, "%lf", mscoutParams.rti);
	temp.attributes.insert(make_pair(CStringA("rti"), CStringA(tempc)));
	sprintf(tempc, "%lf", mscoutParams.rtf);
	temp.attributes.insert(make_pair(CStringA("rtf"), CStringA(tempc)));
	sprintf(tempc, "%lf", mscoutParams.time);
	temp.attributes.insert(make_pair(CStringA("time"), CStringA(tempc)));
	sprintf(tempc, "%lf", mscoutParams.mz);
	temp.attributes.insert(make_pair(CStringA("mz"), CStringA(tempc)));
	sprintf(tempc, "%lf", mscoutParams.sn);
	temp.attributes.insert(make_pair(CStringA("sn"), CStringA(tempc)));
	sprintf(tempc, "%lf", mscoutParams.order);
	temp.attributes.insert(make_pair(CStringA("order"), CStringA(tempc)));
	if (mscoutParams.skipzero){
		temp.attributes.insert(make_pair(CStringA("skipzero"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("skipzero"), CStringA("false")));
	}
	if (mscoutParams.compare){
		temp.attributes.insert(make_pair(CStringA("compare"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("compare"), CStringA("false")));
	}
	if (mscoutParams.centroid){
		temp.attributes.insert(make_pair(CStringA("centroid"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("centroid"), CStringA("false")));
	}
	if (mscoutParams.histograms){
		temp.attributes.insert(make_pair(CStringA("histograms"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("histograms"), CStringA("false")));
	}
	if (mscoutParams.siglist){
		temp.attributes.insert(make_pair(CStringA("siglist"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("siglist"), CStringA("false")));
	}
	
	xmlreader.WriteElement(temp, fout);
	return true;
}
bool Parameters::WriteFAIMScoutParams(FILE * fout){
	Element temp;
	strcpy(temp.name, "FAIMScout_params");
	temp.length = 9;
	char tempc[32];

	sprintf(tempc, "%lf", faimscoutParams.cvi);
	temp.attributes.insert(make_pair(CStringA("cvi"), CStringA(tempc)));
	sprintf(tempc, "%lf", faimscoutParams.cvf);
	temp.attributes.insert(make_pair(CStringA("cvf"), CStringA(tempc)));
	sprintf(tempc, "%lf", faimscoutParams.cv);
	temp.attributes.insert(make_pair(CStringA("cv"), CStringA(tempc)));
	sprintf(tempc, "%lf", faimscoutParams.mz);
	temp.attributes.insert(make_pair(CStringA("mz"), CStringA(tempc)));
	sprintf(tempc, "%lf", faimscoutParams.order);
	temp.attributes.insert(make_pair(CStringA("order"), CStringA(tempc)));

	if(faimscoutParams.skipzero){
		temp.attributes.insert(make_pair(CStringA("skipzero"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("skipzero"), CStringA("false")));
	}

	if(faimscoutParams.centroid){
		temp.attributes.insert(make_pair(CStringA("centroid"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("centroid"), CStringA("false")));
	}

	if(faimscoutParams.siglist){
		temp.attributes.insert(make_pair(CStringA("siglist"), CStringA("true")));
	}
	else{
		temp.attributes.insert(make_pair(CStringA("siglist"), CStringA("false")));
	}

	xmlreader.WriteElement(temp, fout);
	return true;
}
