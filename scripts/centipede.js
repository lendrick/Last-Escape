include("baseEnemyPatroller.js");
include("centipedeSpit.js");
include("energyBall.js");

function centipedeUpdate(dt)
{
	//print("Centipede update: " + this.lastShot + " " + this.shootInterval + " " + this.time);
	if(!this.dying) {
		this.time += dt;
		if(this.lastShot + this.shootInterval <= this.time && this.animation != "shoot") {
			this.setCurrentAnimation("shoot");
			this.shape.friction = 1.0;
		} else {
			this.shape.friction = 0.1;
		}

		if(this.animation != "shoot")
			this.baseUpdate(dt);
	}
}

function centipedeAnimationComplete(anim)
{
	//print("Centipede animation complete: " + anim)
	if(anim == "die") {
		this.destroy();
		energyBall(this.x, this.y);
	} else if(anim == "shoot") {
		//shoot a projectile
		this.lastShot = this.time;
		this.setCurrentAnimation("walk");
		var px = this.x + 8;
		if(this.facing == Facing.left) px -= 16;
		centipedeSpit(px, this.y + 10.0, this.facing);
	}
}

function centipede(x, y) {
	a = baseEnemyPatroller(x, y, "centipede.xml");
	a.baseUpdate = baseEnemyPatrollerUpdate;
	a.update = centipedeUpdate;
	a.onAnimationComplete = centipedeAnimationComplete;
	a.walk_speed = 90.;
	a.shootInterval = 2.5;
	a.lastShot = 0;
	a.time = 0;

	return a;
}
