#include "cKronik.h"

using namespace std;

cKronik::cKronik(){
	allScans = new vector<sScan>;
	rPeps = new vector<realPep>;
	PPM=10;
	CONTAM=150;
	MAXMASS=8000;
	MINMASS=600;
};

cKronik::cKronik(const cKronik& p){
	unsigned int i;
	allScans = new vector<sScan>;
	rPeps = new vector<realPep>;
	for(i=0;i<p.allScans->size();i++) allScans->push_back(p.allScans->at(i));
	for(i=0;i<p.rPeps->size();i++) rPeps->push_back(p.rPeps->at(i));
	PPM=p.PPM;
	CONTAM=p.CONTAM;
	MAXMASS=p.MAXMASS;
	MINMASS=p.MINMASS;
};
	
cKronik& cKronik::operator=(const cKronik& p){
	unsigned int i;

	if(this!=&p){
		delete allScans;
		delete rPeps;

		allScans = new vector<sScan>;
		rPeps = new vector<realPep>;
		for(i=0;i<p.allScans->size();i++) allScans->push_back(p.allScans->at(i));
		for(i=0;i<p.rPeps->size();i++) rPeps->push_back(p.rPeps->at(i));
		PPM=p.PPM;
		CONTAM=p.CONTAM;
		MAXMASS=p.MAXMASS;
		MINMASS=p.MINMASS;
	};
	return *this;
};

cKronik::~cKronik(){
	delete allScans;
	delete rPeps;
};

realPep& cKronik::at(unsigned int i){
	return rPeps->at(i);
};

void cKronik::readHK(char *fn) {
	FILE *hkr;
	sScan scan;
	sPep pep;
	double td;
	char tag;
	bool firstScan;

	int pepCount=0;

	delete allScans;
	allScans=new vector<sScan>;

	firstScan=true;

	hkr = fopen(fn,"rt");
  if(hkr==NULL) {
    cout << "Problem reading file." << endl;
    return;
  };

	while(!feof(hkr)){
    
		tag=fgetc(hkr);

    if(tag=='S') {

			if(firstScan) {
				firstScan=false;
			}	else {
				allScans->push_back(scan);
				//cout << scan.vPep->size() << " vs " << allScans.at(allScans.size()-1).vPep->size() << endl;
			};
		
			scan.clear();
      fscanf(hkr,"\t%d\t%f%s\n",&scan.scanNum,&scan.rTime,scan.file);

		} else {

			pepCount++;
			fscanf(hkr,"\t%lf\t%d\t%f\t%lf\t%lf-%lf\t%lf\t%s\t%lf\n", &pep.monoMass,&pep.charge,&pep.intensity,&pep.basePeak,&td,&td,&td,pep.mods,&pep.xCorr);
			scan.vPep->push_back(pep);

		};
    
	};

	fclose(hkr);

	cout << "Total PIDs (redundant): " << pepCount << endl;

};

void cKronik::findPeps(char *fn){

	int i,j,k;

	FILE *out;

	vector<iTwo> hits;
	iTwo hit;

	realPep rp;

	int charge;
	int scanPoint;
	int match;
	int noMatch;
	int maxIndex;
	int totalMatch=0;

	//char mods[256];
	
	bool bMatch;
	bool bFound;

	double mMass;
	double avgMonoMass;

	float maxIntensity;
	float totalIntensity;

	delete rPeps;
	rPeps = new vector<realPep>;

  if(fn!=NULL){
	  out = fopen(fn,"wt");
	  if(out==NULL) {
		  cout << "Cannot write to " << fn << endl;
		  return;
	  }
  }

	//cout << allScans.size() << endl;
	//cout << allScans.at(3000).vPep->size() << endl;

	for(i=0;i<allScans->size();i++){

		//pick a peptide
		for(j=0;j<allScans->at(i).vPep->size();j++) {

			//cout << "Checking scan " << i << " of " << allScans.size() << "  pep: " << j << " of " << allScans.at(i).vPep->size() << endl;

			//get the monoMass
			mMass = allScans->at(i).vPep->at(j).monoMass;
			charge = allScans->at(i).vPep->at(j).charge;
			//strcpy(mods,allScans->at(i).vPep->at(j).mods);
			//cout << mMass << " +" << charge << endl;

			//Set up info
			hits.clear();
			hit.scanID=i;
			hit.pepID=j;
			hits.push_back(hit);

			scanPoint=i;
			match=0;
			noMatch=0;

			bFound=false;

			while(true){

				scanPoint++;

				//cout << scanPoint << ":" << allScans.size() << endl;
				if(scanPoint>=allScans->size()) break;

				//check next scan(s)
				bMatch=false;
				for(k=0;k<allScans->at(scanPoint).vPep->size();k++){
					if( fabs(mMass - allScans->at(scanPoint).vPep->at(k).monoMass)/mMass * 1000000 < PPM &&
						  charge == allScans->at(scanPoint).vPep->at(k).charge &&
							//Require same modifications
							strcmp(allScans->at(i).vPep->at(j).mods,allScans->at(scanPoint).vPep->at(k).mods)==0	
							) {

						//cout << mMass << " - " << allScans.at(scanPoint).vPep->at(k).monoMass << endl;
						//cout << fabs(mMass - allScans.at(scanPoint).vPep->at(k).monoMass)/mMass * 1000000 << endl;
						//match
					
						hit.scanID=scanPoint;
						hit.pepID=k;
						hits.push_back(hit);
						bMatch=true;
						//cout << "Match" << endl;
						break;
					};
				};

				//if match was found;
				if(bMatch) {
					match++;
					noMatch=0;
				} else {
					//no match, check to see if enough prior matches warrant keeping this one
					noMatch++;

					if(noMatch==2) {

						//don't keep bad matches
						if(match<2) {
							break;
						} else {

							//cout << "match: " << hits.size() << endl;
							totalMatch++;

							//Create stats
							avgMonoMass=0;
							maxIndex=0;
							maxIntensity=0;
							totalIntensity=0;
							for(k=0;k<hits.size();k++){
								avgMonoMass+=allScans->at(hits.at(k).scanID).vPep->at(hits.at(k).pepID).monoMass;
								if(allScans->at(hits.at(k).scanID).vPep->at(hits.at(k).pepID).intensity > maxIntensity){
									maxIntensity=allScans->at(hits.at(k).scanID).vPep->at(hits.at(k).pepID).intensity;
									maxIndex=k;
								};
								totalIntensity+=allScans->at(hits.at(k).scanID).vPep->at(hits.at(k).pepID).intensity;
							};

							avgMonoMass/=hits.size();

							//cout << "AVG ok" << endl;
							//cout << "maxIndex: " << maxIndex << endl;

							//output good matches
							strcpy(rp.file,allScans->at(i).file);
							rp.lowScan = allScans->at(hits.at(0).scanID).scanNum;
							rp.highScan = allScans->at(hits.at(hits.size()-1).scanID).scanNum;
							rp.scanCount = hits.size();
							rp.charge = charge;
              rp.monoMass = allScans->at(hits.at(maxIndex).scanID).vPep->at(hits.at(maxIndex).pepID).monoMass;
							rp.intensity = maxIntensity;
							rp.sumIntensity = totalIntensity;
							rp.firstRTime = allScans->at(hits.at(0).scanID).rTime;
							rp.lastRTime = allScans->at(hits.at(hits.size()-1).scanID).rTime;
							rp.rTime = allScans->at(hits.at(maxIndex).scanID).rTime;
							//cout << "Rtime ok" << endl;
							rp.xCorr = allScans->at(hits.at(maxIndex).scanID).vPep->at(hits.at(maxIndex).pepID).xCorr;
							//cout << "before mods" << endl;
							strcpy(rp.mods,allScans->at(hits.at(0).scanID).vPep->at(hits.at(0).pepID).mods);
							rp.basePeak=allScans->at(hits.at(maxIndex).scanID).vPep->at(hits.at(maxIndex).pepID).basePeak;
							rPeps->push_back(rp);
							//cout << "HIT: " << rp.monoMass << ", " << rp.charge << endl;
						
							//erase all hits so they are not re-analyzed;
							//erase from last to first otherwise indexes will be incorrect. -- Should not be a factor since only one pep in each
							//scan is removed.
							for(k=0;k<hits.size();k++){
								allScans->at(hits.at(k).scanID).vPep->erase(allScans->at(hits.at(k).scanID).vPep->begin()+hits.at(k).pepID);
							};

							//cout << "Erase ok" << endl;

							//decrement j because we erased it
							j--;
							bFound=true;
							break;

						};//end if(match<3);
						bFound=true;
						break;
					};//end if(noMatch==2);

				};//end if(bMatch);

			};//end while;

			//cout << "End while" << endl;

			if(!bFound){
				if(match<2) {
					//break;
				} else {

					totalMatch++;

					//Create stats
					avgMonoMass=0;
					maxIndex=0;
					maxIntensity=0;
					totalIntensity=0;
					for(k=0;k<hits.size();k++){
						avgMonoMass+=allScans->at(hits.at(k).scanID).vPep->at(hits.at(k).pepID).monoMass;
						if(allScans->at(hits.at(k).scanID).vPep->at(hits.at(k).pepID).intensity > maxIntensity){
							maxIntensity=allScans->at(hits.at(k).scanID).vPep->at(hits.at(k).pepID).intensity;
							maxIndex=k;
						};
						totalIntensity+=allScans->at(hits.at(k).scanID).vPep->at(hits.at(k).pepID).intensity;
					};

					avgMonoMass/=hits.size();

					//output good matches
					strcpy(rp.file,allScans->at(i).file);
					rp.lowScan = allScans->at(hits.at(0).scanID).scanNum;
					rp.highScan = allScans->at(hits.at(hits.size()-1).scanID).scanNum;
					rp.scanCount = hits.size();
					rp.charge = charge;
					rp.monoMass = allScans->at(hits.at(maxIndex).scanID).vPep->at(hits.at(maxIndex).pepID).monoMass;
					rp.intensity = maxIntensity;
					rp.sumIntensity = totalIntensity;
					rp.firstRTime = allScans->at(hits.at(0).scanID).rTime;
					rp.lastRTime = allScans->at(hits.at(hits.size()-1).scanID).rTime;
					rp.rTime = allScans->at(hits.at(maxIndex).scanID).rTime;
					rp.xCorr = allScans->at(hits.at(maxIndex).scanID).vPep->at(hits.at(maxIndex).pepID).xCorr;
					strcpy(rp.mods,allScans->at(hits.at(0).scanID).vPep->at(hits.at(0).pepID).mods);
					rp.basePeak=allScans->at(hits.at(maxIndex).scanID).vPep->at(hits.at(maxIndex).pepID).basePeak;
					rPeps->push_back(rp);
					//cout << "HIT: " << rp.monoMass << ", " << rp.charge << endl;
						
					//erase all hits so they are not re-analyzed;
					//erase from last to first otherwise indexes will be incorrect. -- Should not be a factor since only one pep in each
					//scan is removed.
					for(k=0;k<hits.size();k++){
						allScans->at(hits.at(k).scanID).vPep->erase(allScans->at(hits.at(k).scanID).vPep->begin()+hits.at(k).pepID);
					};
					

					//decrement j because we erased it
					j--;
				};
			};

			//cout << "Go to next j  " << j << endl;

		};//for j (all peptides)

	};//for i (all scans)

	cout << "Persistent PIDs: " << totalMatch << endl;

	//Remove contaminants (elements that repeat too frequently).
	//If same mass appears at same charge in > CONTAM scans, it is a contaminant;
	vector<int> matchList;
	int totalScans;
	for(i=0;i<rPeps->size()-1;i++){
		totalScans = rPeps->at(i).scanCount;
		matchList.clear();
		matchList.push_back(i);
		for(j=i+1;j<rPeps->size();j++){
			if( fabs(rPeps->at(i).monoMass-rPeps->at(j).monoMass)/rPeps->at(i).monoMass * 1000000 < PPM &&
					rPeps->at(i).charge == rPeps->at(j).charge) {
				totalScans+=rPeps->at(j).scanCount;
				matchList.push_back(j);
			};
		};
		if(totalScans>CONTAM){
			//erase contaminants from last to first
			for(j=matchList.size()-1;j>=0;j--){
				rPeps->erase( rPeps->begin() + matchList.at(j) );
			};
			//decrement i because we erased it.
			i--;
		};
	};

	
	cout << "After removing contaminants: " << rPeps->size() << endl;

	i=0;
	while(i<rPeps->size()){
		if(rPeps->at(i).monoMass<MINMASS || rPeps->at(i).monoMass>MAXMASS) rPeps->erase(rPeps->begin()+i);
		else i++;
	};

	cout << "From range " << MINMASS << " to " << MAXMASS << ": " << rPeps->size() << endl;

  if(fn!=NULL){
	  //Heading line
	  fprintf(out,"File\tFirst Scan\tLast Scan\tNum of Scans\tCharge\tMonoisotopic Mass\tBase Isotope Peak\t");
	  fprintf(out,"Best Intensity\tSummed Intensity\tFirst RTime\tLast RTime\tBest RTime\tBest Correlation\tModifications\n");

	  for(i=0;i<rPeps->size();i++){
		  fprintf(out,"%s\t%d\t%d\t%d\t%d\t%lf\t%lf\t%f\t%f\t%f\t%f\t%f\t%lf\t%s\n",rPeps->at(i).file,
																																		 rPeps->at(i).lowScan,
																																		 rPeps->at(i).highScan,
																																		 rPeps->at(i).scanCount,
																																		 rPeps->at(i).charge,
																																		 rPeps->at(i).monoMass,
																																		 rPeps->at(i).basePeak,
																																		 rPeps->at(i).intensity,
																																		 rPeps->at(i).sumIntensity,
																																		 rPeps->at(i).firstRTime,
																																		 rPeps->at(i).lastRTime,
																																		 rPeps->at(i).rTime,
																																		 rPeps->at(i).xCorr,
																																		 rPeps->at(i).mods);
	  };
  
	  fclose(out);
  }

}

int cKronik::size(){
	return rPeps->size();
};

void cKronik::setPPM(int i){
	PPM=i;
};

void cKronik::setCONTAM(int i){
	CONTAM=i;
};

void cKronik::setMAXMASS(int i){
	MAXMASS=i;
};

void cKronik::setMINMASS(int i){
	MINMASS=i;
};

int cKronik::getPPM(){
	return PPM;
};

int cKronik::getCONTAM(){
	return CONTAM;
};

int cKronik::getMAXMASS(){
	return MAXMASS;
};

int cKronik::getMINMASS(){
	return MINMASS;
};