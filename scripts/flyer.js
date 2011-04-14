include("baseEnemy.js");

function flyerUpdate(dt) {
	if(!this.dying) {
		var fx = 0;
		var fy = 0;

		if(this.diving && this.timeUntilEndDive <= 0) {
			this.diving = false;
			this.timeUntilDive = frand(this.minTimeBetweenDives, this.minTimeBetweenDives);
		} else if(!this.diving && this.timeUntilDive <= 0) {
			this.diving = true;
			this.timeUntilEndDive = frand(this.minDiveTime, this.maxDiveTime);
		}

		if(this.facing == Facing.left) {
			if(this.x < this.x1) {
				this.facing = Facing.right;
			} else if(this.body.velocity.x > -this.fly_speed) {
				fx = -75;
			}
		} else if(this.facing == Facing.right) {
			if(this.x > this.x2) {
				this.facing = Facing.left;
			} else if(this.body.velocity.x < this.fly_speed) {
				fx = 75;
			}
		}

		// Doesn't matter if they go below zero.
		this.timeUntilEndDive -= dt;
		this.timeUntilDive -= dt;

		if(!this.diving && this.y < this.y1 && this.body.velocity.y < this.fly_speed) {
			fy = 500;
		}

		this.body.applyImpulse(fx, fy);

		this.updateSpriteFacing();
	}
}

function flyer(x, y) {
	//print("Creating flyer at " + x + " " + y);
	var a = baseEnemy(x, y, "flyer.xml");

	a.animation = "fly";
	a.actorName = "flyer";

	a.update = flyerUpdate;

	a.patrolLength = 100;
	a.fly_speed = 200;
	a.patrolCountdown = a.patrolLength;

	a.x1 = x - 100;
	a.x2 = x + 100;
	a.y1 = y - 20;
	a.y2 = y + 40;

	a.shape.friction = 0.1;

	a.timeUntilDive = 0;
	a.timeUntilEndDive = 0;

	a.minDiveTime = 0.1;
	a.maxDiveTime = 0.3;
	a.minTimeBetweenDives = 0.8;
	a.maxTimeBetweenDives = 2.5;

	a.diving = false;

	return a;
}

