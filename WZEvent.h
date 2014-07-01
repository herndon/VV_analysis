/**
 * @file WZEvent.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 * @author Kenneth Long <kdlong@hep.wisc.edu>
 *
 * @brief Stores the particle information from a generator WZ event.
 *
 * This class reads and stores all particle information from a single 
 * WZ generated event. It stores simple kinematic information on all leptons
 * and jets in the event (p_{T} and eta) as well as TLorentzVectors of all 
 * WZ-type leptons and W and Z particles (identified by pt and eta cuts on
 * leptons and jets). This information is accessed for each event by 
 * instances of the WZEventsTacker class.
 */  
#ifndef __WZEvent_h__
#define __WZEvent_h__

#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "external/ExRootAnalysis/ExRootClasses.h"
#include <vector>
#include <string>
#include "LHEWeights.h"

class ParticleVector : public TLorentzVector
{
private:
    int type;
public:
    ParticleVector(int type)
    {
        this->type = type;
        TLorentzVector();
    };
    ParticleVector() {ParticleVector(0);};
    int getType() const {return type;};
};


/**
 * @struct WZlVectors
 * @brief Holds lorentz vectors of particles from a WZ VBF-type event
 *
 * @param lepton1 Lorentz vector of a VBF-type muon or electron
 * @param lepton2 Lorentz vector of a VBF-type muon or electron
 * @param lepton3 Lorentz vector of a VBF-type muon or electron
 * @param leptonFromW Lorentz vector of a VBF lepton which decays from the W.
 * @param jet1 Lorentz vector of a VBF-type jet.
 * @param jet1 Lorentz vector of a VBF-type jet.
 * @param MET Lorentz vector of a VBF neutrino.
 * @param W Lorentz vector of W.
 * @param Z Lorentz vector of Z.
 */ 
struct WZlVectors
{
    std::vector<ParticleVector> allLeptons;
    std::vector<TLorentzVector> allJets; 
    TLorentzVector MET;
    TLorentzVector W;
    TLorentzVector Z;
};

/**
 * @struct EventCounter
 * @brief Holds all counters indentifing the number of events passing each
 *        selection.
 *
 * @param leptons Number of leptons in event.
 * @param jets Number of jets in event.
 * @param electrons Number of electrons in event.
 * @param muons Number of muons in event.
 * @param leptonsPostCut number of leptons with \f$p_{T} > \f$
 *        WZEvent::cuts.leptonPt and \f$\eta > \f$ WZvent::cuts.leptonEta
 * @param electronsPostCut number of electrons with \f$p_{T} >\f$
 *        Cuts::leptonPt and \f$\eta > \f$ WZEvent::cuts.leptonEta.
 * @param leptonsPostCut number of leptons with \f$p_{T} > \f$
 *        Cuts::leptonPt and \f$\eta > \f$ Cuts::leptonEta.
 */
struct EventCounter
{
    int leptons;
    int jets;
    int electrons;
    int muons;
    int leptonsPostCut;
    int electronsPostCut;
    int muonsPostCut;
    int jetsPostCut;
};

struct Cuts
{
    float leptonPt;
    float jetPt;
    float leptonEta;
    float jetEta;
};

/**
 *
 * @class WZEvent
 *
 *
 */
class WZEvent
{
private: 
    TRootLHEFParticle* particle;
    TRootLHEFParticle* particleMother;
    LHEWeights* weights;
    bool useWeights;
    EventCounter counter;
    Cuts cuts;
    WZlVectors wzlVectors;
    
    void processWZLepton(std::string type);
    void foundLepton();
    void foundJet();
    void foundW();
    void foundZ();
    void foundMET();
    void readWeights();   
public:
    /// Constructor
    WZEvent();
    /// Constructor with stuff
    WZEvent(const char* lheFileName);
    ~WZEvent();
    void loadEvent(TClonesArray* branchGenParticle);
    void setLeptonCuts(float, float);
    void setJetCuts(float, float);
    void removeCuts();

    float getDiJetInvMass();
    float getJetDeltaEta();
    float getZpt();
    float getWZTransMass();
    float getWZInvMass();

    std::vector<ParticleVector>& getAllLeptons();
    std::vector<TLorentzVector>& getAllJets();
    const std::vector<float>& getWeights();
    void resetEvent();
    float getWMass();
    float getZMass();
    float getMET();
    int getNumLeptons();
    int getNumElectrons();
    int getNumMuons();
    int getNumPostCutMuons();
    int getNumPostCutElectrons();
    int getNumPostCutLeptons();
    int getNumPostCutJets();
    int getNumWeights();
    const int getSMWeightPos();
    float getSMWeight();
};

#endif
