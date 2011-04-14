include("energyBall.js");

function baseEnemyDie()
{
	this.canCollide = false;
	this.dying = true;
	this.freeze();
	this.setCurrentAnimation("die");
	playRandom(deathSounds);
	//fireSound->playSound();
}

function baseEnemyCollide(otherActor) {
	//print("Collide!");
	//this.doDamage(1);
	if(otherActor.isPlayer()) otherActor.doDamage(1);
}

function baseEnemyAnimationComplete(anim) {
	if(anim == "die") {
		this.destroy();
		energyBall(this.x, this.y);
	}
}

function baseEnemy(x, y, anim) {
	//print("Creating flyer at " + x + " " + y);
	var a = new Actor(x, y, anim);
	a.canSleep = true;
	a.life = 0.1;
	a.damageTimer = 0.1;
	a.immunityTime = 0.05;

	a.shape.collisionType = 2;
	a.actorName = "baseEnemy";

	a.onCollide = baseEnemyCollide;
	a.onAnimationComplete = baseEnemyAnimationComplete;
	a.die = baseEnemyDie;

	a.patrolLength = 100;
	a.fly_speed = 200;
	a.patrolCountdown = a.patrolLength;

	a.shapeLayers = PhysicsLayer.map|PhysicsLayer.enemy|PhysicsLayer.playerBullet;
	a.collisionType = PhysicsType.enemy;

	return a;
}

