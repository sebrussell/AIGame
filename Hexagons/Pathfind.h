#ifndef PATHFIND_H					//defines the header 
#define PATHFIND_H

#include <allegro5\allegro.h>
#include "Cell.h"
#include <allegro5\allegro_primitives.h>
#include <vector>									//defines the files to be used in the program 
#include <memory>

class Pathfind			//defines the class and its functions 
{
public:
	Pathfind();				//the functions the class uses 
	~Pathfind();
	void aStar(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, std::vector<std::shared_ptr<Cell::Position>>& cellsToCheck, std::shared_ptr<Cell::Position>& startCell, std::shared_ptr<Cell::Position> &endCell, std::vector<std::shared_ptr<Cell::Position>>& path);
	void ExpandNodes(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, std::vector<std::shared_ptr<Cell::Position>>& cellsToCheck, Cell::Position positionToExpand, int amountOfCellsY, int amountOfCellsX, std::shared_ptr<Cell::Position> &endCell, int expandPosition);
	void FindPathBack(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, std::vector<std::shared_ptr<Cell::Position>>& path, std::shared_ptr<Cell::Position> &endCell, std::shared_ptr<Cell::Position> &startCell);
	void OrderCells(std::vector<std::shared_ptr<Cell::Position>>& cellsToCheck, std::vector<std::vector<std::shared_ptr<Cell>>>& nodes);
private:
	bool completedSearch;			//the private variables
};

#endif