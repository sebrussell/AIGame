#include "IdleState.h"
#include "AttackState.h"
#include "PersueState.h"				//header files to include
#include "FleeState.h"

#include <iostream>
#include <memory>

IdleState::IdleState(std::weak_ptr<StateManager> manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes) : State(manager)
{
	this->owner = owner;
	this->nodes = nodes;
}
											//constructor and decontrcutor
IdleState::~IdleState(void)
{
}

void IdleState::update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player)		//everytime the update function for idle is called
{
	std::shared_ptr<AttackState> attackState(new AttackState(manager, owner, nodes));
	std::shared_ptr<PersueState> persueState(new PersueState(manager, owner, nodes));
	std::shared_ptr<FleeState> fleeState(new FleeState(manager, owner, nodes));

	owner.lock()->ZombieTimer(*deltaTime.get());

	if (owner.lock()->GetDistanceFromPlayer(player->GetHexagonPosition()) <= owner.lock()->GetAttackDistance())			//if they are close to the player
	{
		manager.lock()->setState(persueState);			//set the state to pursue
	}

	if (player->ShouldZombieFlee() == true)				//if they should flee
	{
		manager.lock()->setState(fleeState);			//set the state to flee
	}
}				//idle state is another default state along with search, depending on the zombie type, so shouldn't every be removed from the vector list