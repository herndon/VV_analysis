#ifndef __AnalyseEvent_h__
#define __AnalyseEvent_h__
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

#include <vector>
#include <fstream>
#include <iostream>

class TLorentzVector;

namespace vvana {

class WZEventList;

class AnalyseEventsModule
{
 public:

  AnalyseEventsModule(int debugLevel, std::ofstream & debugfile);

  void AnalyseEvents(std::vector<WZEventList>& eventLists);

 private:

  int _debugLevel;
  std::ofstream & _debugfile;

};

bool WZMassCalculation(const TLorentzVector& lVectorlW,
                     const TLorentzVector& lVectorMET, Float_t WMass, Float_t pz);

} // end namespace vvana  

#endif //__Analyse_Event_h__
