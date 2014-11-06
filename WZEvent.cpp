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
    weights = new LHEWeights("");
    resetEvent();
}
WZEvent::~WZEvent()
{
    //delete weights;
}
const std::vector<float>& WZEvent::getWeightsVector()
{
    return weights->getVector();
}
const LHEWeights* WZEvent::getLHEWeights()
{
    return weights;
}
const std::vector<std::string>& WZEvent::getWeightNames()
{
    return weights->getNames();
}
void WZEvent::loadEvent(TClonesArray* branchGenParticle)
{
    for(int i = 0; i < branchGenParticle->GetEntriesFast(); ++i) 
    {
        particle = (TRootLHEFParticle*) branchGenParticle->At(i);

        //Particle is stable
        if(particle->Status == 1)
        {
            if((particle->Mother1 < i+2) && (abs(particle->PID) == 11 
                        || abs(particle->PID) == 13|| abs(particle->PID) == 15))
            {
                if (particle->Mother1 != 0)
                {    
                    particleMother = (TRootLHEFParticle*) 
                                        branchGenParticle->At(particle->Mother1);
                    
                    if (particleMother->PID == 23 && !hasLepZ)
                    {
                        hasLepZ = true;
                        foundLeptonicZ();
                    }
                }
                foundLepton();
            }
            // for W+Z event initial particles end after 4. Status is not 2 which
            // indicates intermediate history Pythia/delphies populates every event 
            // with extra substantially increasing the events that pass
            else if (i>5 && (abs(particle->PID) < 6||abs(particle->PID) == 21))
                foundJet();
            else if (abs(particle->PID) == 12 || abs(particle->PID) == 14 ||
                     abs(particle->PID) == 16) {
                foundMET();
                particleMother = (TRootLHEFParticle*) 
                                        branchGenParticle->At(particle->Mother1);
                    
                if (particleMother->PID == 24)
                {
                    foundLeptonicW();            
                }
            }
        }
    }
    if(useWeights)
        weights->readWeights();

}
float WZEvent::getSMWeight()
{
    if(weights == NULL)
        return 1.;
    else
        return weights->getSMWeight();
}
const unsigned int WZEvent::getSMWeightPos()
{
    return weights->getSMWeightPos();
}
float WZEvent::getWZInvMass()
{
    if (hasLepZ)
        return (wzlVectors.W + wzlVectors.Z).M();
    else
        return - 1;
}
float WZEvent::get4lMass()
{
    if(wzlVectors.allLeptons.size() == 3) 
    {
        TLorentzVector leptonSum;
        for(const auto& lepton : wzlVectors.allLeptons)
            leptonSum += lepton;
        return  (leptonSum + wzlVectors.neutrino).M();
    }
    else
    {
        std::cout << "\nError in WZ transverse mass calculation.\n";
        return 0.;
    } 
}
float WZEvent::getWZTransMass()
{
    if (!hasLepZ)
        return -1;
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
    if(wzlVectors.allJets.size() >= 2)
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
    if (hasLepZ)
        return wzlVectors.Z.Pt();
    else
        return -1;
}
unsigned int WZEvent::getNumWeights()
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
unsigned int WZEvent::getNumLeptons()
{
    return particleCounts["leptons"];
}
unsigned int WZEvent::getNumElectrons()
{
    return particleCounts["electrons"];
}
unsigned int WZEvent::getNumMuons()
{
    return particleCounts["muons"];;
}
unsigned int WZEvent::getNumPostCutMuons()
{
    return particleCounts["muonsPostCut"];;
}
unsigned int WZEvent::getNumPostCutElectrons()
{
    return particleCounts["electronsPostCut"];
}
unsigned int WZEvent::getNumPostCutTaus()
{
    return particleCounts["tausPostCut"];
}
unsigned int WZEvent::getNumPostCutLeptons()
{
    return particleCounts["leptonsPostCut"];
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
std::vector<ParticleVector>& WZEvent::getAllJets()
{
    return wzlVectors.allJets;
}

void WZEvent::foundLepton()
{
    particleCounts["leptons"]++;
    switch(abs(particle->PID))
    {
        case 11:
            processWZLepton("electron");
            particleCounts["electrons"]++;
            break;
        case 13:
            processWZLepton("muon");
            particleCounts["muons"]++;
            break;
        case 15:
            processWZLepton("tau");
            particleCounts["taus"]++;
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
        this->particleCounts["leptonsPostCut"]++;
        
        ParticleVector lepton(particle->PID);
        lepton.SetPtEtaPhiM(particle->PT,particle->Eta,particle->Phi,particle->M);
        wzlVectors.allLeptons.push_back(lepton);
        
        if(type == "muon")
        {
            this->particleCounts["muonsPostCut"]++;
        }
        else if(type == "electron")
        {
            this->particleCounts["electronsPostCut"]++;
        }
        else if(type == "tau")
        {
            this->particleCounts["tausPostCut"]++;
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
    particleCounts["jets"]++;

    if(std::abs(particle->Eta) < cuts.jetEta && particle->PT > cuts.jetPt)
    {
        particleCounts["jetsPostCut"]++;
        ParticleVector jet;
        jet.SetPtEtaPhiM(particle->PT,particle->Eta,particle->Phi,particle->M);
        wzlVectors.allJets.push_back(jet);
    }
}
void WZEvent::foundMET()
{
    if(particle->PT > 0) {
        wzlVectors.MET.SetPtEtaPhiM(particle->PT, 0.0,particle->Phi,particle->M);
        wzlVectors.neutrino.SetPtEtaPhiM(particle->PT, particle->Eta,particle->Phi,
                                   particle->M);   
        }
}
unsigned int WZEvent::getNumPostCutJets()
{
    return particleCounts["jetsPostCut"];
}
void WZEvent::foundLeptonicZ()
{
    wzlVectors.Z.SetPtEtaPhiM(particleMother->PT,particleMother->Eta,
                              particleMother->Phi,particleMother->M);
}
void WZEvent::foundLeptonicW()
{
    wzlVectors.W.SetPtEtaPhiM(particleMother->PT,particleMother->Eta,
                              particleMother->Phi,particleMother->M);
}
bool WZEvent::hasLeptonicZ()
{
    return hasLepZ;
}
void WZEvent::removeCuts()
{
    cuts = {0.};
}
void WZEvent::resetEvent()
{
    particle = NULL;
    particleMother = NULL;
    hasLepZ = false;
    wzlVectors.allLeptons.clear();
    wzlVectors.allJets.clear();
    particleCounts["jets"] = 0;
    particleCounts["jetsPostCut"] = 0;
    particleCounts["leptons"] = 0;
    particleCounts["leptonsPostCut"] = 0;
    particleCounts["muons"] = 0;
    particleCounts["muonsPostCut"] = 0;
    particleCounts["electrons"] = 0;
    particleCounts["electronsPostCut"] = 0;
    particleCounts["taus"] = 0;
    particleCounts["tausPostCut"] = 0;
}
    

