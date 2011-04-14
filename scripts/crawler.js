include("baseEnemyPatroller.js");

function crawler(x, y) {
	//print("Creating walker at " + x + " " + y);
	var a = baseEnemyPatroller(x, y, "crawler.xml");
	a.walk_speed = 70.0;
	a.actorName = "crawler";
	a.life = 2;
	return a;
}

