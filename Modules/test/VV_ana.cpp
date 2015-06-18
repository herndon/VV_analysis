#include "TROOT.h"
#include "TSystem.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "ExRootAnalysis/include/ExRootTreeReader.h"
#include "ExRootAnalysis/include/ExRootTreeWriter.h"
#include "ExRootAnalysis/include/ExRootTreeBranch.h"
#include "ExRootAnalysis/include/ExRootResult.h"
#include "ExRootAnalysis/include/ExRootUtilities.h"

#include "DataObjects/include/WZEvent.h"
#include "DataObjects/include/WZEventsTracker.h"
#include "DataObjects/include/WZEventList.h"
#include <iostream>
#include <vector>

bool WZMassCalculation(const TLorentzVector& lVectorlW,
                     const TLorentzVector& lVectorMET, Float_t WMass, Float_t pz);
void AnalyseEvents(std::vector<WZEventList>& eventLists);

//------------------------------------------------------------------------------

int main( int argc, char *argv[])
{
    const char* kDefaultRootFile = "unweighted_events.root";
    const char* kDefaultLHEFile = "unweighted_events.lhe";
    std::vector<const char*> root_files;
    std::vector<const char*> lhe_files;
    
     if (argc == 1) {
        root_files.push_back(kDefaultRootFile);
        lhe_files.push_back(kDefaultLHEFile);
    }
    else if (argc == 2) {
        std::string root_file = const_cast<const char*>(argv[1]);
        if (root_file.find(".root") != std::string::npos) {
            root_files.push_back(root_file.c_str());
            lhe_files.push_back("NONE");
        }
        else 
            std::cout << "First CL argument must be .root file";
    }
    else if ((argc - 1) % 2 == 0) {
        for (int i = 1; i < argc - 1; i += 2) {
            std::string root_file = argv[i];
            std::string lhe_file = argv[i+1];
            if (root_file.find(".root") != std::string::npos &&
                lhe_file.find(".lhe") != std::string::npos) {
                lhe_files.push_back(argv[i+1]);
                root_files.push_back(argv[i]);
            }
            else {
                std::cout << "Must have command line arguments in pairs: "
                          << "./WpZ_ana rootfile1.root lhefile1.lhe ...";
                exit(0);
            }
        }
        if (argc > 3) {
            std::cout << "\nWARNING! Combining multiple LHE files. It is your "
                      << "responsiblity to check that these files are " 
                      << "compatible!!!\n\n";
        }
    }
    else {
        std::cout << "Must have command line arguments in pairs: "
                  << "./WpZ_ana rootfile1.root lhefile1.lhe ...";
        exit(0);
    }
    std::vector<WZEventList> event_lists;
    for (unsigned int i = 0; i < root_files.size(); i++) {
        if (root_files.size() != lhe_files.size()) {
            std::cout << "\nERROR! Must have same number of root files "
                      << "as LHE files.\n";
            exit(0);
        }
        WZEventList event_list(root_files[i], lhe_files[i]);
        event_lists.push_back(event_list);
    }
    AnalyseEvents(event_lists);
    
    return 0;   
}
//------------------------------------------------------------------------------
void AnalyseEvents(std::vector<WZEventList>& eventLists)
{
    // Get pointers to branches used in analysis
   
    WZEventsTracker generatorEvents(eventLists[0].getWeightNames(), 
                                    "generatorWeights.root", 100000.);
    WZEventsTracker selectionEvents(eventLists[0].getWeightNames(),
                                    "selectionWeights.root", 100000.);
    generatorEvents.setMetCut(30);
    //generatorEvents.setZMassCut(20);
    //in inverse picobarns
    
    //selectionEvents.setMetCut(30);
    selectionEvents.setZMassCut(20);
    //selectionEvents.setWZMassCut(1000);
    selectionEvents.setJetMassCut(600);
    selectionEvents.setEtajjCut(4.);

    WZEvent* event;
    for (auto& eventList : eventLists) {
        std::cout << "Event list contains " << eventList.getNumEntries() << " events" 
             << std::endl;
        std::cout.flush();
        eventList.setLeptonCuts(20, 2.4);
        eventList.setJetCuts(30, 4.7);
        
        for(unsigned int entry = 0; entry < eventList.getNumEntries(); ++entry) 
        {
            event = eventList.getEvent(entry);
            generatorEvents.processEvent(event);
            selectionEvents.processEvent(event);
        }
    }
 
    generatorEvents.printEventInfo();
    selectionEvents.printEventInfo();
    
    generatorEvents.writePlotsToFile();
    selectionEvents.writePlotsToFile();
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
        //std::cout << "Root was imaginary" << endl;
    }

    //std::cout << "pzp " << pzp << " pzm " << pzm << endl;
    //std::cout << "pz " << pz << endl;
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
        //std::cout << "New Root was real" << endl;
    }
    else
    {
        pzp = -b/(2.0*a);
        pzm = -b/(2.0*a);      
        //std::cout << "New Root was imaginary" << endl;
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

