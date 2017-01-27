#include "State.h"					//header files that are inherited 
#include "StateManager.h"


State::State(std::weak_ptr<StateManager>& manager)		//constructor which is passed in a weak pointer to the manager
{
	this->manager = manager;					//sets the managaer
}	

State::~State(void)				//deconstructor
{
}
