function testActorUpdate(dt) {
	if(this.body.velocity.y == 0)
		this.body.velocity.y = 500;

	this.updateSpriteFacing();
}

function testActorCollide() {
	print("Collide!");
	this.doDamage(1);
}

function testActorAnimationEnd() {
	print("Animation ended: " + this.animation);
}

function testActor(x, y) {
	/* //do nothing
	print("Creating testActor at " + x + " " + y);
	var a = new Actor(x, y, "xeon.xml");
	a.canSleep = false;
	a.life = 3;
	a.shape.collisionType = 2;
	a.animation = "walk";
	a.facing = Facing.left;
	a.actorName = "testActor";

	a.update = testActorUpdate;
	a.onCollide = testActorCollide;
	a.onAnimationEnd = testActorAnimationEnd;

	return a;
	*/
}


