#ifndef __PARTICLE_VECTOR_H__
#define __PARTICLE_VECTOR_H__

#include "TLorentzVector.h"

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



#endif


