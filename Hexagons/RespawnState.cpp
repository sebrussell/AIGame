#include "IdleState.h"
#include "AttackState.h"
#include "PersueState.h"				//header files
#include "FleeState.h"
#include "RespawnState.h"

#include <iostream>
#include <memory>

RespawnState::RespawnState(std::weak_ptr<StateManager> manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes) : State(manager)
{
	this->owner = owner;
	this->nodes = nodes;		//constructor and deconstructors 
}

RespawnState::~RespawnState(void)
{
}

void RespawnState::update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player)		//the update function for the respawn state
{
	owner.lock()->SetTargetPosition(owner.lock()->GetSpawnPoint());
	owner.lock()->PathFind(nodes);												//everytime it is called it makes sure the end goal is the spawn point, finds a path there and moves there
	owner.lock()->MoveToNextPoint(nodes, *deltaTime.get());

	if (owner.lock()->GetHexagonPosition()->m_x == owner.lock()->GetSpawnPoint().m_x && owner.lock()->GetHexagonPosition()->m_y == owner.lock()->GetSpawnPoint().m_y)	//if you are at the spawn point
	{
		owner.lock()->RespawnZombie();			//respawn the zombie
		manager.lock()->StateDone();			//call the done state, which removes it from the vector and will go back to the previous state 
	}
}