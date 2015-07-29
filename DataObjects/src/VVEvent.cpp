#include "DataObjects/include/VVEvent.h"
#include <cmath>
#include <iostream>

vvana::VVEvent::VVEvent(const VVType vvType, const std::vector<Particle> particles, const std::vector<float> & weights):
  _vvType(vvType),
  _particles(particles),
  _weights(weights)
{
}

const std::vector<vvana::Particle> vvana::VVEvent::electrons() const {
  return _particles;
}
const std::vector<vvana::Particle> vvana::VVEvent::muons() const {
  return _particles;
}
const std::vector<vvana::Particle> vvana::VVEvent::taus() const {
  return _particles;
}
const std::vector<vvana::Particle> vvana::VVEvent::leptons() const {
  return _particles;
}
const std::vector<vvana::Particle> vvana::VVEvent::jets() const {
  return _particles;
}
const std::vector<vvana::Particle> vvana::VVEvent::gammas() const {
  return _particles;
}
const std::vector<vvana::Particle> vvana::VVEvent::Ws() const {
  return _particles;
}
const std::vector<vvana::Particle> vvana::VVEvent::Zs() const {
  return _particles;
}
const std::vector<vvana::Particle> vvana::VVEvent::bosons() const {
  return _particles;
}
        
float vvana::VVEvent::jetInvMass() const {
  return 0.0;
}
float vvana::VVEvent::jetDeltaEta() const {
  return 0.0;

}
float vvana::VVEvent::zPt() const {
  return 0.0;

}
float vvana::VVEvent::vvTransMass() const {
  return 0.0;

}
float vvana::VVEvent::vvMass() const {
  return 0.0;

}

float vvana::VVEvent::smWeight() const {
  return 0.0;

}

void vvana::VVEvent::printEvent() const {

  // Implement event type check
  std::cout << "Event type: WZ" << std::endl;
  // Implement particle print
  // Implement particles loop
  if (_particles.begin() != _particles.end()) std::cout << "First particle type: " << _particles.begin()->type() << std::endl;
}


