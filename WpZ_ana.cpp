#include "TSystem.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"

#include "iostream"
#include <vector>
#include "fstream"
#include "readWeights.h" 
#include "WZEvent.h"
#include "WZEventsTracker.h"

void AnalyseEvents(ExRootTreeReader *treeReader, const char* inputFile, int NUM_WEIGHTS);
void WpZ_ana(const char *inputFile, int NUM_WEIGHTS);
bool WZMassCalculation(const TLorentzVector& lVectorlW,
                     const TLorentzVector& lVectorMET, Float_t WMass, Float_t pz);

using namespace std;

//------------------------------------------------------------------------------

int main()
{
    WpZ_ana("unweighted_events.root", 31);
    return 0;
}
void AnalyseEvents(ExRootTreeReader *treeReader, const char* inputFile, int NUM_WEIGHTS)
{
    cout << "Processing file " << inputFile << endl;
  
    bool useWeightInfo = true;
    vector<float> weights;
    //weights.resize(NUM_WEIGHTS);
    
    fstream lheFile;
    lheFile.open("unweighted_events.lhe", ios::in | ios::binary);

    // Get pointers to branches used in analysis
    TClonesArray *branchGenParticle = treeReader->UseBranch("Particle");
    
    Long64_t allEntries = treeReader->GetEntries();
    cout << "** Chain contains " << allEntries << " events" << endl;
    cout.flush();

    vector<float> nwGenWZ_all(NUM_WEIGHTS, 0.);
    vector<float> nwGenWZ_gbr2(NUM_WEIGHTS, 0.);
    vector<float> nwGenWZ_wztmass(NUM_WEIGHTS, 0.);
    WZEvent wzEvent = WZEvent();
    wzEvent.setLeptonCuts(20, 2.4);
    wzEvent.setJetCuts(30, 4.7);

    
    ExRootResult* selectionResult = new ExRootResult;
    ExRootResult* generatorResult = new ExRootResult;

    WZEventsTracker generatorEvents(generatorResult, 31, "generator", true);
    generatorEvents.setLeptonSelection(3);
    generatorEvents.setJetSelection(2);
    generatorEvents.setMetCut(30);
    generatorEvents.setZMassCut(20);
    
    WZEventsTracker selectionEvents(selectionResult, 31, "selection", true);
    selectionEvents.setLeptonSelection(3);
    selectionEvents.setJetSelection(2);
    selectionEvents.setMetCut(30);
    selectionEvents.setZMassCut(20);
   // selectionEvents.setWZTMassCut(1200);
    selectionEvents.setJetMassCut(600);
    selectionEvents.setEtajjCut(4.);

    
    // Loop over all events
    for(unsigned int entry = 0; entry < allEntries; ++entry) 
    {
        // Load selected branches with data from specified event
        //Updates entry pointed to by branchGenParticle and branchEvent 
        treeReader->ReadEntry(entry);
	    
        if ((entry%1000) == 0)  cout << entry << endl;

        if (useWeightInfo) 
        {
            readWeights(weights, lheFile);
            
            for(int i = 0; i < NUM_WEIGHTS; i++)
            {
                nwGenWZ_all[i] += weights[i]; 
            }
        }  

        wzEvent.resetEvent();
        wzEvent.loadEvent(branchGenParticle);
       
        generatorEvents.processEvent(&wzEvent, weights);
        selectionEvents.processEvent(&wzEvent, weights);
    }
    //Standard Model cross section in picobarns
    //FT1 = 0 corresponds to SM, which is the 16th parameter used in Cards/reweight_card.dat
    //const Float_t sm_crossx = nwGenWZ_all[15];
    //const Float_t luminosity = 19.6; //Integrated Luminosity in fb^-1 
    //float scale = sm_crossx*1000*luminosity; //Number of events at given luminosity
 
    generatorEvents.printEventInfo();
    selectionEvents.printEventInfo();
    
    generatorResult->Write("generatorResult.root");
    selectionResult->Write("selectionResult.root");
    delete generatorResult;
    delete selectionResult;
}
//------------------------------------------------------------------------------
void WpZ_ana(const char* inputFile, int NUM_WEIGHTS)
{
    TChain *chain = new TChain("LHEF");
    //const char * outputFile = "results_wpz_ewk.root";

    cout << "Processing file " << inputFile << endl;
    chain->Add(inputFile);

    ExRootTreeReader *treeReader = new ExRootTreeReader(chain);


    AnalyseEvents(treeReader, inputFile, NUM_WEIGHTS);
    
    cout << "** Exiting..." << endl;
    
    delete treeReader;
    delete chain;
   
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

