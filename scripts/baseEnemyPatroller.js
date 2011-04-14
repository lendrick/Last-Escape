include("baseEnemy.js");

function baseEnemyPatrollerUpdate(dt) {
	//print(this.leftBumper.shape.body);
	if(!this.dying) {
		if(this.grounded) {
			//print("Patroller update");
			if (this.facing == Facing.left) {
				if(this.body.velocity.x > -this.walk_speed)
					this.body.applyImpulse(-500, 0);

				if(!this.leftBumper.grounded)
					this.facing = Facing.right;
			} else if (this.facing == Facing.right) {
				if(this.body.velocity.x < this.walk_speed)
					this.body.applyImpulse(500, 0);

				if(!this.rightBumper.grounded)
					this.facing = Facing.left;
			}
		}

		this.updateSpriteFacing();
	}
}

function baseEnemyPatrollerBumperCollide(facing) {
	//print("Patroller bumper " + facing);
	if(facing == Facing.left) {
		this.facing = Facing.right;
	} else if(facing == Facing.right) {
		this.facing = Facing.left;
	}
}

function baseEnemyPatroller(x, y, anim) {
	//print("Creating flyer at " + x + " " + y);
	var a = baseEnemy(x, y, anim);

	a.animation = "walk";
	a.actorName = "baseEnemyPatroller";

	a.update = baseEnemyPatrollerUpdate;
	a.onBumperCollide = baseEnemyPatrollerBumperCollide;

	a.shape.friction = 0.1;

	a.drop_offset_x = 0;
	a.drop_offset_y = 0;

	a.leftBumper = a.addBumper(Facing.left, 4);
	a.rightBumper = a.addBumper(Facing.right, 4);

	return a;
}

