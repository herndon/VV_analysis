#ifndef __PARTICLE_H__
#define __PARTICLE_H__

/**
 * @file Particle.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief Stores the particle information for jet, letpon or vector boson.
 *
 * This class reads and stores all particle information from a single particle
 * Includes particle type, TlorentzVector for basic kinematics, 
 * and mother/daughter information.
 */  

#include "TLorentzVector.h"
#include <vector>

namespace vvana {

class Particle
{
private:
    int _type;
    TLorentzVector _lorentzVector;
    std::vector<int> _mothers;
    std::vector<int> _daughters;
 public:
    Particle(int _type, const TLorentzVector& lorentzVector, const std::vector<int>& mothers, const std::vector<int>& daughters);
    int type() const {return _type;}
    const TLorentzVector & lorentzVector() const {return _lorentzVector;}
    const std::vector<int> & mothers() const {return _mothers;}
    const std::vector<int> & daughters() const {return _daughters;}
    bool isStable() const {return (_daughters.size() == 0);}
    
};

} // end namespacei vvana 

#endif // __PARTICLE_H__


