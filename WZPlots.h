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
    TH1* jet_pt;
    TH1* jet_eta;
    TH1* met;
    TH1* deltaEta_jj;
    TH1* mjj;
    TH1* Z_pt;
    TH1* wztmass;
    TH1* wzmass;
    std::vector<TH1*> wztmassWeights;
public:
    WZPlots(ExRootResult* result);
    WZPlots(ExRootResult* result, std::string, const int, const int);
    void addElectron(float pt, float eta, float scale);
    void addMuon(float pt, float eta, float scale);
    void addJet(float pt, float eta, float scale);
    void fillMET(float met, float scale);
    void fillDeltaEta_jj(double deltaEta_jj, float scale);
    void fillMjj(double mjj, float scale);
    void fillZpt(double Zpt, float scale);
    void fillWZTMass(double wztmass, float scale);
    void fillWZMass(double, float scale);
    void fillWZTMassWeights(float wztmass, const std::vector<float>& weights,
                                                             float luminosity);
    void printHistograms(const char*);
};

#endif    
