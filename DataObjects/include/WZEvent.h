#ifndef __WZEvent_h__
#define __WZEvent_h__
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

#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "ExRootAnalysis/include/ExRootClasses.h"
#include <vector>
#include <string>
#include <map>
#include "DataObjects/include/LHEWeights.h"
#include "DataObjects/include/ParticleVector.h"

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

namespace vvana {
  
struct WZlVectors
{
    std::vector<ParticleVector> allLeptons;
    std::vector<ParticleVector> allJets; 
    TLorentzVector MET;
    TLorentzVector neutrino;
    TLorentzVector W;
    TLorentzVector Z;
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
    bool hasLepZ;
    std::map<std::string,int> particleCounts;
    Cuts cuts;
    WZlVectors wzlVectors;
    
    void processWZLepton(std::string type);
    void foundLepton();
    void foundJet();
    void foundLeptonicW();
    void foundLeptonicZ();
    void foundMET();
    void readWeights();   
public:
    /// Constructor
    WZEvent();
    /// Constructor with stuff
    WZEvent(const char* lheFileName);
    ~WZEvent();
    void loadEvent(TClonesArray* branchGenParticle,TClonesArray* branchWeights);
    void setLeptonCuts(float, float);
    void setJetCuts(float, float);
    void removeCuts();

    float getDiJetInvMass();
    float getJetDeltaEta();
    float getZpt();
    float getWZTransMass();
    float get4lTransMass();
    float getWZInvMass();
    float get4lMass();

    const std::vector<std::string>& getWeightNames();
    std::vector<ParticleVector>& getAllLeptons();
    std::vector<ParticleVector>& getAllJets();
    const std::vector<float>& getWeightsVector();
    const LHEWeights* getLHEWeights(); 
    void resetEvent();
    float getWMass();
    float getZMass();
    float getMET();
    bool hasLeptonicZ();
    unsigned int getNumLeptons();
    unsigned int getNumElectrons();
    unsigned int getNumMuons();
    unsigned int getNumPostCutMuons();
    unsigned int getNumPostCutElectrons();
    unsigned int getNumPostCutLeptons();
    unsigned int getNumPostCutJets();
    unsigned int getNumPostCutTaus();
    unsigned int getNumWeights();
    const unsigned int getSMWeightPos();
    float getSMWeight();
};
} // end namespace vvana 
 
#endif // __WZEvent_h__
