#pragma once
#include "state.h"
#include "Cell.h"
#include "ZombieBase.h"

//header file for the persue state, just included the functions

class PersueState : public State
{
public:
	PersueState(std::weak_ptr<StateManager> manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes);
	~PersueState(void);
	void update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player);
};

