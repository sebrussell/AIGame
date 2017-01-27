#include "FleeState.h"
#include "AttackState.h"
#include "PersueState.h"
#include "RespawnState.h"			//header files

#include <iostream>
#include <memory>

FleeState::FleeState(std::weak_ptr<StateManager> manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes) : State(manager)
{
	this->owner = owner;
	this->nodes = nodes;
}
												//constructor and deconstructor
FleeState::~FleeState(void)
{
}

void FleeState::update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player)
{
	std::shared_ptr<PersueState> persueState(new PersueState(manager, owner, nodes));
	std::shared_ptr<RespawnState> respawnState(new RespawnState(manager, owner, nodes));

	owner.lock()->Flee(nodes, player->GetHexagonPosition());										//calls the flee function to find out where to flee to
	owner.lock()->ZombieTimer(*deltaTime.get());

	owner.lock()->PathFind(nodes);																		//path finds to that flee location
	owner.lock()->MoveToNextPoint(nodes, *deltaTime.get());												//moves to that location


	if (player->ShouldZombieFlee() == false)				//if you should no longer flee
	{
		manager.lock()->StateDone();						//remove the state from the vector of states
	}

	if (owner.lock()->IsZombieDead() == true)				//if you have been caught whilst fleeing
	{
		manager.lock()->StateDone();						//end the state
		manager.lock()->setState(respawnState);				//go to the respawn state
	}

}
