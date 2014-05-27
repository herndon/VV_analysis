#include "WZEvent.h"

WZEvent()
{
    WZEvent(NULL, NULL);
}
WZEvent(TRootLHEFParticle* currentParticle, TRootLHEFParticle* mother);
{
    resetEvent();
    particle = currentParticle;
    particleMother = mother;
}
bool particleIsStable()
{
    return (particle.Status() == 1);
}
bool particleIsLepton()
{
    return (abs(particle.PID) == 11 || abs(particle.PID == 13); 
}
bool isGeneratedParticle(int particleCount)
{
    return (particle->Mother1 < particleCount + 2) 
}
void foundLepton()
{
    counter.leptons++;
    GeneratorParticle lepton;
    lepton.pt = particle->PT;
    lepton.eta = particle->eta

    switch(abs(particle->PID))
    {
        case 11:
            processWZLeptons(&this.counter.electrons, &this.counter.electrons_ptCut, 11);
            allMuons.push_back(lepton);
            break;
        case 13:
            processWZLeptons(&this.counter.muons, &this.counter.muons_ptCut, 13);  
            allMuons.push_back(lepton);
            break;
        default:
            cout << "A problem occured processing leptons in WZEvent class."
            exit(0);
    }
}

void processWZLeptons(int* typeCounter, int* typePtCut_counter)
{
    &typeCounter++;

    if(particle->PID == 24)
    {    
        WZlVectors.leptonFromW.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle->M);
        WMass = particleM->M;
    }
    if(fabs(particle->Eta) < cuts.leptonEta && particle->PT < cuts.leptonPt) 
    {
        &typePtCut_counter++;
        switch(counter.leptons)
        {
            case 1:
                WZlVectors.lepton1.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle-M);
                //lepton1Type = type;
                break;
            case 2: 
                WZlVectors.lepton2.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle-M);
                //lepton2Type = type;
                break;
            case 3:
                WZlVectors.lepton2.SetPtEtaPhiM(particle->PT,
                        particle->Eta,particle->Phi,particle-M);
                //lepton3Type = type;
                break;
            default:
                cout << "A critical error occured in processLepton() function."
                exit(0);
         }
    }
}
void resetEvent()
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
}

