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
    return wzlVectors.leptonSum;
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
            processWZLeptons(&counter.electrons, &counter.electrons_ptCut);
            allElectrons.push_back(lepton);
            break;
        case 13:
            processWZLeptons(&counter.muons, &counter.muons_ptCut);  
            allMuons.push_back(lepton);
            break;
        default:
            std::cout << "A problem occured processing leptons in WZEvent class.";
            exit(0);
    }
}

void WZEvent::processWZLeptons(int* typeCounter, int* typePtCut_counter)
{
    *typeCounter++;
    
    if(particle->PID == 24)
    {    
        wzlVectors.leptonFromW.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
        WMass = particleMother->M;
    }
    if(fabs(particle->Eta) < cuts.leptonEta && particle->PT < cuts.leptonPt) 
    {
        *typePtCut_counter++;
        switch(counter.leptons)
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
                std::cout << "A critical error occured in processLepton() function.";
                exit(0);
         }
    }
}
void WZEvent::resetEvent()
{
    particle = NULL;
    particle = NULL;
    counter.leptons = 0;
    counter.jets = 0;
    counter.electrons = 0;
    counter.muons = 0;
    counter.leptons_ptCut = 0;
    counter.electrons_ptCut = 0;
    counter.muons_ptCut = 0;
    counter.jets_ptCut = 0;

    cuts.leptonPt = 0.;
    cuts.jetPt = 0.;
    cuts.leptonEta = 0.;
    cuts.jetEta = 0.;
}

