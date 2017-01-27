#include "SmartZombie.h"



SmartZombie::SmartZombie(int hexagonWidth) : ZombieBase(hexagonWidth)
{
	SetBitmap("Images\\Zombie.bmp");
	SetImageValues(0, 0, 32, 32, 0, 0, 96, 32, 0.001, hexagonWidth);

	attackDistance = 10;
	moveSpeed = 200;																	//smart zombie is very similar to regular zombie just with higher difficulites
	dead = false;
	pathLockOff = 10;
	damage = 10;

	spawnPoint = Cell::Position(1, 1);

	manager.reset(new StateManager());
}


SmartZombie::~SmartZombie()
{
}
