include("baseEnemyPatroller.js");

function walker(x, y) {
	//print("Creating walker at " + x + " " + y);
	var a = baseEnemyPatroller(x, y, "walker.xml");
	a.walk_speed = 120.0;
	a.actorName = "walker";
	return a;
}

