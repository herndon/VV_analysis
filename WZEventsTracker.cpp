#include "WZEventsTracker.h"
#include <iostream>

using namespace std;

WZEventsTracker::WZEventsTracker(ExRootResult* result, WZEvent* event,
                                                            std::string name)
{
    wzEvent = event;
    nameBase = name;

    const int numWeights =  wzEvent->getNumWeights();
    plots = new WZPlots(result, nameBase, numWeights, wzEvent->getSMWeightPos());
    luminosity = 0.0;
    
    if(numWeights > 1)
    {
        useWeights = true;
        crossSections.resize(numWeights);
        crossSections = {0.};
    }
    else
        useWeights = false;
    eventCounts = {0};
    kinCuts = {0.};
    tieredCuts.numHighPtLeptons = 3;
    tieredCuts.numHighPtJets = 2;
    tieredCuts.met = 0.;
    tieredCuts.ZMassRange = 0.;
}

WZEventsTracker::~WZEventsTracker()
{
    delete plots;
}

void WZEventsTracker::setLuminosity(float luminosity)
{
    this->luminosity = luminosity;
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
void WZEventsTracker::setJetMassCut(float diJetMass)
{
    kinCuts.diJetMass = diJetMass;
}
void WZEventsTracker::setEtajjCut(float eta_jj)
{
    kinCuts.jetDeltaEta = eta_jj;
}

//THESE ARE TIERED CUTS!
void WZEventsTracker::processEvent(WZEvent* event)
{
    wzEvent = event;

    if(!passedKinematicCuts())
        return;
    if(wzEvent->getNumPostCutLeptons() == tieredCuts.numHighPtLeptons)
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

    if(useWeights)
    {
        int i = 0;
        for(auto weight : wzEvent->getWeights())
        {
            crossSections[i] += weight;
            i++;
        }
    }

    fillPlots();

}

bool WZEventsTracker::passedKinematicCuts()
{
    if(wzEvent->getWZTransMass() < kinCuts.WZTMass)
        return false;
    else if(std::abs(wzEvent->getJetDeltaEta() < kinCuts.jetDeltaEta))
        return false;
    else if(wzEvent->getDiJetInvMass() < kinCuts.diJetMass)
        return false;
    
    return true;
}


void WZEventsTracker::processByLeptonType()
{
    int numHighPtE = wzEvent->getNumPostCutElectrons();
    int numHighPtMu = wzEvent->getNumPostCutMuons();

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
    cout << "\n\nEvents Passing Selection for " << nameBase << " class" << endl;
    cout << "Kinematic cuts applied to all events: " << endl;
    cout << "di-Jet invariant mass > " << kinCuts.diJetMass << " GeV" << endl;
    cout << "Jet eta separation > " << kinCuts.jetDeltaEta << endl;
    cout << "WZ transverse mass > " << kinCuts.WZTMass << " GeV" << endl << endl;

    cout << "Number of events passing post cut lepton number = "
         << tieredCuts.numHighPtLeptons << ": "  << eventCounts.passedLeptonCut
         << endl;
    cout << "Number of 3 muon events: " << eventCounts.events3mu << endl;
    cout << "Number of 1 electron 2 muon events:  " << eventCounts.events2mu1e 
         << endl;
    cout <<  "Number of 2 electron 1 muon events:  " << eventCounts.events2e1mu
         << endl;
    cout <<  "Number of 3 electon  events:  " << eventCounts.events3e << endl;
    cout << "Number of events passing post cut jet number = "
         << tieredCuts.numHighPtJets << ": "  << eventCounts.passedJetCut << endl;
    cout << "Number of events with MET > " << tieredCuts.met << " GeV: "
         << eventCounts.passedMETCut << endl;
    cout << "Number of events with Z Mass within " << tieredCuts.ZMassRange
         << " GeV of on-shell Z mass: " << eventCounts.passedZMassCut << endl;
    cout << "\n__________________________________________________________________\n";
}

void WZEventsTracker::fillPlots()
{
    float scale = 1.0;

    if(luminosity != 0.0 && useWeights)
        scale = wzEvent->getSMWeight()*luminosity;

    for(auto electron : wzEvent->getAllElectrons())
    {
        plots->addElectron(electron.pt, electron.eta, scale);
    }
    for(auto muon : wzEvent->getAllMuons())
    {
        plots->addMuon(muon.pt, muon.eta, scale);
    }
    for(auto jet : wzEvent->getAllJets())
    {
        plots->addJet(jet.pt, jet.eta, scale);
    }
    plots->fillMET(wzEvent->getMET(), scale);
    plots->fillDeltaEta_jj(wzEvent->getJetDeltaEta(),scale);
    plots->fillMjj(wzEvent->getDiJetInvMass(), scale);
    plots->fillZpt(wzEvent->getZpt(), scale);
    plots->fillWZTMass(wzEvent->getWZTransMass(),scale);
    plots->fillWZMass(wzEvent->getWZInvMass(),scale);
    if(useWeights)
    {
        if(luminosity == 0.0)
            plots->fillWZTMassWeights(wzEvent->getWZTransMass(), 
                                            wzEvent->getWeights(), 1.);
        else
        {
        //    std::cout << "HI";
            plots->fillWZTMassWeights(wzEvent->getWZTransMass(), 
                                            wzEvent->getWeights(), luminosity);
        }
    }
}

