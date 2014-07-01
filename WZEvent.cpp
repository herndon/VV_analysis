#include "WZEvent.h"
#include <cmath>
#include <iostream>

WZEvent::WZEvent(const char* lheFileName)
{
    weights = new LHEWeights(lheFileName);
    useWeights = true;
    resetEvent();
}
WZEvent::WZEvent()
{
    useWeights = false;
    weights = NULL;
    resetEvent();
}
WZEvent::~WZEvent()
{
    delete weights;
}
const std::vector<float>& WZEvent::getWeights()
{
    return weights->getVector();
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
    if(useWeights)
        weights->readWeights();

}
float WZEvent::getSMWeight()
{
    if(weights == NULL)
        return 0.;
    else
        return weights->getSMWeight();
}
const int WZEvent::getSMWeightPos()
{
    if(weights == NULL)
        return 0;
    else
        return weights->getSMWeightPos();
}
float WZEvent::getWZInvMass()
{
    return (wzlVectors.W + wzlVectors.Z).M();
}
float WZEvent::getWZTransMass()
{
    if(wzlVectors.allLeptons.size() == 3) 
    {
        TLorentzVector leptonSum;
        for(const auto& lepton : wzlVectors.allLeptons)
            leptonSum += lepton;
        return  (leptonSum + wzlVectors.MET).M();
    }
    else
    {
        std::cout << "\nError in WZ transverse mass calculation.\n";
        return 0.;
    }
}
float WZEvent::getDiJetInvMass()
{
    if(wzlVectors.allJets.size() == 2) 
        return (wzlVectors.allJets[0] + wzlVectors.allJets[1]).M();
    else
        std::cout << "\nError in di-Jet mass calculation.\n";
    return 0.;
}
float WZEvent::getJetDeltaEta()
{
    if(wzlVectors.allJets.size() == 2) 
        return std::abs(wzlVectors.allJets[0].Eta() - wzlVectors.allJets[1].Eta());
    else
    {
        std::cout << "\nError in Jet deltaEta calculation.\n";
        return 0.;
    }
}
float WZEvent::getZpt()
{
    return wzlVectors.Z.Pt();
}
int WZEvent::getNumWeights()
{
    if(weights == NULL)
        return 0;
    else
        return weights->getNumWeights();
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
int WZEvent::getNumLeptons()
{
    return counter.leptons;
}
int WZEvent::getNumElectrons()
{
    return counter.electrons;
}
int WZEvent::getNumMuons()
{
    return counter.muons;
}
int WZEvent::getNumPostCutMuons()
{
    return counter.muonsPostCut;
}
int WZEvent::getNumPostCutElectrons()
{
    return counter.electronsPostCut;
}
int WZEvent::getNumPostCutLeptons()
{
    return counter.leptonsPostCut;
}
float WZEvent::getWMass()
{
    return wzlVectors.W.M();
}
float WZEvent::getZMass()
{
    return wzlVectors.Z.M();
}
float WZEvent::getMET()
{
    return wzlVectors.MET.Pt();
}
std::vector<ParticleVector>& WZEvent::getAllLeptons()
{
    return wzlVectors.allLeptons;
}
std::vector<TLorentzVector>& WZEvent::getAllJets()
{
    return wzlVectors.allJets;
}

void WZEvent::foundLepton()
{
    counter.leptons++;

    switch(abs(particle->PID))
    {
        case 11:
            processWZLepton("electron");
            counter.electrons++;
            break;
        case 13:
            processWZLepton("muon");
            counter.muons++;
            break;
        default:
            std::cout << "A problem occured processing leptons in WZEvent class.";
            exit(0);
    }
}
void WZEvent::processWZLepton(std::string type)
{
    if(std::abs(particle->Eta) < cuts.leptonEta && particle->PT > cuts.leptonPt) 
    {
        this->counter.leptonsPostCut++;
        
        ParticleVector lepton(particle->PID);
        lepton.SetPtEtaPhiM(particle->PT,particle->Eta,particle->Phi,particle->M);
        wzlVectors.allLeptons.push_back(lepton);
        
        if(type == "muon")
        {
            this->counter.muonsPostCut++;
        }
        else if(type == "electron")
        {
            this->counter.electronsPostCut++;
        }
        else
        {
            std::cout << "A critical error occured in processLepton() function.";
            exit(0);
        }
    }
}
void WZEvent::foundJet()
{
    counter.jets++;

    if(std::abs(particle->Eta) < cuts.jetEta && particle->PT > cuts.jetPt)
    {
        counter.jetsPostCut++;
        TLorentzVector jet;
        jet.SetPtEtaPhiM(particle->PT,particle->Eta,particle->Phi,particle->M);
        wzlVectors.allJets.push_back(jet);
   }
}
void WZEvent::foundMET()
{
    wzlVectors.MET.SetPtEtaPhiM(particle->PT, 0.0,particle->Phi,particle->M);
}
int WZEvent::getNumPostCutJets()
{
    return counter.jetsPostCut;
}
void WZEvent::foundZ()
{
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
    wzlVectors.allLeptons.clear();
    wzlVectors.allJets.clear();
    counter = {0};
}
    

