#include "ZombieBase.h"
#include "IdleState.h"			//header files it inherits


ZombieBase::ZombieBase(int hexagonWidth)		//passed in the hexagon width to the constructor
{
	SetBitmap("Images\\Zombie.bmp");	
	SetImageValues(0, 0, 32, 32, 0, 0, 96, 32, 0.001, hexagonWidth);

	attackDistance = 7;
	moveSpeed = 100;
	dead = false;																//sets the values of different variables
	pathLockOff = 5;
	damage = 7;
	attackCoolDown = 3;

	spawnPoint = Cell::Position(1, 1);

	manager.reset(new StateManager());
}


ZombieBase::~ZombieBase()		//deconstructor
{
}

void ZombieBase::Draw()			
{	
	DrawAnimation(zombiePosition);		//calls the draw function inside the Image class
}

void ZombieBase::SetDrawPosition(Cell::Position _position)
{
	zombiePosition = _position;										//sets the zombie position
}

void ZombieBase::MoveToNextPoint(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, double _deltaTime)	
{
	std::vector<std::shared_ptr<Cell::Position>> tempPath = this->GetPath();	//gets the path thats the pathfiding function has generated

	if (moving == false && tempPath.size() > 1)		//if you havent started to move yet and you arent on the last path
	{
		startX = zombiePosition.m_x;				//gets the start position that you are at
		startY = zombiePosition.m_y;

		/*tempPos.m_x = tempPath[tempPath.size() - 1]->m_x;
		tempPos.m_y = tempPath[tempPath.size() - 1]->m_y;
		SetPosition(tempPos);*/

		Cell::Position tempOrigin = nodes[tempPath[tempPath.size() - 2]->m_y][tempPath[tempPath.size() - 2]->m_x]->GetOrigin();		//gets the end position 
		endX = tempOrigin.m_x;		//sets the end position in easier variables 
		endY = tempOrigin.m_y;

		distance = sqrt(pow(endX - startX, 2) + pow(endY - startY, 2));			//the distance is calculated with pythagoras
		if (endX - startX != 0)													//the if statements are used to prevent dividing by 0 (infinity)
		{
			directionX = (endX - startX) / distance;			//gets how far you should move each turn in x direction
		}
		if (endY - startY != 0)
		{
			directionY = (endY - startY) / distance;			//gets how far you should move each turn in y direction
		}		

		moving = true;											//now you can start moving

	}
	else
	{
		zombiePosition.m_x += directionX * moveSpeed * _deltaTime;		//moves the zombie in each direction * speed, delta time to make it the same on each pc
		zombiePosition.m_y += directionY * moveSpeed * _deltaTime;

		if (sqrt(pow(zombiePosition.m_x - startX, 2) + pow(zombiePosition.m_y - startY, 2)) >= distance)		//if you have gone past the target
		{		
			if (tempPath.size() > 0)		//and you arent at the end of your path
			{
				Cell::Position tempPos;
				tempPos.m_x = tempPath[tempPath.size() - 1]->m_x;		//set them back to the origin of the hexagon in case they have missed it
				tempPos.m_y = tempPath[tempPath.size() - 1]->m_y;
				SetPosition(tempPos);						
			}			

			zombiePosition.m_x = endX;									//updates the position again
			zombiePosition.m_y = endY;									
			this->RemoveLastOneFromPath();								//remove it from the path list so they dont keep going back to it
			moving = false;												//stop moving to update the values to the next location 
		}
	}


}

Cell::Position ZombieBase::GetPixelPosition()
{
	return zombiePosition;									//returns the zombie pixel position 
}

int ZombieBase::GetDistanceFromPlayer(Cell::Position playerHexagonLocation)			//passing in a position value
{
	return sqrt(pow(this->GetHexagonPosition()->m_x - playerHexagonLocation.m_x, 2) + pow(this->GetHexagonPosition()->m_y - playerHexagonLocation.m_y, 2));		//using pythagoras will return the distance
}

int ZombieBase::GetAttackDistance()
{
	return attackDistance;
}

int ZombieBase::GetPathLockOff()
{
	return pathLockOff;																	//just getters and setters
}

int ZombieBase::GetDamage()
{
	return damage;
}

Cell::Position ZombieBase::GetSpawnPoint()
{
	return spawnPoint;
}

void ZombieBase::SetSpawnPoint(Cell::Position _spawnPoint)
{
	spawnPoint = _spawnPoint;
}

void ZombieBase::ZombieDead()															//just getters and setters
{
	dead = true;
}

bool ZombieBase::IsZombieDead()
{
	return dead;
}

void ZombieBase::RespawnZombie()
{
	dead = false;
}

bool ZombieBase::CanZombieAttack(double deltaTime)
{
	
	if (timeSinceLastAttack > attackCoolDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ZombieBase::ResetAttackTimer()
{
	timeSinceLastAttack = 0;
}

void ZombieBase::ZombieTimer(double deltaTime)
{
	timeSinceLastAttack += deltaTime;
}