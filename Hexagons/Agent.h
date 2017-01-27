#pragma once

#include <memory>
#include <vector>
#include "Cell.h"
#include <stdio.h>						//header files to include 
#include "Pathfind.h"
#include "allegro5\allegro.h"
#include "Image.h"

class Agent : public Image			//inherits from image, in order to diplay the bitmap 
{
public:
	Agent();
	~Agent();	
	std::shared_ptr<Cell::Position> GetHexagonPosition();
	std::shared_ptr<Cell::Position> GetEndPosition();
	std::vector<std::shared_ptr<Cell::Position>> GetPath();
	void PathFind(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes);									//the functions the agent uses
	void StartPathFind();
	void SetPosition(Cell::Position pos);
	void SetTargetPosition(Cell::Position pos);
	void DrawPath(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes);
	void RemoveLastOneFromPath();
	void Flee(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, Cell::Position playerHexagonValue);
private:
	
	std::vector<std::shared_ptr<Cell::Position>> cellsToCheck;
	std::vector<std::shared_ptr<Cell::Position>> path;					//the private variables that it uses 
	std::shared_ptr<Cell::Position> cellPosition, endCell;
	Pathfind pathFinder;
	bool pathFind;
};

