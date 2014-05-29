#include "WZEvent.h"
#include <cmath>
#include <iostream>

WZEvent::WZEvent(TRootLHEFParticle* currentParticle, TRootLHEFParticle* mother)
{
    resetEvent();
    particle = currentParticle;
    particleMother = mother;
}
WZEvent::WZEvent()
{
    WZEvent(NULL, NULL);
}
void WZEvent::setParticle(TRootLHEFParticle* currentParticle)
{
    particle = currentParticle;
}
void WZEvent::setParticleMother(TRootLHEFParticle* mother)
{
    particleMother = mother;
}
bool WZEvent::particleIsStable()
{
    return (particle->Status == 1);
}
bool WZEvent::particleIsLepton()
{
    return (abs(particle->PID) == 11 || abs(particle->PID == 13)); 
}
bool WZEvent::isGeneratedParticle(int particleCount)
{
    return (particle->Mother1 < particleCount + 2); 
}
TLorentzVector WZEvent::getWZSum()
{
    return wzlVectors.WZSum;
}
TLorentzVector WZEvent::getWZleptonSum()
{
    wzlVectors.leptonSum = wzlVectors.lepton1 + wzlVectors.lepton2
                                + wzlVectors.lepton3;
    return wzlVectors.leptonSum;
}
TLorentzVector WZEvent::getleptonFromW()
{
    return wzlVectors.leptonFromW;
}
void WZEvent::setLeptonCuts(float leptonPt, float leptonEta)
{
    cuts.leptonPt = leptonPt;
    cuts.leptonEta = leptonEta;
}
void WZEvent::setJetCuts(float jetPt, float jetEta)
{
    cuts.jetPt = jetPt;
    cuts.jetEta = jetEta;
}
int WZEvent::getGenLeptonNumber()
{
    return counter.leptons;
}
int WZEvent::getGenElectronNumber()
{
    return counter.electrons;
}
int WZEvent::getGenMuonNumber()
{
    return counter.muons;
}
int WZEvent::getGenMuonPtCutNumber()
{
    return counter.muons_ptCut;
}
int WZEvent::getGenElectronPtCutNumber()
{
    return counter.electrons_ptCut;
}
int WZEvent::getGenLeptonPtCutNumber()
{
    return counter.leptons_ptCut;
}
float WZEvent::getWMass()
{
    return WMass;
}      
void WZEvent::foundLepton()
{
    counter.leptons++;
    GeneratorParticle lepton;
    lepton.pt = particle->PT;
    lepton.eta = particle->Eta;

    switch(abs(particle->PID))
    {
        case 11:
            processWZLepton("electron");
            counter.electrons++;
            allElectrons.push_back(lepton);
            break;
        case 13:
            processWZLepton("muon");
            counter.muons++;
            allMuons.push_back(lepton);
            break;
        default:
            std::cout << "A problem occured processing leptons in WZEvent class.";
            exit(0);
    }
}
void WZEvent::processWZLepton(std::string type)
{
    if(particleMother->PID == 24)
    {    
        wzlVectors.leptonFromW.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
        WMass = particleMother->M;
    }
    if(fabs(particle->Eta) < cuts.leptonEta && particle->PT > cuts.leptonPt) 
    {
        counter.leptons_ptCut++;
        if(type == "muon")
            this->counter.muons_ptCut++;
        else if(type == "electron")
            this->counter.electrons_ptCut++;
        else
        {
            std::cout << "A critical error occured in processLepton() function.";
            exit(0);
        }
        switch(counter.leptons_ptCut)
        {
            case 1:
                wzlVectors.lepton1.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
                //lepton1Type = type;
                break;
            case 2: 
                wzlVectors.lepton2.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
                //lepton2Type = type;
                break;
            case 3:
                wzlVectors.lepton2.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
                //lepton3Type = type;
                break;
            default:
                std::cout << "More than 3 hight PT leptons\n";
                //std::cout << "A critical error occured in processLepton() function.";
                //exit(0);
         }
    }
}
void WZEvent::removeCuts()
{
    cuts.leptonPt = 0.;
    cuts.jetPt = 0.;
    cuts.leptonEta = 0.;
    cuts.jetEta = 0.;
}
void WZEvent::resetEvent()
{
    particle = NULL;
    particleMother = NULL;
    allElectrons.clear();
    allMuons.clear();
    counter.leptons = 0;
    counter.jets = 0;
    counter.electrons = 0;
    counter.muons = 0;
    counter.leptons_ptCut = 0;
    counter.electrons_ptCut = 0;
    counter.muons_ptCut = 0;
    counter.jets_ptCut = 0;
}
    

