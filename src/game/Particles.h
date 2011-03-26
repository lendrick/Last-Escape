#ifndef PARTICLES_H
#define PARTICLES_H

#include "Actor.h"

struct Particle
{
    float x, y, xSpeed, ySpeed, angle, angleSpeed;
    float age, maxAge;
};

class ParticleEmitter : public Actor
{
public:
	ParticleEmitter();
	virtual void draw();
	virtual void update(float dt);
private:
    float maxAge;

	float timeLeft;
    float emissionRate;
    int maxParticles;

    size_t nextIdx;
	std::vector<Particle> particles;

	void addParticle();
};

#endif
