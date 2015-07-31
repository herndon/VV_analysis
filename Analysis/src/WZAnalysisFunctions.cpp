#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Analysis/include/WZAnalysis.h"
#include "Analysis/include/WZAnalysisFunctions.h"


const vvana::WZAnalysis vvana::BuildWZAnalysis(const VVEvent& vvEvent){

  bool wzEvent = true;

  // All particles in VVEvent referenced by index
  int w = -1;
  int z = -1;
  // Vectors are pT ordered
  std::vector<int> wLeptons;
  int wNeutrino = -1;
  int wLepton = -1;
  std::vector<int> zLetpons;
  std::vector<int> jets;

  int index = 0;
  for (auto const & particle: vvEvent.particles()) {
    if (particle.type() == 23) z = index;
    if (particle.type() == 24) w = index;
    index++;
    
  }
  
  
  return WZAnalysis(vvEvent);
  
}

  
