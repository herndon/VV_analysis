#include "WZEvent.h"
#include <cmath>
#include <iostream>

WZEvent::WZEvent()
{
    resetEvent();
}
void WZEvent::loadEvent(TClonesArray* branchGenParticle)
{
    for(int i = 0; i < branchGenParticle->GetEntriesFast(); ++i) 
    {
        particle = (TRootLHEFParticle*) branchGenParticle->At(i);

        //Particle is stable
        if(particle->Status == 1)
        {
            if((particle->Mother1 < i+2) 
                    && (abs(particle->PID) == 11 || abs(particle->PID) == 13))
            {
                particleMother = (TRootLHEFParticle*) branchGenParticle->At(particle->Mother1-1);
                foundLepton();
            }
            // for W+Z event initial particles end after 4. Status is not 2 which
            // indicates intermediate history Pythia/delphies populates every event 
            // with extra substantially increasing the events that pass
            else if (i>5 && (abs(particle->PID) < 6||abs(particle->PID) == 21))
                foundJet();
            else if (abs(particle->PID) == 12 || abs(particle->PID) == 14) 
                foundMET();
        }
        if (particle->PID==23)
            foundZ();
        else if (particle->PID==24)  
            foundW();
    }
}
TLorentzVector WZEvent::getWZSum()
{
    return wzlVectors.W + wzlVectors.Z;
}
TLorentzVector WZEvent::getWZleptonMETSum()
{
    return  wzlVectors.lepton1 + wzlVectors.lepton2
                           + wzlVectors.MET  + wzlVectors.lepton3;
}
TLorentzVector WZEvent::getLeptonFromW()
{
    return wzlVectors.leptonFromW;
}
TLorentzVector WZEvent::getJetSum()
{
    return wzlVectors.jet1 + wzlVectors.jet2;
}
TLorentzVector WZEvent::getZ()
{
    return wzlVectors.Z;
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
int WZEvent::getNumHighPtMuons()
{
    return counter.muonsPostCut;
}
int WZEvent::getNumHighPtElectrons()
{
    return counter.electronsPostCut;
}
int WZEvent::getNumHighPtLeptons()
{
    return counter.leptonsPostCut;
}
float WZEvent::getWMass()
{
    return WMass;
}
float WZEvent::getZMass()
{
    return ZMass;
}
float WZEvent::getMET()
{
    return MET;
}
std::vector<GeneratorParticle> WZEvent::getAllElectrons()
{
    return allElectrons;
}
std::vector<GeneratorParticle> WZEvent::getAllMuons()
{
    return allMuons;
}
std::vector<GeneratorParticle> WZEvent::getAllJets()
{
    return allJets;
}
TLorentzVector WZEvent::getMETVector()
{
    return wzlVectors.MET;
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
        this->counter.leptonsPostCut++;
        
        if(type == "muon")
            this->counter.muonsPostCut++;
        else if(type == "electron")
            this->counter.electronsPostCut++;
        else
        {
            std::cout << "A critical error occured in processLepton() function.";
            exit(0);
        }
        switch(counter.leptonsPostCut)
        {
            case 1:
                wzlVectors.lepton1.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
                break;
            case 2: 
                wzlVectors.lepton2.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
                break;
            case 3:
                wzlVectors.lepton3.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
                break;
            default:
                std::cout << "A critical error occured in processLepton() function.";
                exit(0);
         }
    }
}
void WZEvent::foundJet()
{
    counter.jets++;
    GeneratorParticle jet;
    jet.pt = particle->PT;
    jet.eta = particle->Eta;

    allJets.push_back(jet);

    if(std::abs(jet.eta) < cuts.jetEta && jet.pt > cuts.jetPt)
    {
        counter.jetsPostCut++;
        if(counter.jetsPostCut == 1)
            wzlVectors.jet1.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
        else if(counter.jetsPostCut == 2)
            wzlVectors.jet2.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
        else
        {
            std::cout << "A critical error occured in foundJet() function.";
            exit(0);
        }
    }
}
void WZEvent::foundMET()
{
    MET = particle->PT;
    wzlVectors.MET.SetPtEtaPhiM(particle->PT, 0.0,
                        particle->Phi,particle->M);
}
TLorentzVector WZEvent::getJet1()
{
    return wzlVectors.jet1;
}

TLorentzVector WZEvent::getJet2()
{
    return wzlVectors.jet2;
}
int WZEvent::getNumPostCutJets()
{
    return counter.jetsPostCut;
}
void WZEvent::foundZ()
{
    ZMass = particle->M;
    wzlVectors.Z.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
}
void WZEvent::foundW()
{
    wzlVectors.W.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
}
void WZEvent::removeCuts()
{
    cuts = {0.};
}
void WZEvent::resetEvent()
{
    particle = NULL;
    particleMother = NULL;
    allElectrons.clear();
    allMuons.clear();
    allJets.clear();
    counter = {0};
}
    

