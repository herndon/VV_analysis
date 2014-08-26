#include "TSystem.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"

#include "WZEvent.h"
#include "WZEventsTracker.h"
#include <iostream>

void AnalyseEvents(ExRootTreeReader *treeReader, float eventWeight);
bool WZMassCalculation(const TLorentzVector& lVectorlW,
                     const TLorentzVector& lVectorMET, Float_t WMass, Float_t pz);

using namespace std;

//------------------------------------------------------------------------------

int main( int argc, char *argv[])
{
    const char* inputFile = "unweighted_events.root";
    TChain *chain = new TChain("LHEF");

    cout << "Processing file " << inputFile << endl;
    chain->Add(inputFile);

    ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
    
    float eventWeight = 1.;
    
    if(argc == 2)
        eventWeight = atof(argv[1]);

    AnalyseEvents(treeReader, eventWeight);
    
    delete treeReader;
    delete chain;
    
    return 0;   
}
//------------------------------------------------------------------------------
void AnalyseEvents(ExRootTreeReader *treeReader, float eventWeight)
{
    // Get pointers to branches used in analysis
    TClonesArray *branchGenParticle = treeReader->UseBranch("Particle");
    
    Long64_t allEntries = treeReader->GetEntries();
    cout << "** Chain contains " << allEntries << " events" << endl;
    cout.flush();

    WZEvent wzEvent = WZEvent("unweighted_events.lhe");
    wzEvent.setLeptonCuts(20, 2.4);
    wzEvent.setJetCuts(30, 4.7);
   
    WZEventsTracker generatorEvents(&wzEvent, "generatorWeights.root", 100000.);
    generatorEvents.setMetCut(30);
    generatorEvents.setZMassCut(20);
    //in inverse picobarns
    generatorEvents.setLuminosity(100000.);
    
    WZEventsTracker selectionEvents(&wzEvent, "selectionWeights.root", 100000.);
    selectionEvents.setLuminosity(100000.);
    selectionEvents.setMetCut(30);
    //selectionEvents.setZMassCut(20);
    //selectionEvents.setWZTMassCut(1200);
    selectionEvents.setJetMassCut(600);
    selectionEvents.setEtajjCut(4.);

    
    // Loop over all events
    for(unsigned int entry = 0; entry < allEntries; ++entry) 
    {
        // Load selected branches with data from specified event
        //Updates entry pointed to by branchGenParticle and branchEvent 
        
        treeReader->ReadEntry(entry);
	    
        wzEvent.resetEvent();
        wzEvent.loadEvent(branchGenParticle);
       
        generatorEvents.processEvent();
        selectionEvents.processEvent();
    }
 
    generatorEvents.printEventInfo();
    selectionEvents.printEventInfo();
    
    generatorEvents.writePlotsToFile();
    selectionEvents.writePlotsToFile();
    //delete generatorResult;
    //delete selectionResult;
    //delete generatorEvents;
}
//------------------------------------------------------------------------------
bool WZMassCalculation(const TLorentzVector& lVectorlW,const TLorentzVector& lVectorMET,
                            Float_t WMass, Float_t pz)
{
    Float_t pzp;
    Float_t pzm;

    Float_t mu = (80.387*80.387)/2.0 + lVectorlW.Px()*lVectorMET.Px()
        + lVectorlW.Py()*lVectorMET.Py();
    mu = (WMass*WMass)/2.0 + lVectorlW.Px()*lVectorMET.Px() + lVectorlW.Py()*lVectorMET.Py();
    Float_t t1 = mu*lVectorlW.Pz()/(lVectorlW.Pt()*lVectorlW.Pt());
    Float_t t2 = t1*t1;
    Float_t t3 = (lVectorlW.E()*lVectorlW.E()*lVectorMET.Pt()*lVectorMET.Pt()-t1*t1)
        /(lVectorlW.Pt()*lVectorlW.Pt());

    if (t3<t2) 
    {
        pzp = t1 + sqrt(t2-t3);
        pzm = t1 - sqrt(t2-t3);      
    }
    else
    {
        pzp = t1;
        pzm = t1;      
        //cout << "Root was imaginary" << endl;
    }

    //cout << "pzp " << pzp << " pzm " << pzm << endl;
    //cout << "pz " << pz << endl;
    WMass = 80.387;

    // Try again with my own solution
    Float_t ptlnu =  lVectorlW.Px()*lVectorMET.Px() + lVectorlW.Py()*lVectorMET.Py();
    Float_t a = 4.0*lVectorlW.E()*lVectorlW.E() -4.0*lVectorlW.Pz()*lVectorlW.Pz();
    Float_t b = -8.0*ptlnu*lVectorlW.Pz() -4.0*lVectorlW.Pz()*WMass*WMass;
    Float_t c = 4.0*lVectorlW.E()*lVectorlW.E()*lVectorMET.Pt()*lVectorMET.Pt()
        - 4.0*ptlnu*ptlnu - WMass*WMass*WMass*WMass - 4.0*ptlnu*WMass*WMass;

    t2 = b*b;
    t3 = 4.0*a*c;

    if (t3<t2)
    {
        pzp = (-b + sqrt(t2-t3))/(2.0*a);
        pzm = (-b - sqrt(t2-t3))/(2.0*a);      
        //cout << "New Root was real" << endl;
    }
    else
    {
        pzp = -b/(2.0*a);
        pzm = -b/(2.0*a);      
        //cout << "New Root was imaginary" << endl;
    }
    Float_t pzsmall;
    Float_t pzlarge;

    if (fabs(pzp) < fabs(pzm))
    {
        pzsmall = pzp;
        pzlarge = pzm;
    }
    else
    {
        pzsmall = pzm; 
        pzlarge = pzp;
    }
    
    return (fabs(pzsmall-pz) < fabs(pzlarge-pz));
}

//------------------------------------------------------------------------------

