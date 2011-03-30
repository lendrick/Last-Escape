/*
 *  This file is part of Last Escape.
 *
 *  Last Escape is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Last Escape is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Last Escape.  If not, see <http://www.gnu.org/licenses/>.
 */

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
	ParticleEmitter(float x, float y);
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
