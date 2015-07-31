/**
 * @file VVAnalysisFunctions.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief Functions for building WZAnalysis
 *
 * Common functions for analyzing a VVEvent
 */  
#ifndef __VVAnalysisFunctions_h__
#define __VVAnalysisFunctions_h__

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

  
} // end namespace vvana
 
#endif // __VVAnalysisFunctions_h__
