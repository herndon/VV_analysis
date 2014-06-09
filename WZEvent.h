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



/**
 * @struct GeneratorParticle
 * @brief Holds the pt and eta of a simple particle
 *
 *
 * @param pt P_{T} of the particle
 * @param eta pseudo-rapidity of the particle
 *
 */
struct GeneratorParticle
{
    float pt;
    float eta; 
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
    TLorentzVector lepton1;
    TLorentzVector lepton2;
    TLorentzVector lepton3;
    TLorentzVector leptonFromW;
    TLorentzVector jet1;
    TLorentzVector jet2;
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

/**
 * @stuct Cuts
 * 
 * @param leptonPt minimum \f$p_{T}\f$ for a VBF-type lepton
 * @param jetPt \f$p_{T}\f$ for a VBF-type jet
 * @param leptonEta minimum \f$p\eta\f$ for a VBF-type lepton
 * @param jetEta minimum \f$eta\f$ for a VBF-type jet
 */ 
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
    //vector<float> weights;
    float WMass;
    float ZMass;
    float MET;
    EventCounter counter;
    Cuts cuts;
    WZlVectors wzlVectors;
    std::vector<GeneratorParticle> allElectrons;
    std::vector<GeneratorParticle> allMuons;
    std::vector<GeneratorParticle> allJets; 
    void processWZLepton(std::string type);
    void foundLepton();
    void foundJet();
    void foundW();
    void foundZ();
    void foundMET();
    //void readWeights();   
public:
    /// Constructor
    WZEvent();
    /// Constructor with stuff
    WZEvent(TRootLHEFParticle*, TRootLHEFParticle*);
    /// Reads Event
    void loadEvent(TClonesArray* branchGenPArticle);
    /// Sets Lepton Pt and Eta cuts for identifying a VBF event
    void setLeptonCuts(float, float);
    void setJetCuts(float, float);
    void removeCuts();
    TLorentzVector getWZSum();
    TLorentzVector getWZleptonMETSum();
    TLorentzVector getLeptonFromW();
    TLorentzVector getJet1();
    TLorentzVector getJet2();
    TLorentzVector getMETVector();
    TLorentzVector getJetSum();
    TLorentzVector getZ();
    std::vector<GeneratorParticle> getAllElectrons();
    std::vector<GeneratorParticle> getAllMuons();
    std::vector<GeneratorParticle> getAllJets();
    void resetEvent();
    float getWMass();
    float getZMass();
    float getMET();
    int getGenLeptonNumber();
    int getGenElectronNumber();
    int getGenMuonNumber();
    int getNumHighPtMuons();
    int getNumHighPtElectrons();
    int getNumHighPtLeptons();
    int getNumPostCutJets();
};

#endif
