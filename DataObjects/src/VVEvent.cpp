#include "DataObjects/include/VVEvent.h"
#include <cmath>

VVEvent::VVEvent(const VVType vvType, const std::vector<Particle> & particles, const std::vector<float> & weights):
  _vvType(vvType),
  _particles(particles),
  _weights(weights)
{
}

const std::vector<Particle> VVEvent::electrons() const {
  return _particles;
}
const std::vector<Particle> VVEvent::muons() const {
  return _particles;
}
const std::vector<Particle> VVEvent::taus() const {
  return _particles;
}
const std::vector<Particle> VVEvent::leptons() const {
  return _particles;
}
const std::vector<Particle> VVEvent::jets() const {
  return _particles;
}
const std::vector<Particle> VVEvent::gammas() const {
  return _particles;
}
const std::vector<Particle> VVEvent::Ws() const {
  return _particles;
}
const std::vector<Particle> VVEvent::Zs() const {
  return _particles;
}
const std::vector<Particle> VVEvent::bosons() const {
  return _particles;
}
        
float VVEvent::jetInvMass() const {
  return 0.0;
}
float VVEvent::jetDeltaEta() const {
  return 0.0;

}
float VVEvent::zPt() const {
  return 0.0;

}
float VVEvent::vvTransMass() const {
  return 0.0;

}
float VVEvent::vvMass() const {
  return 0.0;

}

float VVEvent::smWeight() const {
  return 0.0;

}



