#include "WZEventsTracker.h"
#include <iostream>

using namespace std;

WZEventsTracker::WZEventsTracker(ExRootResult* result, 
                                            const int NUM_WEIGHTS, std::string name, bool cuts)
{
    useCuts = cuts;
    nameBase = name;
    plots = new WZPlots(result, NUM_WEIGHTS, nameBase);

    eventCounts = {0};
    kinCuts = {0.};
    tieredCuts.numHighPtLeptons = 0;
    tieredCuts.numHighPtJets = 0;
    tieredCuts.met = 0.;
    tieredCuts.ZMassRange = 0.;
}
WZEventsTracker::~WZEventsTracker()
{
    delete plots;
}
void WZEventsTracker::setLeptonSelection(int numLeptons)
{
    tieredCuts.numHighPtLeptons = numLeptons;
}
void WZEventsTracker::setJetSelection(int numJets)
{
    tieredCuts.numHighPtJets = numJets;
}
void WZEventsTracker::setMetCut(float metCut)
{
    tieredCuts.met = metCut;
}
void WZEventsTracker::setZMassCut(float ZMassRange)
{
    tieredCuts.ZMassRange = ZMassRange;
}
void WZEventsTracker::setWZTMassCut(float WZTMass)
{  
    kinCuts.WZTMass = WZTMass;
}
void WZEventsTracker::setJetMassCut(float jetMass)
{
    kinCuts.jetMass = jetMass;
}
void WZEventsTracker::setEtajjCut(float eta_jj)
{
    kinCuts.jetDeltaEta = eta_jj;
}

//THESE ARE TIERED CUTS!
void WZEventsTracker::processEvent(WZEvent* event)
{
    wzEvent = event;
    
    if(useCuts)
    {
        if(!passedKinematicCuts())
            return;
        if(wzEvent->getNumHighPtLeptons() == tieredCuts.numHighPtLeptons)
        {
            eventCounts.passedLeptonCut++;
            processByLeptonType();
        }
        else
            return;
        if(wzEvent->getNumPostCutJets() == tieredCuts.numHighPtJets)
            eventCounts.passedJetCut++;
        else
            return;
        if(wzEvent->getMET() > tieredCuts.met)
            eventCounts.passedMETCut++;
        else
            return;
        if(std::abs(wzEvent->getZMass() - ON_SHELL_ZMASS) < tieredCuts.ZMassRange)     
            eventCounts.passedZMassCut++;       
        else
            return;
    }
    
    fillPlots();
      
}

bool WZEventsTracker::passedKinematicCuts()
{
    if(wzEvent->getWZleptonMETSum().M() < kinCuts.WZTMass)
        return false;
    else if(std::abs(wzEvent->getJet1().Eta() - wzEvent->getJet2().Eta()) < kinCuts.jetDeltaEta)
        return false;
    else if(wzEvent->getJetSum().M() < kinCuts.jetMass)
        return false;
    
    return true;
}


void WZEventsTracker::processByLeptonType()
{
    int numHighPtE = wzEvent->getNumHighPtElectrons();
    int numHighPtMu = wzEvent->getNumHighPtMuons();

    if(numHighPtE == 3 && numHighPtMu == 0)
        eventCounts.events3e++;
    else if(numHighPtE == 2  && numHighPtMu == 1)
        eventCounts.events2e1mu++;
    else if(numHighPtE == 1 && numHighPtMu == 2)
        eventCounts.events2mu1e++;
    else if(numHighPtE == 0 && numHighPtMu == 3)
        eventCounts.events3mu++;
    else
    {
        std::cout << "A Critical Error occurred in the processByType() function of "
                  << "WZEventsTracker Class.\n";
        exit(0);
    }
}
void WZEventsTracker::printEventInfo()
{
    cout << "\nOutput from the WZEventsTrackerClass\n";

    cout << "Gen event counts" << endl;
    cout << "Gen Baseline selection 1, leptons: " << eventCounts.passedLeptonCut << endl;
    cout << "Gen Baseline selection 1, 3m     : " << eventCounts.events3mu << endl;
    cout << "Gen Baseline selection 1, 2m1e   : " << eventCounts.events2mu1e << endl;
    cout << "Gen Baseline selection 1, 2e1m   : " << eventCounts.events2e1mu << endl;
    cout << "Gen Baseline selection 1, 3e     : " << eventCounts.events3e << endl;
    cout << "Gen Baseline selection 1, jets   : " << eventCounts.passedJetCut << endl;
    cout << "Gen Baseline selection 1, MET    : " << eventCounts.passedMETCut << endl;
    cout << "Gen Baseline selection 1, Z      : " << eventCounts.passedZMassCut << endl;
    cout << " " << endl << "____________________________\n";
}

void WZEventsTracker::fillPlots()
{
    for(auto electron : wzEvent->getAllElectrons())
    {
        plots->addElectron(electron.pt, electron.eta);
    }
    for(auto muon : wzEvent->getAllMuons())
    {
        plots->addMuon(muon.pt, muon.eta);
    }
    for(auto jet : wzEvent->getAllJets())
    {
        plots->addJet(jet.pt, jet.eta);
    }
    plots->fillMET(wzEvent->getMET());
    plots->fillDeltaEta_jj(std::abs(wzEvent->getJet1().Eta() - wzEvent->getJet2().Eta()));
    plots->fillMjj(wzEvent->getJetSum().M());
    plots->fillZpt(wzEvent->getZ().Pt());
    plots->fillWZtMass(wzEvent->getWZleptonMETSum().M());
    plots->fillWZMass(wzEvent->getWZSum().M());
}

