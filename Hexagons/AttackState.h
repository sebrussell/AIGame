#pragma once
#include "State.h"
#include "Cell.h"

class ZombieBase;

//header file for the attack state, just included the functions

class AttackState : public State
{
public:
	AttackState(std::weak_ptr<StateManager>& manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes);
	~AttackState(void);
	void update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player);	
};

