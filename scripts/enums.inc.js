var Facing = {
	none:  0,
	up:    1,
	down:  2,
	left:  3,
	right: 4
};

var PhysicsLayer = {
	map: 1,
	player: 2,
	enemy: 4,
	playerBullet: 8,
	enemyBullet: 16
};

var PhysicsType = {
	player: 1,
	enemy: 2,
	ground: 3,
	sensor: 4,
	wall: 5,
	death: 6,
	item: 7,
	neutral: 8,
	playerBullet: 9,
	enemyBullet: 10,
	bumper: 11
};

var PhysicsGroup = {
	playerBullets: 1,
	enemyBullets: 2,
	bumpers: 3
};

var Collision = {
	none: 0,
	tile: 1,
	danger: 2,
	slantUp: 3,
	slantdown: 4
}
