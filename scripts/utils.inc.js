function frand(min, max) {
	if(min > max) {
		var tmp = min;
		min = max;
		max = tmp;
	}

	return Math.random() * (max - min) + min;
}
