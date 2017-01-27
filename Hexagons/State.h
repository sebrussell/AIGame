#pragma once
class StateManager;
#include "StateManager.h"
#include "ZombieBase.h"

#include <memory>

class State
{
public:
	State(std::weak_ptr<StateManager>& manager);		//each state requires a manager to be passed to it
	~State(void);
	virtual void update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player) = 0;			//a pure virtual function meaning it has to be included in derived classes
protected:
	std::weak_ptr<StateManager> manager;
	std::weak_ptr<ZombieBase> owner;								//information which needs to be accessed by each derived state, hence why its protected
	std::vector<std::vector<std::shared_ptr<Cell>>> nodes;
};

