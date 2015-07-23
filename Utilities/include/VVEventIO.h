/**
 * @file VVEventIO.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief IO class ffor VVEvent
 *
 * This class reads and stores all particle information from a single 
 * VV generated event as a VVEvent object. 
 */  
#ifndef __VVEvent_h__
#define __VVEvent_h__

#include "TClonesArray.h"

class VVEvent;

/**
 *
 * @class VVEventIO
 *
 *
 */
class VVEventIO
{
 public:

    /// Constructor and destructor
  VVEventIO();
  ~VVEventIO();

  const VVEvent readVVEvent(TClonesArray* branchGenParticle,TClonesArray* branchWeights);
  
 private: 

};

#endif // __VVEvent_h__
