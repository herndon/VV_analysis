#ifndef __WZPLOTS_H__
#define __WZPLOTS_H__

#include <string>
#include "TH1.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include <vector>
#include "WZEvent.h"

class WZPlots
{
private:
    ExRootResult* result;
    std::vector<TH1*> lepton_pt;
    std::vector <TH1*> lepton_eta;
    
    TH1* diLeptonMass3eNearZ;
    TH1* diLeptonMass3eOffZ;
    TH1* diLeptonPt3eNearZ;
    TH1* diLeptonPt3eOffZ;
    
    TH1* diLeptonMass3muNearZ;
    TH1* diLeptonMass3muOffZ;
    TH1* diLeptonPt3muNearZ;
    TH1* diLeptonPt3muOffZ;
    
    TH1* diLeptonMass2mu;
    TH1* diLeptonPt2mu;
    TH1* diLeptonMass2e;
    TH1* diLeptonPt2e;

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
    void addLeptons(const std::vector<ParticleVector>& leptons, float scale);
    void addJet(float pt, float eta, float scale);
    void fillMET(float met, float scale);
    void fillDeltaEta_jj(double deltaEta_jj, float scale);
    void fillMjj(double mjj, float scale);
    void fillZpt(double Zpt, float scale);
    void fillWZTMass(double wztmass, float scale);
    void fillWZMass(double, float scale);
    void fillWZTMassWeights(float wztmass, const std::vector<float>& weights,
                                                             float luminosity);
    void add1typeDiLeptons(const std::vector<TLorentzVector>&, std::string, float);
    void add2muDiLepton(const TLorentzVector& diLepton, float scale);
    void add2eDiLepton(const TLorentzVector& diLepton, float scale);
    void printHistograms(const char*);
};

#endif    
