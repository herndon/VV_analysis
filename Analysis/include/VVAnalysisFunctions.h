/**
 * @file VVAnalysisFunctions.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief  Common functions for analyzing a VVEvent
 *
 * Common functions for analyzing a VVEvent
 */  
#ifndef __VVAnalysisFunctions_h__
#define __VVAnalysisFunctions_h__

#include <vector>

namespace vvana {

  class VVEvent;
  
 /**
 *
 * @function VVAnalysisFunctions
 *
 *
 */
  int numberLeptons(const VVEvent& vvEvent);
  int numberChargedLeptons(const VVEvent& vvEvent);
  int numberNeutrinos(const VVEvent& vvEvent);
  int numberZs(const VVEvent& vvEvent);
  int numberWs(const VVEvent& vvEvent);
  int numberJets(const VVEvent& vvEvent);

  const std::vector<int> leptons(const VVEvent& vvEvent);
  const std::vector<int> chargedLeptons(const VVEvent& vvEvent);
  const std::vector<int> neutrinos(const VVEvent& vvEvent);
  const std::vector<int> zs(const VVEvent& vvEvent);
  const std::vector<int> ws(const VVEvent& vvEvent);
  const std::vector<int> jets(const VVEvent& vvEvent);

  const std::vector<int> zlls(const VVEvent& vvEvent);
  const std::vector<int> wlnus(const VVEvent& vvEvent);

  
} // end namespace vvana
 
#endif // __VVAnalysisFunctions_h__
