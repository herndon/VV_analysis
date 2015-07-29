#ifndef __VVEvent_h__
#define __VVEvent_h__
/**
 * @file VVEvent.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief Stores the particle information from a generator VV event.
 *
 * This class reads and stores all particle information from a single 
 * VV generated event. It stores simple kinematic information on all leptons
 * and jets in the event (p_{T} and eta) as well as TLorentzVectors of all 
 * V decay leptons and V particles (identified by pt and eta cuts on
 * leptons and jets).
 */  

#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "ExRootAnalysis/include/ExRootClasses.h"
#include <vector>
#include <string>
#include <map>
#include "DataObjects/include/LHEWeights.h"
#include "DataObjects/include/Particle.h"

namespace vvana {

enum VVType {Wg, Zg, WW, WZ, ZZ};

/**
 *
 * @class VVEvent
 *
 *
 */
class VVEvent
{
  
 private: 

  VVType _vvType;
  std::vector<Particle> _particles;
  std::vector<float> _weights;

  
public:
    /// Constructor
  VVEvent(const VVType vvType, const std::vector<Particle> particles, const std::vector<float> & _weights);
  // ~VVEvent();

    const VVType vvType() const {return _vvType;}
    
    // Event particles
    const std::vector<Particle> electrons() const;
    const std::vector<Particle> muons() const;
    const std::vector<Particle> taus() const;
    const std::vector<Particle> leptons() const;
    const std::vector<Particle> jets() const;
    const std::vector<Particle> gammas() const;
    const std::vector<Particle> Ws() const;
    const std::vector<Particle> Zs() const;
    const std::vector<Particle> bosons() const;
    const std::vector<Particle> particles() const {return _particles;}
        
    float jetInvMass() const;
    float jetDeltaEta() const;
    float zPt() const;
    float vvTransMass() const;
    float vvMass() const;

    const std::vector<float>& weights() const {return _weights;};
    float smWeight() const;

    void printEvent() const;
};
} // end namespace vvana 
 
#endif // __VVEvent_h__
