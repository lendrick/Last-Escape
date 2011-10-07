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

#include "Particles.h"

#include "Map.h"

ParticleEmitter::ParticleEmitter(double x, double y)
:Actor(x, y, 32.0f, 32.0f)
{

	emissionRate = 5.f;
	maxAge = 3.f;

	if (!image.LoadFromFile("images/smoke.png"))
		printf("failed to load images/smoke.png\n");

	sprite.SetTexture(image);
	sprite.SetOrigin(16, 16);

	maxParticles = int(maxAge*emissionRate);
	timeLeft = 0;
	nextIdx = 0;
	setVelocityFunc(no_gravity);
	setCanCollide(false);
}

void ParticleEmitter::update(double dt) {

	timeLeft += dt;

	while (timeLeft > 0.f)
	{
		timeLeft -= 1.f/emissionRate;
		addParticle();
	}

	for (size_t i = 0; i < particles.size(); ++i)
	{
        particles[i].angle += particles[i].angleSpeed * dt;
        particles[i].x += particles[i].xSpeed * dt;
        particles[i].y += particles[i].ySpeed * dt;
        particles[i].age += dt;
	}
}

void ParticleEmitter::draw() {

	for (size_t i = 0; i < particles.size(); ++i)
	{
		sf::Vector2f pos = game_map->cp2sfml(cpv(particles[i].x, particles[i].y));
		sprite.SetPosition(pos.x - game_map->cam_x, pos.y - game_map->cam_y);
		sprite.SetRotation(particles[i].angle);
		double a = std::min(1.0f-particles[i].age/particles[i].maxAge, particles[i].age/particles[i].maxAge*5.f);
		sprite.SetColor(sf::Color(255, 255, 255, std::min(255, std::max(0, (int)(a*255)))));
		App->Draw(sprite);
	}
}


void ParticleEmitter::addParticle()
{
	Particle p;
	p.x = body->p.x;
	p.y = body->p.y;
	p.xSpeed = double((rand() % 20) - 10);
	p.ySpeed = -double((rand() % 10) - 50);
	p.angle = double(rand() % 360);
	p.angleSpeed = double((rand() % 200) - 100);
	p.age = 0;
	p.maxAge = maxAge * (100 - (rand() % 10)) / 100.f;

	if ((int)particles.size() < maxParticles)
		particles.push_back(p);
	else
	{
		particles[nextIdx] = p;
		nextIdx = (nextIdx + 1) % maxParticles;
	}
}

