/**
 * @file AnalyzeEvent.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 * @author Kenneth Long <kdlong@hep.wisc.edu>
 *
 * @brief This class analyzes a single WZ event
 *
 * Planned upgrade to work with a general VV state
 */  
#ifndef __AnalyseEvent_h__
#define __AnalyseEvent_h__

#include <vector>

class TLorentzVector;
class WZEventList;

class AnalyseEventsModule
{
 public:

  AnalyseEventsModule(int debugLevel);

  void AnalyseEvents(std::vector<WZEventList>& eventLists);

 private:

  int _debugLevel;

};

bool WZMassCalculation(const TLorentzVector& lVectorlW,
                     const TLorentzVector& lVectorMET, Float_t WMass, Float_t pz);


#endif //__Analyse_Event_h__