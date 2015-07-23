#include "DataObjects/include/ParticleVector.h"

vvana::ParticleVector::ParticleVector(int type)
{
    this->type = type;
    TLorentzVector();
}

vvana::ParticleVector::ParticleVector()
{
    ParticleVector(0);
}
const int vvana::ParticleVector::getType() const
{
    return type;
}
void vvana::ParticleVector::setType(int type)
{
    this->type = type;
}
