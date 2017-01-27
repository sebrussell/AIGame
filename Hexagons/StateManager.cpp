#include "StateManager.h"
#include "State.h"					//list of all the headers
#include <stdlib.h>

StateManager::StateManager(void)
{
}
												//constructor and decontructor

StateManager::~StateManager(void)
{
}

void StateManager::update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player)		//information on the player and delta time is passed through each time
{
	if (stateList.size() > 0)					//if there is currently a state to be in
	{
		currentState = stateList[0];					//make sure you are at the first state in the vector
		currentState->update(deltaTime, player);		//call the states update function 
	}
}

void StateManager::setState(std::shared_ptr<State> newState)		//passed a state into the function
{
	stateList.insert(stateList.begin(), newState);					//adds the state to the top of the vector 
}

void StateManager::StateDone()					//if the state is done with
{
	stateList.erase(stateList.begin());			//remove it from the vector
}
