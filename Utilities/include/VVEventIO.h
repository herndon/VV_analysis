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
#ifndef __VVEventIO_h__
#define __VVEventIO_h__

#include "TClonesArray.h"
#include "DataObjects/include/VVEvent.h"

namespace vvana {

  //class VVEvent;
  class Particle;
  

  enum particleType{electron,muon,tau,neutrino,W,Z,photon,jet};

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
  VVEventIO() {};
  ~VVEventIO() {};

  const VVEvent readVVEvent(TClonesArray* branchGenParticle,TClonesArray* branchWeights);

  void getParticles( particleType type, TClonesArray* branchGenParticle, std::vector<Particle> & particles);
  
 private: 

};

} // end namespace vvana
 
#endif // __VVEventIO_h__
