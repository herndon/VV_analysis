/**
 * @file WZAnalysis.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief IO class ffor VVEvent
 *
 * This class analyses a VVEvent as a WZ->lnull final state
 */  
#ifndef __WZAnalysis_h__
#define __WZAnalysis_h__

#include "DataObjects/include/Particle.h"

#include <vector>


namespace vvana {

  class VVEvent;
  //  class Particle;
  
 /**
 *
 * @class WZAnalysis
 *
 *
 */
class WZAnalysis
{
 public:

    /// Constructor and destructor
  WZAnalysis(const VVEvent & vvEvent);
  WZAnalysis(bool wzEvent,const VVEvent & vvEvent, int w, int z, const std::vector<int> & zLeptons, const std::vector<int> & wLeptons, int wNeutrino, int wLepton, const std::vector<int> & jets);
  ~WZAnalysis() {};

  bool wzEvent() const {return _wzEvent;}
  const VVEvent & vvEvent() const {return _vvEvent;}

 private:

  bool _wzEvent;
  const VVEvent & _vvEvent;

  // All particles in VVEvent referenced by index
  int _w;
  int _z;
  // Vectors are pT ordered
  std::vector<int> _zLeptons;
  std::vector<int> _wLeptons;
  int _wNeutrino;
  int _wLepton;
  std::vector<int> _jets;
  
};

} // end namespace vvana
 
#endif // __WZAnalysis_h__
