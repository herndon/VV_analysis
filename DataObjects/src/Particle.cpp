/**
 * @file PVector.h
 *
 * @author Matt Herndon <herndon@hep.wisc.edu>
 *
 * @brief Stores the particle information for jet, letpon or vector boson.
 *
 * This class reads and stores all particle information from a single particle
 * Includes particle type, TlorentzVector for basic kinematics, 
 * and mother/daughter information.
 */  

#include "DataObjects/include/Particle.h"

vvana::Particle::Particle(int type, const TLorentzVector& lorentzVector, const std::vector<int>& mothers, const std::vector<int>& daughters):
  _type(type),
  _lorentzVector(lorentzVector),
  _mothers(mothers),
  _daughters(daughters)
{
}
