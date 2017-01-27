#include "IdleState.h"
#include "AttackState.h"
#include "PersueState.h"
#include "FleeState.h"						//header files that are used
#include "SearchState.h"

#include <iostream>
#include <memory>

SearchState::SearchState(std::weak_ptr<StateManager> manager, std::weak_ptr<ZombieBase> owner, std::vector<std::vector<std::shared_ptr<Cell>>> nodes) : State(manager)
{
	this->owner = owner;
	this->nodes = nodes;
}										//constructor and deconstrucotr

SearchState::~SearchState(void)
{
}

void SearchState::update(std::shared_ptr<double> deltaTime, std::shared_ptr<Player> player)		//every update call
{
	std::shared_ptr<AttackState> attackState(new AttackState(manager, owner, nodes));
	std::shared_ptr<PersueState> persueState(new PersueState(manager, owner, nodes));
	std::shared_ptr<FleeState> fleeState(new FleeState(manager, owner, nodes));

	owner.lock()->ZombieTimer(*deltaTime.get());

	Cell::Position positionToMoveTo;
	int randomNumber = 0;
	do
	{
		do
		{
			do
			{
				randomNumber = rand() % 4;
				switch (randomNumber)
				{
				case 0:
					positionToMoveTo.m_x = owner.lock()->GetHexagonPosition()->m_x + 1;
					positionToMoveTo.m_y = owner.lock()->GetHexagonPosition()->m_y;
					break;
				case 1:
					positionToMoveTo.m_x = owner.lock()->GetHexagonPosition()->m_x - 1;
					positionToMoveTo.m_y = owner.lock()->GetHexagonPosition()->m_y;						//pick a random hexagon to go to, up, left, down or right
					break;
				case 2:
					positionToMoveTo.m_x = owner.lock()->GetHexagonPosition()->m_x;
					positionToMoveTo.m_y = owner.lock()->GetHexagonPosition()->m_y + 1;
					break;
				case 3:
					positionToMoveTo.m_x = owner.lock()->GetHexagonPosition()->m_x;
					positionToMoveTo.m_y = owner.lock()->GetHexagonPosition()->m_y - 1;
					break;
				default:
					break;
				}
			} while (positionToMoveTo.m_x >= 20 || positionToMoveTo.m_y >= 20);			
		} while (positionToMoveTo.m_x < 0 || positionToMoveTo.m_y < 0);											//if that hexagon is not a wall or out of bounds
	} while (nodes[positionToMoveTo.m_y][positionToMoveTo.m_x]->GetBlock() == Cell::Value::Wall);

	//owner.lock()->StartPathFind();
	owner.lock()->SetTargetPosition(positionToMoveTo);															//set that to the target position
	owner.lock()->PathFind(nodes);
	owner.lock()->MoveToNextPoint(nodes, *deltaTime.get());														//pathfind and move towards that position

	if (owner.lock()->GetDistanceFromPlayer(player->GetHexagonPosition()) <= owner.lock()->GetAttackDistance())		//if you are close to the player
	{
		manager.lock()->setState(persueState);																		//go to the persue state
	}

	if (player->ShouldZombieFlee() == true)				//if the zombie should flee
	{
		manager.lock()->setState(fleeState);			//go to the flee state
	}
}																								//this is a default state so you cannot every remove it from the queue of states