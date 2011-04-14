function baseCollectible(x, y, anim) {
	var a = new StaticActor(x, y, anim);
	a.sensor = true;
	a.collisionType = PhysicsType.item;
	a.shapeLayers = PhysicsLayer.player;
	a.animation = "image";
	a.actorName = "Collectible";
	return a;
}
