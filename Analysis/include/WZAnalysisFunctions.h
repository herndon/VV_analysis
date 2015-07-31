/**
 * @file WZAnalysisFunctions.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief Functions for building WZAnalysis
 *
 * These functions analyses a VVEvent as a WZ->lnull final state
 */  
#ifndef __WZAnalysisFunctions_h__
#define __WZAnalysisFunctions_h__



namespace vvana {

  class VVEvent;
  class WZAnalysis;
  
 /**
 *
 * @function WZAnalysisFunctions
 *
 *
 */
  const WZAnalysis BuildWZAnalysis(const VVEvent& vvEvent);

  
} // end namespace vvana
 
#endif // __WZAnalysisFunctions_h__
