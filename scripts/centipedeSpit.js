include("baseEnemy.js");

function centipedeSpitUpdate(dt)
{
	if(this.grounded) {
		this.setCurrentAnimation("splat");
	}
}

function centipedeSpitCollide(otherActor)
{
	if(otherActor.isPlayer()) otherActor.doDamage(1);
	this.destroy();
}

function centipedeSpitAnimationComplete(anim)
{
	if(anim == "splat") {
		this.destroy();
	}
}


function centipedeSpit(x, y, direction)
{
	//print("centipede spit: " + x + " " + y);
	var a = baseEnemy(x, y, "centipedeSpit.xml");
	a.actorName = "centipedeSpit";
	a.setCurrentAnimation("fly");
	a.update = centipedeSpitUpdate;
	a.onCollide = centipedeSpitCollide;
	a.onAnimationComplete = centipedeSpitAnimationComplete;

	a.shapeLayers = PhysicsLayer.enemyBullet;
	a.collisionGroup = PhysicsGroup.enemyBullets;

	a.fly_speed = 400.0;
	a.body.velocity.y = 200.0;

	a.facing = direction;
	if(a.facing == Facing.left) {
		a.body.velocity.x = -a.fly_speed;
	} else {
		a.body.velocity.x = a.fly_speed;
	}

	return a;
}
