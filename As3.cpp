#include <iostream>
#include <cmath>
#include "As3Pars.h"
#include "Sorts.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TAxis.h"
#include "TProfile.h"
#include "TF1.h"
#include "TCanvas.h"

using namespace std;


void bruteComp();
void compDist(Parameters pars, RootObjs objs);
void compFunc(Parameters pars, RootObjs objs);
TH1F* setHist(Parameters pars, TH1F* hist);
TProfile* setProf(Parameters pars, TProfile* prof);
TProfile* fitProf(Parameters pars, TProfile* prof);

vector<float> GenerateVec(int n);


void As3() {
	Parameters pars;
	RootObjs objs;
	compDist(pars, objs);
//	compFunc(pars, objs);
//	bruteComp();

}


void compDist(Parameters pars, RootObjs objs) {
	objs.compHist = setHist(pars, objs.compHist);

	int bubbleComps;
	vector<float> bubbleSorted;
	for(int i=0; i<pars.distIters; i++){
		vector<float> randList = GenerateVec(pars.distSize);
		tie(bubbleComps, bubbleSorted) = bubbleSort(randList);
		objs.compHist->Fill(bubbleComps);
	}
	TCanvas* distCan = new TCanvas("distCan", "Bubble Distribution Canvas");
	objs.compHist->Draw();
}


void compFunc(Parameters pars, RootObjs objs) {
	objs.compProf = setProf(pars, objs.compProf);

	int bubbleComps;
	vector<float> bubbleSorted;
	for(int n=pars.minN; n<pars.maxN; n+=pars.nStepSize){
		for(int i=0; i<pars.iters; i++){
			vector<float> randList = GenerateVec(n);
			tie(bubbleComps, bubbleSorted) = bubbleSort(randList);
			objs.compProf->Fill(n, bubbleComps);
		}
	}

	objs.compProf = fitProf(pars, objs.compProf);

	TCanvas* funcCan = new TCanvas("funcCan", "Bubble Function Canvas");
	objs.compProf->Draw();
}


void bruteComp(){
	Parameters pars;

	int bruteComps, bubbleComps;
	vector<float> bruteSorted, bubbleSorted;
	for(int n=pars.minN; n<pars.maxN; n+=pars.nStepSize){
		for(int i=0; i<pars.iters; i++){
			vector<float> randList = GenerateVec(n);
			tie(bruteComps, bruteSorted) = bruteSort(randList);
			for(int j=0; j<bruteSorted.size(); j++){
				cout << bruteSorted[j] << " | ";
			}
			cout << " - " << bruteComps << endl;
			tie(bubbleComps, bubbleSorted) = bubbleSort(randList);
			for(int j=0; j<bubbleSorted.size(); j++){
				cout << bubbleSorted[j] << " | ";
			}
			cout << " - " << bubbleComps << endl;
		}
	}

}


vector<float> GenerateVec(int n) {
	vector<float> vec(n);
	TRandom3 r = TRandom3(0);

	for(int i=0; i<vec.size(); i++) {
		vec[i] = r.Rndm();
	}

	return(vec);
}


TH1F* setHist(Parameters pars, TH1F* hist) {
	TAxis* x = hist->GetXaxis();
	Double_t min = x->GetXmin();
	Double_t max = pow(pars.distSize, 2);
	x->Set(pars.distSize, min, max);

	string title = string(hist->GetTitle()) + " | Length : " + to_string(pars.distSize)
			+ " | Iterations: " + to_string(pars.distIters);
	hist->SetTitle(title.c_str());

	hist->SetXTitle("Number of Comparisions");

	return(hist);
}


TProfile* setProf(Parameters pars, TProfile* prof) {
//	TAxis* x = prof->GetXaxis();
//	x->Set(pars.maxN-pars.minN, pars.minN, pars.maxN);

	prof->SetBins(pars.maxN-pars.minN, pars.minN, pars.maxN);


	string title = string(prof->GetTitle()) + " | Iterations: " + to_string(pars.iters);
	prof->SetTitle(title.c_str());

	prof->SetXTitle("Size of List");
	prof->SetYTitle("Number of Comparisons");

	return(prof);
}


TProfile* fitProf(Parameters pars, TProfile* prof) {
	TF1 *compFit = new TF1("compFit", "[0]+[1]*x+[2]*x^2+[3]*x^3", pars.minN, pars.maxN);
	compFit->SetParameters(1, 1, 1, 1);

	prof->Fit(compFit);

	return(prof);
}
