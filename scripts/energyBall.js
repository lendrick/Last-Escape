include("baseCollectible.js");

function energyBallCollide(otherActor)
{
	if (otherActor.isPlayer()) {
		if(!otherActor.dying) {
			this.destroy();
			this.sound.play();

			if(otherActor.energyBalls < 10)
				otherActor.energyBalls++;

			otherActor.addExperience(experienceValue);
			// give player 25 energy, but only up to max energy
			if(otherActor.energy < otherActor.energy_max) {
				otherActor.energy = Math.min(otherActor.energy_max, otherActor.energy + 25.0);
			}
		}
	}
}


function energyBall(x, y) {
	var a = baseCollectible(x, y, "energyBall.xml");
	a.animation = "anim";
	a.actorName = "energyBall";
	a.experienceValue = 1;
	a.onCollide = energyBallCollide;
	a.sound = loadSound("gmae.ogg");
	return a;
}
