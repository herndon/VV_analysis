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
#include "DataObjects/include/VVEvent.h"
#include "Utilities/include/VVEventIO.h"
#include "Analysis/include/WZAnalysis.h"
#include "Modules/include/AnalyseEventsModule.h"
#include <iostream>


//------------------------------------------------------------------------------

vvana::AnalyseEventsModule::AnalyseEventsModule(int debugLevel,std::ofstream & debugfile):
  _debugLevel(debugLevel),
  _debugfile(debugfile)
{
}

//------------------------------------------------------------------------------
void vvana::AnalyseEventsModule::AnalyseEvents(std::vector<WZEventList>& eventLists)
{
    // Get pointers to branches used in analysis
   
  WZEventsTracker generatorEvents(_debugLevel,_debugfile,
				  eventLists[0].getWeightNames(), 
                                    "generatorWeights.root", 100000.);
  WZEventsTracker selectionEvents(_debugLevel,_debugfile,
				  eventLists[0].getWeightNames(),
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

      if (_debugLevel >=1) _debugfile << "Event list contains " << eventList.getNumEntries() << " events" 
				      << std::endl;
      eventList.setLeptonCuts(20, 2.4);
      eventList.setJetCuts(30, 4.7);
        
      for(unsigned int entry = 0; entry < eventList.getNumEntries(); ++entry) 
        {
      // new infrastructure
	  event = eventList.getEvent(entry);
	  // VVEventIO vvEventIO;
	  // VVEvent vvEvent(vvEventIO.readVVEvent(eventList.branch_gen_particle,eventList.branchWeights));
	  // WZAnalysis wzAnalysis(vvEvent);
	  // std::cout << "wzEvent: " << wzAnalysis.wzEvent() << std::endl;
          // vvEvent.printEvent();
	  
	  generatorEvents.processEvent(event);
	  selectionEvents.processEvent(event);
        }
    }
 
    if (_debugLevel >= 2) generatorEvents.printEventInfo(_debugfile);
    if (_debugLevel >= 2) selectionEvents.printEventInfo(_debugfile);
    
    generatorEvents.writePlotsToFile();
    selectionEvents.writePlotsToFile();
}
//------------------------------------------------------------------------------
bool vvana::WZMassCalculation(const TLorentzVector& lVectorlW,const TLorentzVector& lVectorMET,
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
    }

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
    }
    else
    {
        pzp = -b/(2.0*a);
        pzm = -b/(2.0*a);      
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

