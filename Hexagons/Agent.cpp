#include "Agent.h"
#include <memory>				//header files 

Agent::Agent()
{
	cellPosition = std::make_shared<Cell::Position>(-1, -1);
	endCell = std::make_shared<Cell::Position>(-1, -1);
	pathFind = true;
}
																		//constructor and deconstrucor

Agent::~Agent()
{
}

void Agent::PathFind(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes)
{	
	if (pathFind == true)																		//if you should pathfind
	{
		cellsToCheck.clear();																	//clear the old pathfind stuff
		path.clear();
		if (cellPosition->m_x != -1 && endCell->m_x != -1)											//check the end isnt out of bounds
		{
			pathFinder.aStar(nodes, cellsToCheck, cellPosition, endCell, path);						//pathfind
			pathFind = false;																		//stop pathfinding
		}
	}
	
}

std::shared_ptr<Cell::Position> Agent::GetHexagonPosition() 
{
	return cellPosition;
}

std::shared_ptr<Cell::Position> Agent::GetEndPosition()
{
	return endCell;
}																							//getters and setters

std::vector<std::shared_ptr<Cell::Position>> Agent::GetPath()
{
	return path;
}

void Agent::SetPosition(Cell::Position pos)
{
	cellPosition->m_x = pos.m_x;
	cellPosition->m_y = pos.m_y;
}

void Agent::SetTargetPosition(Cell::Position pos)
{
	endCell->m_x = pos.m_x;
	endCell->m_y = pos.m_y;
}

void Agent::DrawPath(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes)
{
	Cell::Position origin(NULL, NULL), parentPosition(NULL, NULL);
	if (path.size() > 0) //if you have found a path 
	{
		for (size_t i = 0; i < path.size() - 1; i++)																			//loop through the path
		{
			origin = nodes[path[i]->m_y][path[i]->m_x]->GetOrigin();															//get the origin of each cell in the path
			parentPosition = nodes[path[i + 1]->m_y][path[i + 1]->m_x]->GetOrigin();
			al_draw_line(origin.m_x, origin.m_y, parentPosition.m_x, parentPosition.m_y, al_map_rgb(42, 120, 242), 5);			//draw a line to each origin
		}
	}
}

void Agent::StartPathFind()
{
	pathFind = true;
}

void Agent::RemoveLastOneFromPath()			//remove the last one from the path
{
	if (path.size() > 0)
	{
		path.pop_back();
	}
}

void Agent::Flee(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, Cell::Position playerHexagonValue)
{
	int hexagonYAmount, hexagonXAmount;
	bool fleeLeft, fleeUp;
														//sets some temporary variables up
	fleeLeft = true;
	fleeUp = true;

	if (nodes[0][0] != NULL)								//if the nodes arent empty
	{
		hexagonYAmount = nodes.size() - 1;					//get the nodes boundary
		hexagonXAmount = nodes[0].size() - 1;
	}

	if (hexagonXAmount / 2 > playerHexagonValue.m_x)		//check if they are closer to the top or bottom / left or right boundary
	{
		fleeLeft = false;
	}
	else
	{
		fleeLeft = true;
	}

	if (hexagonYAmount / 2 > playerHexagonValue.m_y)
	{
		fleeUp = false;
	}
	else
	{
		fleeUp = true;
	}

	Cell::Position positionToFleeTo;
	do
	{		
		if (fleeLeft == true && fleeUp == true)
		{
			positionToFleeTo.m_x = 0;
			positionToFleeTo.m_y = 0;
		}
		else if (fleeLeft == true && fleeUp == false)
		{
			positionToFleeTo.m_x = 0;
			positionToFleeTo.m_y = hexagonYAmount;						//sets the location they should flee too, not very good and should be improved to better location picking
		}
		else if (fleeLeft == false && fleeUp == true)
		{
			positionToFleeTo.m_x = hexagonXAmount;
			positionToFleeTo.m_y = 0;
		}
		else if (fleeLeft == false && fleeUp == false)
		{
			positionToFleeTo.m_x = hexagonXAmount;
			positionToFleeTo.m_y = hexagonYAmount;
		}
	} while (nodes[positionToFleeTo.m_y][positionToFleeTo.m_x]->GetBlock() == Cell::Value::Wall);		//will change this later but checking that they arent in a wall

	SetTargetPosition(positionToFleeTo);		//set the location to flee to in the actual zombie object

	
}