include("baseEnemy.js");
include("centipedeSpit.js");

function spiderUpdate(dt)
{
	if(!this.dying) {
		this.time += dt;
		this.patrolTime += dt;


		if(this.facing == Facing.left) {
			if(this.body.velocity.x > -this.walk_speed)
				this.body.applyImpulse(-500, 0);

			if(this.patrolTime > this.patrolInterval) {
				this.facing = Facing.right;
				this.patrolTime = 0;
			}
		} else {
			if(this.body.velocity.x < this.walk_speed)
				this.body.applyImpulse(500, 0);

			if(this.patrolTime > this.patrolInterval) {
				this.facing = Facing.left;
				this.patrolTime = 0;
			}
		}

		this.updateSpriteFacing();

		if(this.lastShot + this.shootInterval < this.time) {
			this.lastShot = this.time;
			centipedeSpit(this.x, this.y + 20.0, this.facing);
		}
	}
}

function spider(x, y)
{
	var a = baseEnemy(x, y, "spider.xml");

	a.update = spiderUpdate;
	a.setCurrentAnimation("walk");

	a.walk_speed = 100.0;
	a.life = 5;

	a.lastShot = 0;
	a.shootInterval = 0.5;
	a.time = 0;

	a.patrolInterval = 1.5;
	a.patrolTime = 0;

	a.shape.friction = 0.1;

	return a;
}
