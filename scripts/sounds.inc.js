deathSounds = new Array();

for(var i = 1; i <= 20; i++) {
	deathSounds.push(loadSound(i + "-BugSplat.ogg"));
}

function playRandom(soundArray) {
	var l = soundArray.length;
	var s = Math.floor(Math.random() * l);
	soundArray[s].play();
}

