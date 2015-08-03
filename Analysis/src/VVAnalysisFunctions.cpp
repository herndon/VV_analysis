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
    if (std::abs(particle.type()) == 23) numberBosons++;
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
  

const std::vector<int> vvana::leptons(const VVEvent& vvEvent){

  std::vector<int> leptons;
  int index = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) >= 11 && std::abs(particle.type()) <= 16) leptons.push_back(index);
    ++index;
  }
 
  return leptons;

}

const std::vector<int> vvana::chargedLeptons(const VVEvent& vvEvent){

  std::vector<int> leptons;
  int index = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 11 || std::abs(particle.type()) == 13 || std::abs(particle.type()) == 15 ) leptons.push_back(index);
    ++index;
  }
 
  return leptons;

}

const std::vector<int> vvana::neutrinos(const VVEvent& vvEvent){

  std::vector<int> leptons;
  int index = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 12 || std::abs(particle.type()) == 14 || std::abs(particle.type()) == 16 ) leptons.push_back(index);
    ++index;
  }
 
  return leptons;

}

const std::vector<int> vvana::zs(const VVEvent& vvEvent){

  std::vector<int> bosons;
  int index = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 23) bosons.push_back(index);
    ++index;
  }
 
  return bosons;

}

const std::vector<int> vvana::ws(const VVEvent& vvEvent){

  std::vector<int> bosons;
  int index = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 24) bosons.push_back(index);
    ++index;
  }
 
  return bosons;

}

const std::vector<int> vvana::jets(const VVEvent& vvEvent){

  std::vector<int> jets;
  int index = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if ((std::abs(particle.type()) >= 1 && std::abs(particle.type()) <= 5)||std::abs(particle.type()==21)) jets.push_back(index);
    ++index;
  }
 
  return jets;

}
  

const std::vector<int> vvana::zlls(const VVEvent& vvEvent){

  std::vector<int> bosons;
  int index = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 23 &&
	( (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==11&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==11) ||
	  (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==13&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==13) ||
	   (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==15&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==15) )) bosons.push_back(index);
    ++index;
  }
 
  return bosons;

}

const std::vector<int> vvana::wlnus(const VVEvent& vvEvent){

  std::vector<int> bosons;
  int index = 0;
  
  for (auto const & particle: vvEvent.particles()) {
    if (std::abs(particle.type()) == 24 &&
	( (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==11&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==12) ||
	  (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==13&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==14) ||
	   (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==15&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==16) ||
	   (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==12&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==11) ||
	  (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==14&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==13) ||
	   (std::abs(vvEvent.particles()[particle.daughters()[0]].type())==16&&
	   std::abs(vvEvent.particles()[particle.daughters()[0]].type())==15))) bosons.push_back(index);
    ++index;
  }

  
  return bosons;

}
