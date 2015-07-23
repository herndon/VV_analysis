#include "DataObjects/include/WZEvent.h"
#include "Services/include/Exception.h"
#include <cmath>
#include <iostream>

vvana::WZEvent::WZEvent(const char* lheFileName)
{
    weights = new LHEWeights(lheFileName);
    useWeights = true;
    resetEvent();
}
vvana::WZEvent::WZEvent()
{
    useWeights = false;
    weights = new LHEWeights("");
    resetEvent();
}
vvana::WZEvent::~WZEvent()
{
    //delete weights;
}
const std::vector<float>& vvana::WZEvent::getWeightsVector()
{
    return weights->getVector();
}
const vvana::LHEWeights* vvana::WZEvent::getLHEWeights()
{
    return weights;
}
const std::vector<std::string>& vvana::WZEvent::getWeightNames()
{
    return weights->getNames();
}
void vvana::WZEvent::loadEvent(TClonesArray* branchGenParticle,TClonesArray* branchWeights)
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
      {
	std::vector<float> newWeights;
	for (int ii = 0; ii<branchWeights->GetEntriesFast();++ii)
	  {
	    TRootWeight* weight = (TRootWeight*) branchWeights->At(ii);
           newWeights.push_back(weight->Weight);
	    weight = NULL;
	  }
	weights->setWeights(newWeights);
      }

    //weights->readWeights();

}
float vvana::WZEvent::getSMWeight()
{
    if(weights == NULL)
        return 1.;
    else
        return weights->getSMWeight();
}
const unsigned int vvana::WZEvent::getSMWeightPos()
{
    return weights->getSMWeightPos();
}
float vvana::WZEvent::getWZInvMass()
{
    if (hasLepZ)
        return (wzlVectors.W + wzlVectors.Z).M();
    else
        return - 1;
}
float vvana::WZEvent::get4lMass()
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
      throw vvana::Exception("WZEvent::get4lMass: Wrong number of Leptons");
    } 
}
float vvana::WZEvent::getWZTransMass()
{
    if (!hasLepZ)
        return -1;
    else
        return get4lTransMass();
}
float vvana::WZEvent::get4lTransMass()
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
       throw vvana::Exception("WZEvent::get4lTransMass: Wrong number of Leptons");
    }
}
float vvana::WZEvent::getDiJetInvMass()
{
    if(wzlVectors.allJets.size() >= 2)
        return (wzlVectors.allJets[0] + wzlVectors.allJets[1]).M();
    else
       throw vvana::Exception("WZEvent::getDiJetInvMass: Less than two jets");
}
float vvana::WZEvent::getJetDeltaEta()
{
    if(wzlVectors.allJets.size() == 2) 
        return std::abs(wzlVectors.allJets[0].Eta() - wzlVectors.allJets[1].Eta());
    else
    {
        throw vvana::Exception("WZEvent::getJetDeltaEta: Less than two jets");
    }
}
float vvana::WZEvent::getZpt()
{
    if (hasLepZ)
        return wzlVectors.Z.Pt();
    else
        return -1;
}
unsigned int vvana::WZEvent::getNumWeights()
{
    if(weights == NULL)
        return 0;
    else
        return weights->getNumWeights();
}
void vvana::WZEvent::setLeptonCuts(float leptonPt, float leptonEta)
{
    cuts.leptonPt = leptonPt;
    cuts.leptonEta = leptonEta;
}
void vvana::WZEvent::setJetCuts(float jetPt, float jetEta)
{
    cuts.jetPt = jetPt;
    cuts.jetEta = jetEta;
}
unsigned int vvana::WZEvent::getNumLeptons()
{
    return particleCounts["leptons"];
}
unsigned int vvana::WZEvent::getNumElectrons()
{
    return particleCounts["electrons"];
}
unsigned int vvana::WZEvent::getNumMuons()
{
    return particleCounts["muons"];;
}
unsigned int vvana::WZEvent::getNumPostCutMuons()
{
    return particleCounts["muonsPostCut"];;
}
unsigned int vvana::WZEvent::getNumPostCutElectrons()
{
    return particleCounts["electronsPostCut"];
}
unsigned int vvana::WZEvent::getNumPostCutTaus()
{
    return particleCounts["tausPostCut"];
}
unsigned int vvana::WZEvent::getNumPostCutLeptons()
{
    return particleCounts["leptonsPostCut"];
}
float vvana::WZEvent::getWMass()
{
    return wzlVectors.W.M();
}
float vvana::WZEvent::getZMass()
{
    return wzlVectors.Z.M();
}
float vvana::WZEvent::getMET()
{
    return wzlVectors.MET.Pt();
}
std::vector<vvana::ParticleVector>& vvana::WZEvent::getAllLeptons()
{
    return wzlVectors.allLeptons;
}
std::vector<vvana::ParticleVector>& vvana::WZEvent::getAllJets()
{
    return wzlVectors.allJets;
}

void vvana::WZEvent::foundLepton()
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
       throw vvana::Exception("WZEvent::getFoundLepton: Not a known charged lepton");
    }
}
void vvana::WZEvent::processWZLepton(std::string type)
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
	  throw vvana::Exception("WZEvent::processWZLepton: Not a known charged lepton");
        }
    }
}
void vvana::WZEvent::foundJet()
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
void vvana::WZEvent::foundMET()
{
    if(particle->PT > 0) {
        wzlVectors.MET.SetPtEtaPhiM(particle->PT, 0.0,particle->Phi,particle->M);
        wzlVectors.neutrino.SetPtEtaPhiM(particle->PT, particle->Eta,particle->Phi,
                                   particle->M);   
        }
}
unsigned int vvana::WZEvent::getNumPostCutJets()
{
    return particleCounts["jetsPostCut"];
}
void vvana::WZEvent::foundLeptonicZ()
{
    wzlVectors.Z.SetPtEtaPhiM(particleMother->PT,particleMother->Eta,
                              particleMother->Phi,particleMother->M);
}
void vvana::WZEvent::foundLeptonicW()
{
    wzlVectors.W.SetPtEtaPhiM(particleMother->PT,particleMother->Eta,
                              particleMother->Phi,particleMother->M);
}
bool vvana::WZEvent::hasLeptonicZ()
{
    return hasLepZ;
}
void vvana::WZEvent::removeCuts()
{
    cuts = {0.};
}
void vvana::WZEvent::resetEvent()
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
    

