#if ndef __WZEvent_h__
#define __WZEvent_h__

class WZEvent
{
private: 
    TRootLHEFParticle* particle;
    TRootLHEFParticle* particleMother;
    EventCounter counter;
    Cuts cuts;
    LorentzVectors lorentzVectors;
    float WMass;
    float ZMass;

public:
    WZEvent();
    setParticle(TRootLHEFParticle*);
    bool particleIsStable();
    bool particleIs 
    foundFinalStateLepton()
    foundJet()
};
struct KinematicVariables
{
    WMass;
    ZMass;
    
};

struct LorentzVectors
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

#endif
