#include "TClonesArray.h"
#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Utilities/include/VVEventIO.h"

#include <vector>

const VVEvent VVEventIO::readVVEvent(TClonesArray* branchGenParticle,TClonesArray* branchWeights){

  VVType vvType = WZ;
  std::vector<Particle> particles;
  std::vector<float> weights;
  VVEvent vvEvent(vvType,particles,weights);
  
  return vvEvent;

}
  
