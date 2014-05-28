#ifndef __WZEvent_h__
#define __WZEvent_h__

#include "TLorentzVector.h"
#include "external/ExRootAnalysis/ExRootClasses.h"
#include <vector>

struct GeneratorParticle
{
    float pt;
    float eta; 
};

struct WZlVectors
{
    TLorentzVector lepton1;
    TLorentzVector lepton2;
    TLorentzVector lepton3;
    //decayed from W
    TLorentzVector leptonFromW;
    TLorentzVector jet1;
    TLorentzVector jet2;
    TLorentzVector jetsSum;
    //Includes MET
    TLorentzVector leptonSum;
    TLorentzVector WZSum;
    TLorentzVector met;
};

struct EventCounter
{
    int leptons;
    int jets;
    int electrons;
    int muons;
    int leptons_ptCut;
    int electrons_ptCut;
    int muons_ptCut;
    int jets_ptCut;
};

struct Cuts
{
    float leptonPt;
    float jetPt;
    float leptonEta;
    float jetEta;
};


class WZEvent
{
private: 
    TRootLHEFParticle* particle;
    TRootLHEFParticle* particleMother;
    float WMass;
    EventCounter counter;
    Cuts cuts;
    WZlVectors wzlVectors;
    std::vector<GeneratorParticle> allElectrons;
    std::vector<GeneratorParticle> allMuons;
    
    void processWZLeptons(int* typeCounter, int* typePtCut_counter);

public:
    WZEvent();
    WZEvent(TRootLHEFParticle*, TRootLHEFParticle*);
    void setParticle(TRootLHEFParticle*);
    void setParticleMother(TRootLHEFParticle*);
    bool isGeneratedParticle(int);
    bool particleIsStable();
    bool particleIsLepton();
    bool isGeneratedParticle(); 
    void foundLepton();
    TLorentzVector getWZSum();
    TLorentzVector getWZleptonSum();
    //foundJet();
    void resetEvent();
    float getWMass();
    int getGenElectronNumber();
    int getGenMuonNumber();
    int getGenMuonPtCutNumber();
    int getGenElectronPtCutNumber();
};

#endif
