#include "PersueState.h"
#include "AttackState.h"
#include "FleeState.h"						//header files

#include <iostream>
#include <memory>

PersueState::PersueState(std::weak_ptr<StateManager> manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes) : State(manager)
{
	this->owner = owner;
	this->nodes = nodes;
}													//constructor and deconstructor

PersueState::~PersueState(void)
{
}

void PersueState::update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player)		//if the zombie is in the persue state and the update function is called
{
	std::shared_ptr<AttackState> attackState(new AttackState(manager, owner, nodes));
	std::shared_ptr<FleeState> fleeState(new FleeState(manager, owner, nodes));

	owner.lock()->ZombieTimer(*deltaTime.get());

	owner.lock()->SetTargetPosition(player->GetHexagonPosition());
	owner.lock()->PathFind(nodes);
	owner.lock()->MoveToNextPoint(nodes, *deltaTime.get());										//pathfind towards the player 

	if (owner.lock()->GetPath().size() > owner.lock()->GetPathLockOff())				//if the player is too many pathfinding steps away 
	{
		manager.lock()->StateDone();													//stop pursuing
	}

	if (owner.lock()->GetHexagonPosition()->m_x == player->GetHexagonPosition().m_x && owner.lock()->GetHexagonPosition()->m_y == player->GetHexagonPosition().m_y)
	{
		manager.lock()->setState(attackState);		//if you are in the same hexagon as the player then go to the attack state
	}

	if (player->ShouldZombieFlee() == true)
	{
		manager.lock()->setState(fleeState);			//if the zombie should run away, go to the flee state
	}
}