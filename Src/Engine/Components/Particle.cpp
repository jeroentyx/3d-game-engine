#include "Particle.h"

bool Particle::operator<(const Particle& that) const
{
	return this->cameraDistance > that.cameraDistance;
}