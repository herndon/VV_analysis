#ifndef __WZPLOTS_H__
#define __WZPLOTS_H__

#include <string>
#include "TH1.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include <vector>

class WZPlots
{
private:
    ExRootResult* result;
    TH1* electron_pt;
    TH1* electron_eta;
    TH1* muon_pt;
    TH1* muon_eta;
    std::vector<TH1*> jet_pt;
    std::vector<TH1*> jet_eta;
    TH1* met;
    TH1* deltaEta_jj;
    TH1* mjj;
    TH1* Z_pt;
    TH1* wztmass;
    TH1* wzmass;
    std::vector<TH1*> wztmassWeights;
public:
    WZPlots(ExRootResult* result);
    WZPlots(ExRootResult* result, const int, const int, std::string);
    void addElectron(float pt, float eta);
    void addMuon(float pt, float eta);
    void addJet(float pt, float eta, int jetNum);
    void fillMET(float met);
    void fillDeltaEta_jj(double deltaEta_jj);
    void fillMjj(double mjj);
    void fillZpt(double Zpt);
    void fillWZtMass(double wztmass);
    void fillWZMass(double);
    void printHistograms(const char*);
};   

#endif    
