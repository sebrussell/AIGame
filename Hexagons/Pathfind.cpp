#include "Pathfind.h"
#include <allegro5\allegro.h>
#include <math.h>						//using these files 
#include <vector>
#include <memory>

Pathfind::Pathfind() {
	completedSearch = false;
}		//default constructor 

Pathfind::~Pathfind()		//default deconstructor 
{

}

void Pathfind::aStar(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, std::vector<std::shared_ptr<Cell::Position>>& cellsToCheck, std::shared_ptr<Cell::Position>& startCell, std::shared_ptr<Cell::Position> &endCell, std::vector<std::shared_ptr<Cell::Position>>& path)
{
	completedSearch = false;
	

	while (completedSearch == false)
	{
		if (cellsToCheck.size() == 0)		//if the program hasnt checked any cells yet 
		{
			for (size_t y = 0; y < 20; y++)
			{
				for (size_t x = 0; x < 20; x++)
				{
					nodes[y][x]->ResetCell();
				}
			}
			nodes[endCell->m_y][endCell->m_x]->SetBlock(Cell::End);

			nodes[startCell->m_y][startCell->m_x]->SetBlock(Cell::Value::Start);
			nodes[startCell->m_y][startCell->m_x]->SetCostToGoal(Cell::Position(endCell->m_x * 10, endCell->m_y));
			nodes[startCell->m_y][startCell->m_x]->SetCostFromNode(startCell, nodes);												//put the start on the grid and set its values
			nodes[startCell->m_y][startCell->m_x]->SetParentPosition(startCell->m_x, startCell->m_y);
			cellsToCheck.push_back(startCell);
		}

		//int tempSize = cellsToCheck.size();	//this is used for breadth first when you want to check all the cells 		
		Cell::Position position;				//defines a temporary position variable to hold the data 
		for (int i = 0; i < 1; i++) //is set to 1 for a*, can be set to many depending on the situation 
		{
			position = nodes[cellsToCheck[i]->m_y][cellsToCheck[i]->m_x]->GetPosition();			//gets the position and stores it 
			if (position.m_x == endCell->m_x && position.m_y == endCell->m_y)						//if the position is at the end cell 
			{
				completedSearch = true;
				path.clear();
				FindPathBack(nodes, path, endCell, startCell);						//find the path back
			}
			else																					//else
			{
				nodes[cellsToCheck[i]->m_y][cellsToCheck[i]->m_x]->CellBeenTransversed(true);		//set the position yyou're expanding to transversed 
				ExpandNodes(nodes, cellsToCheck, position, 20, 20, endCell, i);	//expand that position 
			}
		}

		if (cellsToCheck.size() > 1)		//if you have checked some cells
		{
			cellsToCheck.erase(cellsToCheck.begin());		//remove the cell you just checked
			OrderCells(cellsToCheck, nodes);				//order the cells			
		}
	}
}

void Pathfind::ExpandNodes(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, std::vector<std::shared_ptr<Cell::Position>>& cellsToCheck, Cell::Position positionToExpand, int amountOfCellsY, int amountOfCellsX, std::shared_ptr<Cell::Position> &endCell, int expandPosition)
{
	Cell::Position parent;																					//sets two temporary variables 
	std::shared_ptr<Cell::Position> tempPosition = std::make_shared<Cell::Position>(NULL, NULL);
	if (completedSearch == false)																			//if you havent completed the search 
	{
		for (size_t y = 0; y < 3; y++)																		//check the 6 hexagons around the cell you are expanding 
		{
			for (size_t x = 0; x < 2; x++)
			{
				tempPosition = std::make_shared<Cell::Position>(NULL, NULL);
				if (y == 0 || y == 2)
				{
					if (y == 0)
					{
						tempPosition->m_y = positionToExpand.m_y - 1;
					}
					else
					{
						tempPosition->m_y = positionToExpand.m_y + 1;												//these different if statements are purely for the
					}																								//up and down nature of the grid 
					if ((int)positionToExpand.m_y % 2 == 0)
					{
						if (x == 0)
						{
							tempPosition->m_x = positionToExpand.m_x;
						}
						else
						{
							tempPosition->m_x = positionToExpand.m_x - 1;
						}
					}
					else
					{
						if (x == 0)
						{
							tempPosition->m_x = positionToExpand.m_x;
						}
						else
						{
							tempPosition->m_x = positionToExpand.m_x + 1;											//these different if statements are purely for the		
						}																							//up and down nature of the grid 
					}
				}
				else if (y == 1)
				{
					tempPosition->m_y = positionToExpand.m_y;
					if (x == 0)
					{
						tempPosition->m_x = positionToExpand.m_x - 1;
					}
					else
					{
						tempPosition->m_x = positionToExpand.m_x + 1;
					}
				}

				if (tempPosition->m_y > -1 && tempPosition->m_x > -1 && tempPosition->m_y < amountOfCellsY && tempPosition->m_x < amountOfCellsX) //checks its not out of bounds to stop 
				{
					if (nodes[tempPosition->m_y][tempPosition->m_x]->GetBlock() != Cell::Value::Wall)												 //checks its not a wall
					{
						parent = nodes[tempPosition->m_y][tempPosition->m_x]->GetParentPosition();														//gets the parent of the one you have expanded to
						if (nodes[tempPosition->m_y][tempPosition->m_x]->HasCellBeenTransversed() == true)												//if it hasnt been transvered
						{
							if (nodes[tempPosition->m_y][tempPosition->m_x]->GetCostFromNode() < nodes[parent.m_y][parent.m_x]->GetCostFromNode());		//and the cost is lower than the parent 
							{
								nodes[positionToExpand.m_y][positionToExpand.m_x]->SetParentPosition(tempPosition->m_x, tempPosition->m_y);				//change the cell you're expandings new parent
							}
						}
						else
						{
							nodes[tempPosition->m_y][tempPosition->m_x]->SetParentPosition(positionToExpand.m_x, positionToExpand.m_y);					//otherwise set the parent to the one youre expanding
						}

						cellsToCheck.insert(cellsToCheck.begin() + expandPosition, tempPosition);													//adds the cell to an open list

						nodes[tempPosition->m_y][tempPosition->m_x]->SetCostToGoal(Cell::Position(endCell->m_x, endCell->m_y));						 //calculates the estimated cost to goal

						if (nodes[tempPosition->m_y][tempPosition->m_x]->GetBlock() == Cell::Value::Tile)												 //if the cell is a tile
						{
							nodes[tempPosition->m_y][tempPosition->m_x]->SetBlock(Cell::CheckedTile);													 //set it to a checked tile
						}
						else
						{
							nodes[tempPosition->m_y][tempPosition->m_x]->CellBeenChecked(true);
						}
						nodes[tempPosition->m_y][tempPosition->m_x]->SetCostFromNode(tempPosition, nodes);
						nodes[tempPosition->m_y][tempPosition->m_x]->SetTotalCost();																	 //calculates the total cost
					}

					if (tempPosition == endCell)																										//if the end cell has been found
					{
						completedSearch = true;																						 //end the search
					}
				}
			}
		}
	}
}

void Pathfind::FindPathBack(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, std::vector<std::shared_ptr<Cell::Position>>& path, std::shared_ptr<Cell::Position> &endCell, std::shared_ptr<Cell::Position> &startCell)
{
	std::shared_ptr<Cell::Position> parentPosition;					//sets two temporary varaibles on the position info
	std::shared_ptr<Cell::Position> currentPosition;
	path.insert(path.end(), std::make_shared<Cell::Position>(endCell->m_x, endCell->m_y));								//puts the end cell into the path 
	parentPosition = std::make_shared<Cell::Position>(nodes[endCell->m_y][endCell->m_x]->GetParentPosition());		//gets the parent of the end cell
	currentPosition = parentPosition;																				//sets the current path to the parent 
	path.insert(path.end(), currentPosition);																		//adds the parent of the end cell
	while (parentPosition->m_x != startCell->m_x || parentPosition->m_y != startCell->m_y)						//until you reach the start
	{
		parentPosition = std::make_shared<Cell::Position>(nodes[currentPosition->m_y][currentPosition->m_x]->GetParentPosition());		//get the parent position
		currentPosition = parentPosition;																								//set the current position to the parent position
		path.insert(path.end(), currentPosition);																						//insert the current position 
	}
}

void Pathfind::OrderCells(std::vector<std::shared_ptr<Cell::Position>>& cellsToCheck, std::vector<std::vector<std::shared_ptr<Cell>>>& nodes)
{
	int tempNumber;																					//sets the temporary variables
	std::vector<std::shared_ptr<Cell::Position>> tempCellsToCheck;
	tempNumber = nodes[cellsToCheck[0]->m_y][cellsToCheck[0]->m_x]->GetTotalCost();					//gets the total cost of the first cell in the list

	for (size_t i = 0; i < cellsToCheck.size(); i++)												//for each cell in the list
	{
		if (nodes[cellsToCheck[i]->m_y][cellsToCheck[i]->m_x]->GetTotalCost() < tempNumber)			//if the total cost is lower than the lowest total cost found so far
		{
			tempNumber = nodes[cellsToCheck[i]->m_y][cellsToCheck[i]->m_x]->GetTotalCost();			//update the total cost
		}
	}

	for (size_t i = 0; i < cellsToCheck.size(); i++)												//for each cell again
	{
		if (nodes[cellsToCheck[i]->m_y][cellsToCheck[i]->m_x]->HasCellBeenTransversed() != true)	//if it has not been transvered
		{
			if (nodes[cellsToCheck[i]->m_y][cellsToCheck[i]->m_x]->GetTotalCost() == tempNumber)	//and the total cost is the lowest in the list of cells
			{
				tempCellsToCheck.insert(tempCellsToCheck.begin(), cellsToCheck[i]);					//add it to the start of the list
			}
			else
			{
				tempCellsToCheck.insert(tempCellsToCheck.end(), cellsToCheck[i]);					//else add it to the end of the list
			}
		}
	}
	cellsToCheck = tempCellsToCheck;																//update the "list" of vectors of cells to check 

}
