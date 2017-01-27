#pragma once
#include "Image.h"
#include "Cell.h"
#include "Agent.h"							//header files to include
#include <math.h>
#include "StateManager.h"

#include <memory>

class ZombieBase : public Agent					//inherit from agent, as that is the pathfinding class
{
public:
	ZombieBase(int hexagonWidth);
	~ZombieBase();
	void Draw();
	void SetDrawPosition(Cell::Position _position);
	void MoveToNextPoint(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, double _deltaTime);
	std::shared_ptr<StateManager> manager;
	Cell::Position GetPixelPosition();
	int GetDistanceFromPlayer(Cell::Position playerHexagonLocation);											//functions that are used

	int GetAttackDistance();
	int GetPathLockOff();
	int GetDamage();
	void ZombieDead();
	bool IsZombieDead();
	void RespawnZombie();

	Cell::Position GetSpawnPoint();
	void SetSpawnPoint(Cell::Position _spawnPoint);

	bool CanZombieAttack(double deltaTime);
	void ResetAttackTimer();
	void ZombieTimer(double deltaTime);
protected:	
	Cell::Position zombiePosition;
	Cell::Position spawnPoint;

	float startX, startY, endX, endY;

	bool moving = false;

	float distance;													//protected variables so they can be used by children of the base zombie, for example smart zombie
	float directionX;
	float directionY;		

	int attackDistance;
	float moveSpeed;
	bool dead;
	int pathLockOff;
	float damage;

	float attackCoolDown;
	double timeSinceLastAttack;
};

