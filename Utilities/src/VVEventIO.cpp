#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Utilities/include/VVEventIO.h"

#include <vector>
#include <algorithm>
#include <iostream>

const vvana::VVEvent vvana::VVEventIO::readVVEvent(TClonesArray* branchGenParticle,TClonesArray* branchWeights){

  // VVEvent variables
  VVType vvType = WZ;
  std::vector<Particle> particles;
  std::vector<float> weights;

   
   getParticles(particleType::W,branchGenParticle,particles);  
   getParticles(particleType::Z,branchGenParticle,particles);  
   getParticles(particleType::photon,branchGenParticle,particles);  
   getParticles(particleType::electron,branchGenParticle,particles);
   getParticles(particleType::muon,branchGenParticle,particles);
   getParticles(particleType::tau,branchGenParticle,particles);  
   getParticles(particleType::neutrino,branchGenParticle,particles);  
   getParticles(particleType::jet,branchGenParticle,particles);  

   if (particles.begin() != particles.end()) std::cout << "IO First particle type: " << particles.begin()->type() << std::endl;

  std::vector<Particle> indexedParticles;

  setParentage(particles,indexedParticles); 
  VVEvent vvEvent(vvType,particles,weights);
  
  return vvEvent;

}

void vvana::VVEventIO::getParticles(particleType type, TClonesArray* branchGenParticle, std::vector<Particle> & particles){

  std::vector<int> types;
  switch (type){
  case electron:
    types.push_back(11);
    types.push_back(-11);
    break;
  case muon:
    types.push_back(13);
    types.push_back(-13);
    break;
  case tau:
    types.push_back(15);
    types.push_back(-15);
    break;
  case neutrino:
    types.push_back(12);
    types.push_back(-12);
    types.push_back(14);
    types.push_back(-14);
    types.push_back(16);
    types.push_back(-16);
    break;
  case W:
    types.push_back(24);
    types.push_back(-24);
    break;
  case Z:
    types.push_back(23);
    break;
 case photon:
    types.push_back(21);
    break;
 case jet:
    types.push_back(1);
    types.push_back(-1);
    types.push_back(2);
    types.push_back(-2);
    types.push_back(3);
    types.push_back(-3);
    types.push_back(4);
    types.push_back(-4);
    types.push_back(5);
    types.push_back(-5);
    types.push_back(6);
    types.push_back(-6);
    types.push_back(21);
    break;
  default:
    // !!!!! replace with Exception
    break;
  }
    
  std::cout << "Number of entries on gen particle branch: " <<  branchGenParticle->GetEntriesFast() << std::endl; 
  for(int ii = 0; ii < branchGenParticle->GetEntriesFast(); ++ii) {
    TRootLHEFParticle* particle = (TRootLHEFParticle*) branchGenParticle->At(ii);

    std::cout << "Particle, status: " << particle->PID << ", " << particle->Status << std::endl;
    //int id = particle->PID;
    if (((particle->Status==1)||(particle->Status==2))&&std::any_of(types.begin(),types.end(), [particle](int i){return i==particle->PID;})){
      std::cout << "Found particle index, PID: " << ii << " " << particle->PID << std::endl;

      std::vector<int> mothers;
      mothers.push_back(particle->Mother1);
      mothers.push_back(particle->Mother2);
      std::cout << "Mothers " << particle->Mother1 << " " << particle->Mother2 << std::endl;
      std::cout << "Mothers " << mothers[0] << " " << mothers[1] << std::endl;
      
      std::vector<int> daughters;

      for(int jj = 0; jj < branchGenParticle->GetEntriesFast(); ++jj) {
	TRootLHEFParticle* particle2 = (TRootLHEFParticle*) branchGenParticle->At(jj);
	std::cout << "On particle: " << ii << " Mothers of index, PID: " << jj << " " << particle2->PID << " are " << particle2->Mother1 << " " << particle2->Mother2 << std::endl;
	if (particle2->Mother1 == ii) daughters.push_back(jj);
	if (particle2->Mother2 == ii) daughters.push_back(jj);
      }


      TLorentzVector lorentzVector(particle->Px,particle->Py,particle->Pz,particle->E);

      Particle newParticle(ii,particle->PID,lorentzVector,mothers,daughters);
      newParticle.print(std::cout);
      particles.push_back(newParticle);

    }
  }

}

void vvana::VVEventIO::setParentage(const std::vector<Particle> & particles, std::vector<Particle> & indexedParticles){

  std::vector<std::vector<int>> mothersVector;
  std::vector<std::vector<int>> daughtersVector;

  for (auto const & particle : particles){
    std::vector<int> mothers;
    std::vector<int> daughters;
    
    for (auto const & mother : particle.mothers()){
      int index = 0;
      for (auto const & motherParticle : particles){
	if (mother==motherParticle.index()) mothers.push_back(index);
	index++;
      }
    }
    for (auto const & daughter : particle.daughters()){
      int index = 0;
      for (auto const & daughterParticle : particles){
	if (daughter==daughterParticle.index()) daughters.push_back(index);
	index++;
      }
    }
      
    mothersVector.push_back(mothers);
    daughtersVector.push_back(daughters);
            
    
  }


    int index = 0;
    for (auto const & particle : particles){

      Particle newParticle(particle.index(),particle.type(),particle.lorentzVector(),mothersVector[index],daughtersVector[index]);
      indexedParticles.push_back(std::move(newParticle));
      index++;

  }      

}
