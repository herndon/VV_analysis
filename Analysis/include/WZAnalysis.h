/**
 * @file WZAnalysis.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief IO class ffor VVEvent
 *
 * This class reads and stores all particle information from a single 
 * VV generated event as a VVEvent object. 
 */  
#ifndef __WZAnalysis_h__
#define __WZAnalysis_h__

#include <vector>


namespace vvana {

  class VVEvent;
  
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
  ~WZAnalysis() {};

  bool wzEvent() const {return _wzEvent;}
  const VVEvent & vvEvent() const {return _vvEvent;}

 private:

  bool _wzEvent;
  const VVEvent & _vvEvent;
  /* const Particle & _w; */
  /* const Particle & _z; */
  /* // Vectors are pT ordered */
  /* const std::vector<const Particle &> _wLeptons; */
  /* const Partcile & _wNeutrino; */
  /* const Particle & _wLepton; */
  /* const std::vector<const Particle &> _zLetpons; */
  /* const std::vector<const Particle &> _jets; */
  
};

} // end namespace vvana
 
#endif // __WZAnalysis_h__
