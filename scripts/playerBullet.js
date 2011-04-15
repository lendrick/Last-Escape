function playerBulletUpdate(dt) {
	this.bulletTime += dt;
	//cout << "angle " << rad2deg(body->a) << "\n";

	this.updateSpriteFacing();
	//checkcollisions();
	if(this.bulletTime > this.lifetime) {
		this.destroy();
	}
}


function playerBulletCollide(otherActor)
{
	if (otherActor.shape.collision_type == PhysicsType.enemy) {
		otherActor.doDamage(damage);
		this.destroy();
	}
}

function playerBulletDestroy()
{
	this.destroy();
}

function playerBullet(x, y) {
	var a = new Actor(x, y, "playerBullet.xml");
	a.actorName = "playerBullet";
	a.speed = 500.0;
	a.angle = ((a.facing == Facing.right) ? 90 : -90) + (frand(100, 200)) * angleVariation/100;
	a.speed_x = 0.5 + Math.sin(a.angle * 3.14159/180.0) * a.speed;
	a.speed_y = 0.5 + Math.cos(a.angle * 3.14159/180.0) * a.speed;

	// Put it in the PLayerBullets group so it doesn't collide with other PlayerBullets
	a.collisionGroup = PhysicsGroup.playerBullets;
	a.shapeLayers = PhysicsLayer.map|PhysicsLayer.playerBullet;
	a.collisionType = PhysicsType.playerBullet;

	a.update = playerBulletUpdate;
	a.onCollide = playerBulletCollude;
	a.onCollideGround = playerBulletDestroy;
	a.onCollideWall = playerBulletDestroy;

	a.body.velocity.x = a.speed_x;
	a.body.velocity.y = a.speed_y;

	if(a.facing == Facing.right)
		a.body.rotationDegrees = -a.angle - 270;
	else
		a.body.rotationDegrees = -a.angle - 90;

	//TODO
	//setVelocityFunc(no_gravity);

	a.damage = 1;
	a.canSleep = false;

	a.animation = "bullet";
}
