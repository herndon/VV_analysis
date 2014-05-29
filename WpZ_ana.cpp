/*
root -b rootlogin.C
gSystem->Load("libExRootAnalysis.so");
.L WpZ_ana.C+
WpZ_ana("1")


WpZ_ana("unweighted_events.lhe.root")


root -l WpZ_ana.C\(\".root\"\)
*/

#include "TH1.h"
#include "TF1.h"
#include "TSystem.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TClonesArray.h"
//#include "TLorentzVector.h"



#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"
//#include "external/ExRootAnalysis/ExRootClasses.h"


//#include "iostream"
//#include <vector>
#include "fstream"
#include "readWeights.h" 
#include "WZEvent.h"

struct MyPlots;
void AnalyseEvents(ExRootTreeReader *treeReader, MyPlots *plots,
                                   const char* inputFile, int NUM_WEIGHTS);
void WpZ_ana(const char *inputFile, int NUM_WEIGHTS);
void PrintHistograms(ExRootResult *result, MyPlots *plots);
bool WZMassCalculation(const TLorentzVector& lVectorlW, const TLorentzVector& lVectorMET,
                           Float_t WMass, Float_t pz);

using namespace std;

//------------------------------------------------------------------------------

int main()
{
    WpZ_ana("unweighted_events.root", 31);
    return 0;
}

struct MyPlots
{
    // genrator all region declarations: all phyiscs objects with no selection
    TH1 *gall_electronpt;
    TH1 *gall_muonpt;
    TH1 *gall_electroneta;
    TH1 *gall_muoneta;

    TH1 *gall_jetpt;
    TH1 *gall_jeteta;
    TH1 *gall_met;

    TH1*gall_deltaetajj;
    TH1 *gall_mjj;
    TH1 *gall_zpt;
    TH1 *gall_wztmass;
    TH1 *gall_wzmass;

    // genrator baseline regions 1 declarations. First level "weak" cuts

    TH1 *gbr1_electronpt;
    TH1 *gbr1_muonpt;
    TH1 *gbr1_electroneta;
    TH1 *gbr1_muoneta;

    TH1 *gbr1_jet1pt;
    TH1 *gbr1_jet2pt;
    TH1 *gbr1_jet1eta;
    TH1 *gbr1_jet2eta;

    TH1 *gbr1_met;

    TH1 *gbr1_deltaetajj;
    TH1 *gbr1_mjj;
    TH1 *gbr1_zpt;
    TH1 *gbr1_wztmass;
    TH1 *gbr1_wzmass;

    // genrator baseline regions 2 declarations. Greater sensitivity cuts

    TH1 *gbr2_electronpt;
    TH1 *gbr2_muonpt;
    TH1 *gbr2_electroneta;
    TH1 *gbr2_muoneta;

    TH1 *gbr2_jet1pt;
    TH1 *gbr2_jet2pt;
    TH1 *gbr2_jet1eta;
    TH1 *gbr2_jet2eta;
    TH1 *gbr2_met;
    TH1 *gbr2_deltaetajj;
    TH1 *gbr2_mjj;
    TH1 *gbr2_zpt;
    TH1 *gbr2_wztmass;
    vector<TH1*> gbr2w_wztmass;
    TH1 *gbr2_wzmass;
};

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

void BookHistograms(ExRootResult *result, MyPlots *plots, Int_t NUM_WEIGHTS)
{
  // generator all region 1 booking: all physics objects with no selection
  plots->gall_electronpt = result->AddHist1D(
    "gall_electronpt", "electron P_{T}",
    "electron P_{T}, GeV/c", "number of electrons",
    20, 0.0, 100.0);

  plots->gall_muonpt = result->AddHist1D(
    "gall_muonpt", "muon P_{T}",
    "muon P_{T}, GeV/c", "number of muons",
    20, 0.0, 100.0);

  plots->gall_electroneta = result->AddHist1D(
    "gall_electroneta", "electron eta",
    "electron Eta", "number of electrons",
    20, -4.0, 4.0);

  plots->gall_muoneta = result->AddHist1D(
    "gall_muoneta", "muon Eta",
    "muon Eta", "number of muons",
    20, -4.0, 4.0);

  plots->gall_jetpt = result->AddHist1D(
    "gall_jetpt", "jet P_{T}",
    "jet P_{T}, GeV/c", "number of jets",
    20, 0.0, 200.0);

  plots->gall_jeteta = result->AddHist1D(
    "gall_jeteta", "jet Eta}",
    "jet Eta", "number of jets",
    20, -5.0, 5.0);

 plots->gall_deltaetajj = result->AddHist1D(
    "gall_deltaetajj", "Delta Eta JJ",
    "Delta Eta JJ, GeV", "number of events",
    20, 0.0, 8.0);

  plots->gall_mjj = result->AddHist1D(
    "gall_mjj", "diJet Mass",
    "diJet Mass, GeV", "number of events",
    10, 0.0, 1000.0);

  plots->gall_zpt = result->AddHist1D(
    "gall_zpt", "Z P_{T}",
    "Z p_{T}, GeV/c", "number of events",
    20, 0.0, 1000.0);

  plots->gall_met = result->AddHist1D(
    "gall_met", "Missing E_{T}",
    "Missing E_{T}, GeV", "number of events",
    20, 0.0, 200.0);

 plots->gall_wztmass = result->AddHist1D(
    "gall_wztmass", "WZ Transverse Mass",
    "WZ Transverse Mass, GeV", "number of events",
    20, 0.0, 2000.0);

 plots->gall_wzmass = result->AddHist1D(
    "gall_wzmass", "WZ Mass",
    "WZ Mass, GeV", "number of events",
    1000, 0.0, 10000.0);

  plots->gbr1_electronpt = result->AddHist1D(
    "gbr1_electron_pt", "electron P_{T}",
    "electron P_{T}, GeV/c", "number of electrons",
    20, 0.0, 100.0);

  plots->gbr1_muonpt = result->AddHist1D(
    "gbr1_muonpt", "muon P_{T}",
    "muon P_{T}, GeV/c", "number of muons",
    20, 0.0, 100.0);

  plots->gbr1_electroneta = result->AddHist1D(
    "gbr1_electroneta", "electron eta",
    "electron Eta", "number of electrons",
    20, -4.0, 4.0);

  plots->gbr1_muoneta = result->AddHist1D(
    "gbr1_muoneta", "muon Eta}",
    "muon Eta", "number of muons",
    20, -4.0, 4.0);
 
  plots->gbr1_jet1pt = result->AddHist1D(
    "gbr1_jet1pt", "jet 1 P_{T}",
    "jet 1 P_{T}, GeV/c", "number of jets",
    20, 0.0, 200.0);

  plots->gbr1_jet2pt = result->AddHist1D(
    "gbr1_jet2pt", "jet 2 P_{T}",
    "jet 2 P_{T}, GeV/c", "number of jets",
    20, 0.0, 200.0);

  plots->gbr1_jet1eta = result->AddHist1D(
    "gbr1_jet1eta", "jet Eta}",
    "jet 1 Eta", "number of jets",
    20, -5.0, 5.0);

  plots->gbr1_jet2eta = result->AddHist1D(
    "gbr1_jet2eta", "jet Eta}",
    "jet 2 Eta", "number of jets",
    20, -5.0, 5.0);

  plots->gbr1_met = result->AddHist1D(
    "gbr1_met", "Missing E_{T}",
    "Missing E_{T}, GeV", "number of events",
    20, 0.0, 200.0);
 
  plots->gbr1_deltaetajj = result->AddHist1D(
    "gbr1_deltaetajj", "Delta Eta JJ",
    "Delta Eta JJ, GeV", "number of events",
    20, 0.0, 8.0);

  plots->gbr1_mjj = result->AddHist1D(
    "gbr1_mjj", "diJet Mass",
    "diJet Mass, GeV", "number of events",
    10, 0.0, 1000.0);

  plots->gbr1_zpt = result->AddHist1D(
    "gbr1_zpt", "Z P_{T}",
    "Z p_{T}, GeV/c", "number of events",
    20, 0.0, 1000.0);

  plots->gbr1_wztmass = result->AddHist1D(
    "gbr1_wztmass", "WZ Transverse Mass",
    "WZ Transverse Mass, GeV", "number of events",
    20, 0.0, 2000.0);

  plots->gbr1_wzmass = result->AddHist1D(
    "gbr1_wzmass", "WZ Mass",
    "WZ  Mass, GeV", "number of events",
    1000, 0.0, 10000.0);

  plots->gbr2_electronpt = result->AddHist1D(
    "gbr2_electron_pt", "electron P_{T}",
    "electron P_{T}, GeV/c", "number of electrons",
    20, 0.0, 100.0);

  plots->gbr2_muonpt = result->AddHist1D(
    "gbr2_muonpt", "muon P_{T}",
    "muon P_{T}, GeV/c", "number of muons",
    20, 0.0, 100.0);

  plots->gbr2_electroneta = result->AddHist1D(
    "gbr2_electroneta", "electron eta",
    "electron Eta", "number of electrons",
    20, -4.0, 4.0);

  plots->gbr2_muoneta = result->AddHist1D(
    "gbr2_muoneta", "muon Eta}",
    "muon Eta", "number of muons",
    20, -4.0, 4.0);
 
  plots->gbr2_jet1pt = result->AddHist1D(
    "gbr2_jet1pt", "jet 1 P_{T}",
    "jet 1 P_{T}, GeV/c", "number of jets",
    20, 0.0, 200.0);

  plots->gbr2_jet2pt = result->AddHist1D(
    "gbr2_jet2pt", "jet 2 P_{T}",
    "jet 2 P_{T}, GeV/c", "number of jets",
    20, 0.0, 200.0);

  plots->gbr2_jet1eta = result->AddHist1D(
    "gbr2_jet1eta", "jet Eta}",
    "jet 1 Eta", "number of jets",
    20, -5.0, 5.0);

  plots->gbr2_jet2eta = result->AddHist1D(
    "gbr2_jet2eta", "jet Eta}",
    "jet 2 Eta", "number of jets",
    20, -5.0, 5.0);

  plots->gbr2_met = result->AddHist1D(
    "gbr2_met", "Missing E_{T}",
    "Missing E_{T}, GeV", "number of events",
    20, 0.0, 200.0);
 
  plots->gbr2_deltaetajj = result->AddHist1D(
    "gbr2_deltaetajj", "Delta Eta JJ",
    "Delta Eta JJ, GeV", "number of events",
    20, 0.0, 8.0);

  plots->gbr2_mjj = result->AddHist1D(
    "gbr2_mjj", "diJet Mass",
    "diJet Mass, GeV", "number of events",
    10, 0.0, 1000.0);

  plots->gbr2_zpt = result->AddHist1D(
    "gbr2_zpt", "Z P_{T}",
    "Z p_{T}, GeV/c", "number of events",
    20, 0.0, 1000.0);

  plots->gbr2_wztmass = result->AddHist1D(
    "gbr2_wztmass", "WZ Transverse Mass",
    "WZ Transverse Mass, GeV", "number of events",
    20, 0.0, 2000.0);

  plots->gbr2w_wztmass.resize(NUM_WEIGHTS);

  for (int i = 0; i < NUM_WEIGHTS; i++) 
  {
    plots->gbr2w_wztmass[i] = result->AddHist1D(
    "gbr2w_wztmass", "WZ Transverse Mass",
    "WZ Transverse Mass, GeV", "number of events",
    20, 0.0, 2000.0);
  }

  plots->gbr2_wzmass = result->AddHist1D(
    "gbr2_wzmass", "WZ Mass",
    "WZ  Mass, GeV", "number of events",
    1000, 0.0, 10000.0);






}

//------------------------------------------------------------------------------

void AnalyseEvents(ExRootTreeReader *treeReader, MyPlots *plots, const char* inputFile, int NUM_WEIGHTS)
{
    cout << "Processing file " << inputFile << endl;
  
    bool useWeightInfo = true;
    float weights[NUM_WEIGHTS];
    
    fstream lheFile;
    lheFile.open("unweighted_events.lhe", ios::in | ios::binary);

    // Get pointers to branches used in analysis
    TClonesArray *branchGenParticle = treeReader->UseBranch("Particle");
    TClonesArray *branchEvent = treeReader->UseBranch("Event");

    TRootLHEFParticle *particle;
    TRootLHEFParticle *particleM;
    TRootLHEFEvent *event;
    
    Long64_t allEntries = treeReader->GetEntries();

    cout << "** Chain contains " << allEntries << " events" << endl;
    cout.flush();

    Float_t leptonEtaCut = 2.4;
    Float_t metCut = 30.0;
    Float_t jetPTCut = 30.0;    
    cout << "Using Lepton eta cut " << leptonEtaCut << endl;
    cout << "Using MET        cut " << metCut << endl;
    cout << "Using Jet pt     cut " << jetPTCut << endl;
    cout << "Using jetCut " << jetPTCut << endl;

    vector<Float_t> nwGenWZ_all(NUM_WEIGHTS, 0.);
    vector<Float_t> nwGenWZ_gbr2(NUM_WEIGHTS, 0.);
    vector<Float_t> nwGenWZ_wztmass(NUM_WEIGHTS, 0.);

    Int_t nGenWZ_leptons = 0;
    Int_t nGenWZ_leptons3m = 0;
    Int_t nGenWZ_leptons2m1e = 0;
    Int_t nGenWZ_leptons2e1m = 0;
    Int_t nGenWZ_leptons3e = 0;
    Int_t nGenWZ_jets = 0;
    Int_t nGenWZ_met = 0;
    Int_t nGenWZ_Z = 0;
    Int_t nGenWZ_etajj = 0;
    Int_t nGenWZ_mjj = 0;
    Int_t nGenWZ_wztmass = 0;
    Int_t nGenWZ_wztmass3m = 0;
    Int_t nGenWZ_wztmass2m1e = 0;
    Int_t nGenWZ_wztmass2e1m = 0;
    Int_t nGenWZ_wztmass3e = 0;
    Bool_t genPureSignalRegion;

    Int_t nGenWZPS_leptons = 0;
    Int_t nGenWZPS_leptons3m = 0;
    Int_t nGenWZPS_leptons2m1e = 0;
    Int_t nGenWZPS_leptons2e1m = 0;
    Int_t nGenWZPS_leptons3e = 0;
    Int_t nGenWZPS_jets = 0;
    Int_t nGenWZPS_met = 0;
    Int_t nGenWZPS_Z = 0;
    Int_t nGenWZPS_etajj = 0;
    Int_t nGenWZPS_mjj = 0;
    Int_t nGenWZPS_wztmass = 0;
    Int_t nGenWZPS_wztmass3m = 0;
    Int_t nGenWZPS_wztmass2m1e = 0;
    Int_t nGenWZPS_wztmass2e1m = 0;
    Int_t nGenWZPS_wztmass3e = 0;
    Int_t pCorrect = 0;
    Int_t mCorrect = 0;
    Int_t pCorrectPS = 0;
    Int_t mCorrectPS = 0;

    
    WZEvent wzEvent;
    wzEvent.setLeptonCuts(20, 2.4);
    wzEvent.setJetCuts(30, 4.7);
    // Lorentz vectors

    TLorentzVector lVectorl1, lVectorl2,lVectorl3, lVectorRl, lVectorj1, 
                   lVectorj2, lVectorRj, lVectorZ, lVectorW, lVectorWZ;

    //TLorentzVector* lVectorlW = new TLorentzVector;
    //TLorentzVector* lVectorMET = new TLorentzVector;
    // Loop over all events
    for(unsigned int entry = 0; entry < allEntries; ++entry) 
    {
        // Load selected branches with data from specified event
    	TLorentzVector lVectorlW;
        TLorentzVector lVectorMET;
        //Updates entry pointed to by branchGenParticle and branchEvent 
        treeReader->ReadEntry(entry);
	    
        if ((entry%1000) == 0)  cout << entry << endl;

        if (useWeightInfo) 
        {
            readWeights(NUM_WEIGHTS, weights, lheFile);
    
            for(int i = 0; i < NUM_WEIGHTS; ++i)
                nwGenWZ_all[i] += weights[i]; 
        }  

        // Generator Baseline region 1: 

        Int_t nGenJet = 0;
        Int_t nGenElectron = 0;
        Int_t nGenMuon = 0; 

        Int_t nGenLepton10= 0;
        Int_t nGenLepton20 = 0;
        Int_t nGenElectron20 = 0;
        Int_t nGenMuon20 = 0;
        Int_t nGenJet30 = 0;

        Float_t genMet = 0.0;
        Float_t genZMass = 0.0;

        Bool_t genbr1Event = true;
        Bool_t foundJet1 = false;

 
        Float_t neutrino_pz = 0;
        Float_t WMass = 0;

        Int_t nLepton = 0;

        genPureSignalRegion = false;


        for(int i = 0; i < branchGenParticle->GetEntriesFast(); ++i) 
        {
            particle = (TRootLHEFParticle*) branchGenParticle->At(i);

            wzEvent.setParticle(particle);
            if(wzEvent.particleIsStable())
            {
                if(wzEvent.isGeneratedParticle(i) && wzEvent.particleIsLepton())
                {
                    nLepton++;
                    particleM = (TRootLHEFParticle*) branchGenParticle->At(particle->Mother1-1);
                    wzEvent.setParticleMother(particleM);
                    wzEvent.foundLepton(); 
        	        //plots->gall_electronpt->Fill(particle->PT);
                    //plots->gall_electroneta->Fill(particle->Eta);
    	    	}
                // for W+Z event initial particles end after 4. Status is not 2 which indicates
                // intermediate history Pythia/delphies populates every event with extra substantially 
                // increasing the events that pass
                if (i>5 && (abs(particle->PID) < 6||abs(particle->PID) == 21))
                { 
                    nGenJet++;
                    plots->gall_jetpt->Fill(particle->PT);
                    plots->gall_jeteta->Fill(particle->Eta);
    		
                   if (particle->PT >jetPTCut && fabs(particle->Eta) <4.7) 
                   {
                       nGenJet30++;
                            
                       if (foundJet1) 
                           lVectorj2.SetPtEtaPhiM(particle->PT,particle->Eta,particle->Phi,particle->M);
                        else
                            lVectorj1.SetPtEtaPhiM(particle->PT,particle->Eta,particle->Phi,particle->M);
                        foundJet1 = true;
                    }
                }
                if ((abs(particle->PID) == 12 || abs(particle->PID) == 14) 
    		    		&& particle->PT > genMet) 
                {
                    //cout << "Filled Gen met" << endl;
                    genMet = particle->PT;
                    plots->gall_met->Fill(particle->PT);
                    lVectorMET.SetPtEtaPhiM(particle->PT,0.0,particle->Phi,particle->M);
                    neutrino_pz = particle->Pz;
    		    //cout << "Set MET " << particle->PT << " " << lVectorMET.Pt() << " " << pz << endl;
                }
            }
            if (particle->PID==23)
                genZMass = particle->M;
            if (particle->PID==23) 
                lVectorZ.SetPtEtaPhiM(particle->PT,particle->Eta,particle->Phi,particle->M);
            if (particle->PID==24)  
                lVectorW.SetPtEtaPhiM(particle->PT,particle->Eta,particle->Phi,particle->M);
    	}

        nGenElectron = wzEvent.getGenElectronNumber();
        nGenMuon = wzEvent.getGenMuonNumber();
        nGenElectron20 = wzEvent.getGenElectronPtCutNumber();
        nGenMuon20 = wzEvent.getGenMuonPtCutNumber();
        WMass = wzEvent.getWMass();
       
        nGenLepton20 = nGenElectron20 + nGenMuon20;
        
        if(entry % 100 == 0)
         {
            cout << "Values:\n"
                 << "\nLeptonsClass = " << wzEvent.getGenLeptonNumber()
                 << "LeptonsNoClass = " << nLepton
                 << "\nLeptons20 =" << wzEvent.getGenLeptonPtCutNumber()
                 << "\nnGenElectron =" << nGenElectron
                 << "\nnGenMuon = " << nGenMuon
                 << "\nnGenElectron20 = " << nGenElectron20
                 << "\nnGenMuon20 = " << nGenMuon20;
        }
        if(nGenLepton20 != wzEvent.getGenLeptonPtCutNumber())
        {
                cout << "\nThat ain't good! At entry = ";
                cout << entry << endl;
        }
        
        plots->gall_zpt->Fill(lVectorZ.Pt());
        lVectorRj = lVectorj1+lVectorj2;
        plots->gall_mjj->Fill(lVectorRj.M());
        plots->gall_deltaetajj->Fill(fabs(lVectorj1.Eta()-lVectorj2.Eta()));
		
        //lVectorRl = lVectorl1+lVectorl2+lVectorl3+lVectorMET;
        lVectorRl = wzEvent.getWZleptonSum() + lVectorMET;
        plots->gall_wztmass->Fill(lVectorRl.M());
		
        lVectorWZ = lVectorW + lVectorZ;
        plots->gall_wzmass->Fill(lVectorWZ.M());
		
		
        wzEvent.resetEvent();
        // WZ mass calculation
        // Need to define Wlepton lVectorlW
        bool correctp = WZMassCalculation(lVectorlW, lVectorMET,  WMass, neutrino_pz); 
		

        //cout << "New pzp " << pzp << " pzm " << pzm << endl;
        //cout << "New pz " << pz << endl;
		
        if (correctp)
        {    
            pCorrect++;
            if(lVectorWZ.M() > 1200.)
                pCorrectPS++;
        }
        else
        {
            mCorrect++;
            if(lVectorWZ.M() > 1200.)
                mCorrectPS++;
        }
        Int_t genltype = 0;
        
        if (nGenLepton20 != 3)
            genbr1Event = false;
        if(genbr1Event)    
        {    
            nGenWZ_leptons++;

            if (nGenElectron20 == 3)
            {
                nGenWZ_leptons3e++;
                if(lVectorRl.M() > 1200.)
                {
                    nGenWZPS_leptons++;    
                    nGenWZPS_leptons3m++;
                }
            }
            else if (nGenElectron20 == 2)
            {
                nGenWZ_leptons2e1m++;
                if(lVectorRl.M() > 1200.)
                {
                    nGenWZPS_leptons++;
                    nGenWZPS_leptons2e1m++;
                }
            }
            else if (nGenMuon20 == 2)
            {
                nGenWZ_leptons2m1e++;
                if(lVectorRl.M() > 1200.)
                {
                    nGenWZPS_leptons++;
		            nGenWZPS_leptons2m1e++;
                }
            }
            else if (nGenMuon20 == 3)
            {
                nGenWZ_leptons3m++;
                if(lVectorRl.M() > 1200.)
                {    
                    nGenWZPS_leptons3m++;
                    nGenWZPS_leptons++;
                 }
            }
        }

        if (nGenElectron20 == 3)
            genltype = 4;
        if (nGenElectron20 == 2) 
            genltype = 3;
        if (nGenMuon20 == 2)
            genltype = 2;
        if (nGenMuon20 == 3) 
            genltype = 1;

        if(lVectorRl.M() > 1200.)
            genPureSignalRegion= true;
        if (nGenJet30<2)
            genbr1Event = false;
        if (genbr1Event) 
            nGenWZ_jets++;
        if (genbr1Event&&genPureSignalRegion) 
            nGenWZPS_jets++;
	    if (genMet < metCut) 
            genbr1Event = false;
	    if (genbr1Event) 
            nGenWZ_met++;
	    if (genbr1Event&&genPureSignalRegion)
            nGenWZPS_met++;
	    if (fabs(genZMass-91.1876)>20.0) 
            genbr1Event = false;
        if (genbr1Event) 
            nGenWZ_Z++;
        if (genbr1Event&&genPureSignalRegion) 
            nGenWZPS_Z++;
	    if (nGenJet>2) 
        {
            cout << "Too many jets " << endl;
            for(int i = 0; i < branchGenParticle->GetEntriesFast(); ++i) 
            {
                particle = (TRootLHEFParticle*) branchGenParticle->At(i);
                cout << "Gen particle " << i << " " << particle->PID << " s "<< particle->Status
                     << " m1 " << particle->Mother1 << " m2 " << particle->Mother2 <<  " "
                     << particle->PT <<  " " << particle->Eta << endl;
            }
        }
        if (genbr1Event) 
        {
            plots->gbr1_zpt->Fill(lVectorZ.Pt());
            plots->gbr1_mjj->Fill(lVectorRj.M());
            plots->gbr1_deltaetajj->Fill(fabs(lVectorj1.Eta()-lVectorj2.Eta()));
            plots->gbr1_wztmass->Fill(lVectorRl.M());
            plots->gbr1_wzmass->Fill(lVectorWZ.M());
	
            if (lVectorRj.M()>600.0) 
                nGenWZ_mjj++; 
            if (lVectorRj.M()>600.0 && genPureSignalRegion) 
                nGenWZPS_mjj++;
            if (lVectorRj.M()>600.0 && fabs(lVectorj1.Eta()-lVectorj2.Eta()) > 4.0) 
                nGenWZ_etajj++;
            if (lVectorRj.M()>600.0 && fabs(lVectorj1.Eta()-lVectorj2.Eta()) > 4.0
                    && genPureSignalRegion)
                nGenWZPS_etajj++;
            if (lVectorRj.M()>600.0 && fabs(lVectorj1.Eta()-lVectorj2.Eta()) > 4.0) 
            {
                if (lVectorRl.M()<1200.0)  genbr1Event = false;
                if (genbr1Event) 
                {
                    nGenWZ_wztmass++;
                    if (genltype ==1) 
                        nGenWZ_wztmass3m++;
                    if (genltype ==2) 
                        nGenWZ_wztmass2m1e++;
                    if (genltype ==3) 
                        nGenWZ_wztmass2e1m++;
                    if (genltype ==4) nGenWZ_wztmass3e++;
                }
                if (genbr1Event&&genPureSignalRegion) 
                {
                    nGenWZPS_wztmass++;
                    if (genltype ==1) 
                        nGenWZPS_wztmass3m++;
                    if (genltype ==2) 
                        nGenWZPS_wztmass2m1e++;
                    if (genltype ==3) 
                        nGenWZPS_wztmass2e1m++;
                    if (genltype ==4) 
                        nGenWZPS_wztmass3e++;
                }
                plots->gbr2_zpt->Fill(lVectorZ.Pt());
                plots->gbr2_mjj->Fill(lVectorRj.M());
                plots->gbr2_deltaetajj->Fill(fabs(lVectorj1.Eta()-lVectorj2.Eta()));
                plots->gbr2_wztmass->Fill(lVectorRl.M());
	
                if (useWeightInfo)
                {
                      for (Int_t i=0; i < NUM_WEIGHTS; i++) 
                      {
                          nwGenWZ_gbr2[i] += weights[i];
                          plots->gbr2w_wztmass[i]->Fill(lVectorRl.M(),weights[i]);
                          if (lVectorRl.M() > 1000.0 )
                              nwGenWZ_wztmass[i] += weights[i];
                      }
                }
		        plots->gbr2_wzmass->Fill(lVectorWZ.M());
            }
        }
    }
    //Standard Model cross section in picobarns
    //FT1 = 0 corresponds to SM, which is the 16th parameter used in Cards/reweight_card.dat
    const Float_t sm_crossx = nwGenWZ_all[15];
    const Float_t luminosity = 19.6; //Integrated Luminosity in fb^-1 
    const Float_t scale = sm_crossx*1000*luminosity; //Number of events at given luminosity

    cout << "Processing file " << inputFile << " with scale factor " << scale << endl;
 
    if (useWeightInfo)
    {
        cout << "Gen weighted event counts" << endl;
        for(Int_t i = 0; i < NUM_WEIGHTS; ++i) 
        {
            cout << "Total cross section/Events*       " << i << " " 
	         << nwGenWZ_all[i]*1000*luminosity << endl;
            cout << "Cross section with EWK selection  " << i << " "
	         << nwGenWZ_gbr2[i]*1000*luminosity << endl;
            cout << "Cross section with WZtn selection " << i << " "
	         << nwGenWZ_wztmass[i]*1000*luminosity << endl;
        }
    }
    cout << "Gen event counts" << endl;
    cout << "Gen Baseline selection 1, leptons: " << nGenWZ_leptons << endl;
    cout << "Gen Baseline selection 1, 3m     : " << nGenWZ_leptons3m << endl;
    cout << "Gen Baseline selection 1, 2m1e   : " << nGenWZ_leptons2m1e << endl;
    cout << "Gen Baseline selection 1, 2e1m   : " << nGenWZ_leptons2e1m << endl;
    cout << "Gen Baseline selection 1, 3e     : " << nGenWZ_leptons3e << endl;
    cout << "Gen Baseline selection 1, jets   : " << nGenWZ_jets << endl;
    cout << "Gen Baseline selection 1, MET    : " << nGenWZ_met << endl;
    cout << "Gen Baseline selection 1, Z      : " << nGenWZ_Z << endl;
    cout << "Gen Baseline selection 1, m_jj   : " << nGenWZ_mjj << endl;
    cout << "Gen Baseline selection 1, eta jj : " << nGenWZ_etajj << endl;
    cout << " " << endl;
    cout << " Gen event counts scale by lumi and cross section" << endl;
    cout << "Gen Baseline selection 1, leptons: " << scale*nGenWZ_leptons << endl;
    cout << "Gen Baseline selection 1, 3m     : " << scale*nGenWZ_leptons3m << endl;
    cout << "Gen Baseline selection 1, 2m1e   : " << scale*nGenWZ_leptons2m1e << endl;
    cout << "Gen Baseline selection 1, 2e1m   : " << scale*nGenWZ_leptons2e1m << endl;
    cout << "Gen Baseline selection 1, 3e     : " << scale*nGenWZ_leptons3e << endl;
    cout << "Gen Baseline selection 1, jets   : " << scale*nGenWZ_jets << endl;
    cout << "Gen Baseline selection 1, MET    : " << scale*nGenWZ_met << endl;
    cout << "Gen Baseline selection 1, Z      : " << scale*nGenWZ_Z << endl;
    cout << "Gen Baseline selection 1, m_jj   : " << scale*nGenWZ_mjj << endl;
    cout << "Gen Baseline selection 1, eta jj : " << scale*nGenWZ_etajj << endl;

    //cout << "Gen Baseline selection 1, opp eta: " << nGenWZ_oppetajj << endl;
    cout << "Gen Baseline selection 1, wztmas : " << scale*nGenWZ_wztmass << endl;
    cout << "Gen Baseline selection 1, 3m     : " << scale*nGenWZ_wztmass3m << endl;
    cout << "Gen Baseline selection 1, 2m1e   : " << scale*nGenWZ_wztmass2m1e << endl;
    cout << "Gen Baseline selection 1, 2e1m   : " << scale*nGenWZ_wztmass2e1m << endl;
    cout << "Gen Baseline selection 1, 3e     : " << scale*nGenWZ_wztmass3e << endl;

    cout << "Gen Baseline selection PS, leptons: " << nGenWZPS_leptons << endl;
    cout << "Gen Baseline selection PS, 3m     : " << nGenWZPS_leptons3m << endl;
    cout << "Gen Baseline selection PS, 2m1e   : " << nGenWZPS_leptons2m1e << endl;
    cout << "Gen Baseline selection PS, 2e1m   : " << nGenWZPS_leptons2e1m << endl;
    cout << "Gen Baseline selection PS, 3e     : " << nGenWZPS_leptons3e << endl;
    cout << "Gen Baseline selection PS, jets   : " << nGenWZPS_jets << endl;
    cout << "Gen Baseline selection PS, MET    : " << nGenWZPS_met << endl;
    cout << "Gen Baseline selection PS, Z      : " << nGenWZPS_Z << endl;
    cout << "Gen Baseline selection PS, m_jj   : " << nGenWZPS_mjj << endl;
    cout << "Gen Baseline selection PS, eta jj : " << nGenWZPS_etajj << endl;

    //cout << "Gen Baseline selection PS, opp eta: " << nGenWZPS_oppetajj << endl;
    cout << "Gen Baseline selection PS, wztmas : " << nGenWZPS_wztmass << endl;
    //cout << "Matching selection num " << nMatchedNum << endl;
    //cout << "Matching selection denom " << nMatchedDenom << endl;
    cout << "Correct + " << pCorrect << " - " << mCorrect << endl;
    cout << "Correct PS + " << pCorrectPS << " - " << mCorrectPS << endl;
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
    if (t3>t2) 
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
    if (t3>t2)
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
    if (fabs(pzp) > fabs(pzm))
    {
        pzsmall = pzm; 
        pzlarge = pzp;
    }
    
    return (fabs(pzsmall-pz) < fabs(pzlarge-pz));
}
void PrintHistograms(ExRootResult *result, MyPlots *plots)
{
  result->Print("png");
}

//------------------------------------------------------------------------------

void WpZ_ana(const char* inputFile, int NUM_WEIGHTS)
{
    TChain *chain = new TChain("LHEF");
    const char * outputFile = "results_wpz_ewk.root";

    cout << "Processing file " << inputFile << endl;
    chain->Add(inputFile);

    ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
    ExRootResult *result = new ExRootResult();

    MyPlots *plots = new MyPlots;

    BookHistograms(result, plots, NUM_WEIGHTS);
    AnalyseEvents(treeReader, plots, inputFile, NUM_WEIGHTS);

    //PrintHistograms(result, plots);

    // 1 default
    // 2 no opp eta and no met cut for 3at-1

    result->Write(outputFile);

    cout << "** Exiting..." << endl;
    
    delete plots;
    delete result;
    delete treeReader;
    delete chain;
}
//------------------------------------------------------------------------------

