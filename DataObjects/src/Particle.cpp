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

#include <iostream>
#include "DataObjects/include/Particle.h"

vvana::Particle::Particle(int index, int type, const TLorentzVector& lorentzVector, const std::vector<int> mothers, const std::vector<int> daughters):
  _index(index),
  _type(type),
  _lorentzVector(lorentzVector),
  _mothers(mothers),
  _daughters(daughters)
{
}

void vvana::Particle::print(std::ostream& out) const {

  out << "Index:    " << _index << std::endl;
  out << "Type:     " << _type << std::endl;
  out << "p,mass:   " << _lorentzVector.Px() << " " << _lorentzVector.Py() << " " << _lorentzVector.Pz() << " " << _lorentzVector.E() << " " << _lorentzVector.M() << std::endl;
  out << "Mothers: ";
  for (auto const mother : _mothers){
    out << " " << mother;
      }
  out << std::endl;
  out << "Daughtors:";
  for (auto const daughter : _daughters){
    out << " " << daughter;
      }
  out << std::endl;
  out << std::endl;
  
}
