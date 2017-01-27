#include "AttackState.h"
#include "IdleState.h"						//header files to include
#include "FleeState.h"
#include "RespawnState.h"
#include <iostream>

AttackState::AttackState(std::weak_ptr<StateManager>& manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes) : State(manager)
{
	this->owner = owner;						//owner, nodes and manager passed in so you have  information on them
	this->nodes = nodes;
}
												//constructor and deconstructor

AttackState::~AttackState(void)
{
}

void AttackState::update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player)
{
	std::shared_ptr<IdleState> idleState(new IdleState(manager, owner, nodes));
	std::shared_ptr<FleeState> fleeState(new FleeState(manager, owner, nodes));
	


	if (owner.lock()->CanZombieAttack(*deltaTime.get()) == true)
	{
		player->TakeDamage(owner.lock()->GetDamage());				//as you are in the same hexagon as the player, give the player damage
		owner.lock()->ResetAttackTimer();
	}
	

	if (owner.lock()->GetHexagonPosition()->m_x != player->GetHexagonPosition().m_x || owner.lock()->GetHexagonPosition()->m_y != player->GetHexagonPosition().m_y)	//if you are no longer in same hexagon
	{
		manager.lock()->StateDone();				//leave state and go to previous state
	}

	if (player->ShouldZombieFlee() == true)			//if the zombie should flee
	{
		manager.lock()->StateDone();					//leave the state
		manager.lock()->setState(fleeState);			//go to the flee state
	}
}

