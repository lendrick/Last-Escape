#pragma once

class SpawnPoint : public StartPoint {
	virtual bool isSpawnPoint() { return true; }
};
