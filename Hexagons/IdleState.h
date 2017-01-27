#pragma once
#include "state.h"
#include "Cell.h"
#include "ZombieBase.h"

//header file for the idle state, just included the functions

class IdleState : public State
{
public:
	IdleState(std::weak_ptr<StateManager> manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes);
	~IdleState(void);
	void update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player);
};
