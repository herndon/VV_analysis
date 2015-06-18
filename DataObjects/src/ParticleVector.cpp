#include "DataObjects/include/ParticleVector.h"

ParticleVector::ParticleVector(int type)
{
    this->type = type;
    TLorentzVector();
}

ParticleVector::ParticleVector()
{
    ParticleVector(0);
}
const int ParticleVector::getType() const
{
    return type;
}
void ParticleVector::setType(int type)
{
    this->type = type;
}
