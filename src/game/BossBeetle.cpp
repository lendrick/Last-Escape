/*
 * BossBeetle.cpp
 *
 *  Created on: 27.10.2011
 *      Author: nehmulos
 */

#include "BossBeetle.h"

BossBeetle::BossBeetle(double x, double y) : EnemyPatroller(x, y, 64, 44)
{
	setImage("bossbeetle.png");
	walk_speed = 100.0f;
	canSleep = false;

	dying = false;
	life = 5;
//	status = BeetleBossStatus::patrolling;

	setDrawOffset(32, 22);
	setFrameSize(64, 64);
	dizzynessTime = 2000;
	time = 0;
	dizzynessTimer = dizzynessTime;
	nextDirection = Facing::Left;


	Animation * tmp;

	//pick a random death sound
	int sound_num = rand() % 19;
	sound_num += 1;
	std::string s;
	std::stringstream out;
	out << sound_num;
	s = out.str();

	std::string sound_file = s + "-BugSplat.ogg";
	//cout << sound_file;
	fireSound = soundCache[sound_file];
	shape->u = 0.1f;

	tmp = addAnimation("walk");
	tmp->addFrame(4, 200);
	tmp->addFrame(0, 200);
	tmp->addFrame(5, 200);
	tmp->setDoLoop(true);

	tmp = addAnimation("rolledUp");
	tmp->addFrame(8, 200);

	tmp = addAnimation("die");
	tmp->addFrame(8, 200);
	tmp->addFrame(9, 200);
	tmp->addFrame(10, 200);
	tmp->addFrame(11, 200);

	tmp = addAnimation("hurt");
	tmp->addFrame(9, 45);
	tmp->addFrame(11, 45);

	setCurrentAnimation("walk");
}


bool BossBeetle::doDamage(double damage, bool knockback)
{
	if(getCurrentAnimation()->getName() != "rolledUp")
	{
		return EnemyPatroller::doDamage(damage, knockback);
	}
	return false;
}

void BossBeetle::onDamage()
{
	if(getCurrentAnimation()->getName() != "rolledUp")
		EnemyPatroller::onDamage();
}

void BossBeetle::doUpdate(sf::Uint32 dt)
{
	if(getCurrentAnimation()->getName() != "rolledUp")
	{
		if(dizzynessTimer > 0)
		{
			dizzynessTimer -= dt;
			if(dizzynessTimer <= 0)
			{
				dizzynessTimer = 0;
				setCurrentAnimation("rolledUp",true);
				walk_speed = 400;
				if(facing_direction != nextDirection)
					flipDirection();
			}
			if(frand(0, 100) < 3)
				flipDirection();
		}
	}
	EnemyPatroller::doUpdate(dt);
}

void BossBeetle::collideGround()
{
	if(dizzynessTimer > 0 && getCurrentAnimation()->getName() == "rolledUp")
	{
		setCurrentAnimation("walk", true);
		walk_speed = 100;
	}
	Actor::collideGround();
}

void BossBeetle::onBumperCollide(int facing)
{
	if(getCurrentAnimation()->getName() == "rolledUp")
	{
		if(facing == Facing::Left)
		{
			cpBodyApplyImpulse(body, cpv(4000, 3000), cpv(0, 0));
			nextDirection = Facing::Right;
		}
		else
		{
			cpBodyApplyImpulse(body, cpv(-4000, 3000), cpv(0, 0));
			nextDirection = Facing::Left;
		}
		dizzynessTimer = dizzynessTime;
	}
	EnemyPatroller::onBumperCollide(facing);
}

BossBeetle::~BossBeetle()
{
	// TODO Auto-generated destructor stub
}
