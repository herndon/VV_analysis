#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Analysis/include/VVAnalysisFunctions.h"


int vvana::numberLeptons(const VVEvent& vvEvent){

  int numberLeptons = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) >= 11 && std::abs(particle.type()) <= 16) numberLeptons++;
  }
 
  return numberLeptons;

}

int vvana::numberChargedLeptons(const VVEvent& vvEvent){

  int numberLeptons = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 11 || std::abs(particle.type()) == 13 || std::abs(particle.type()) == 15 ) numberLeptons++;
  }
 
  return numberLeptons;

}

int vvana::numberNeutrinos(const VVEvent& vvEvent){

  int numberLeptons = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 12 || std::abs(particle.type()) == 14 || std::abs(particle.type()) == 16 ) numberLeptons++;
  }
 
  return numberLeptons;

}

int vvana::numberZs(const VVEvent& vvEvent){

  int numberBosons = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 24) numberBosons++;
  }
 
  return numberBosons;

}

int vvana::numberWs(const VVEvent& vvEvent){

  int numberBosons = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 24) numberBosons++;
  }
 
  return numberBosons;

}

int vvana::numberJets(const VVEvent& vvEvent){

  int numberJets = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if ((std::abs(particle.type()) >= 1 && std::abs(particle.type()) <= 5)||std::abs(particle.type()==21)) numberJets++;
  }
 
  return numberJets;

}
  
