#ifndef _CKRONIK_H
#define _CKRONIK_H

#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

//const int PPM=10;
//const int CONTAM=150;
//const int MAXMASS=8000;
//const int MINMASS=700;
//const int MD=12;

typedef struct sPep{
  int charge;
  double monoMass;
	double basePeak;
  float intensity;
	char formula[64];
	char mods[256];
	double xCorr;
} sPep;

typedef struct sScan {
  vector<sPep> *vPep;
	int scanNum;
	char file[256];
	float rTime;

	//Constructors & otherwise
	sScan(){vPep = new vector<sPep>;};
	sScan(const sScan& s){
		vPep = new vector<sPep>;
		for(unsigned int i=0;i<s.vPep->size();i++) vPep->push_back(s.vPep->at(i));
		scanNum = s.scanNum;
		strcpy(file,s.file);
		rTime=s.rTime;
	};
	~sScan(){delete vPep;};


	sScan& operator=(const sScan& s){
		if(this!=&s){
			delete vPep;
			vPep = new vector<sPep>;
			for(unsigned int i=0;i<s.vPep->size();i++) vPep->push_back(s.vPep->at(i));
			scanNum = s.scanNum;
			strcpy(file,s.file);
			rTime=s.rTime;
		};
		return *this;
	};

	void clear(){
		delete vPep;
		vPep = new vector<sPep>;
	};
} sScan;


typedef struct realPep {
  char file[256];
  int lowScan;
  int highScan;
  int scanCount;
  int charge;
  double monoMass;
	double basePeak;
  float intensity;
  float sumIntensity;
  float rTime;
  float firstRTime;
  float lastRTime;
  double xCorr;
	char mods[256];
} realPep;

typedef struct iTwo {
	int scanID;
	int pepID;
} iTwo;

class cKronik {
private:
	vector<sScan>* allScans;
	vector<realPep>* rPeps;

	//User modifiable flags
	int PPM;	//mass accuracy threshold
  int CONTAM;  //contamination threshold
  int MAXMASS;  //maximum mass
  int MINMASS;  //minimum mass

protected:
public:

	cKronik();
	cKronik(const cKronik& p);
	~cKronik();

	cKronik& operator=(const cKronik& p);

	realPep& at(unsigned int i);
	void readHK(char *fn);
	void findPeps(char *fn);
	int size();

	int getPPM();
	int getCONTAM();
	int getMAXMASS();
	int getMINMASS();
	void setPPM(int i);
	void setCONTAM(int i);
	void setMAXMASS(int i);
	void setMINMASS(int i);


};

#endif