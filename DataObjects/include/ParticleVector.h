#ifndef __PARTICLE_VECTOR_H__
#define __PARTICLE_VECTOR_H__

#include "TLorentzVector.h"

namespace vvana {

class ParticleVector : public TLorentzVector
{
private:
    int type;
public:
    ParticleVector(int type);
    ParticleVector();
    void setType(int type);
    const int getType() const;
};

} // end namespace vvana

#endif // __PARTICLE_VECTOR_H__


