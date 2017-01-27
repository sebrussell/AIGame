#pragma once
#include "Cell.h"
#include "Player.h"

class State;		//instead of inherting, you put class State; at the top to allow you to access the features without it being a parent



class StateManager
{
public:
	StateManager(void);
	~StateManager(void);
	void update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player);				//functions used in the class
	void setState(std::shared_ptr<State> newState);
	void StateDone();
private:
	std::shared_ptr<State> currentState;						//current state that the zombie is in
	std::vector<std::shared_ptr<State>> stateList;				//the state list with all the previous states that the zombie was last in
};

