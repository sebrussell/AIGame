#include "MouseClick.h"

//getting the position of a hexagon is useful so i put it into its own class so I could call it in lots of classes

Cell::Position MouseClick::GetMouseClickPosition(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int _hexagonWidth, int _mouseX, int _mouseY, int xCellAmount, int yCellAmount)
{
	int _x, _y, startX, startY, smallRadius, largeRadius, originX, originY, distanceFromRadius, tempDistanceFromLargeRadius, tempDistanceFromSmallRadius;
	tempDistanceFromLargeRadius = 0;
	tempDistanceFromSmallRadius = 0;									//defines and sets values for temporary values 
	distanceFromRadius = 0;
	Cell::Position origin;

	bool success = false;

	startX = _mouseX / (((_hexagonWidth / 2) * pow(3, 0.5)));
	startY = _mouseY / (_hexagonWidth * 0.75);								//gets the estimated hexagon for where the mouse might be 


	if (startY - 1 < 0)
	{
		startY + 1;
	}
	else if (startY + 2 >= yCellAmount)
	{
		startY = startY - ((startY + 2) - (yCellAmount - 1));
	}
	else if (startY + 1 <= yCellAmount)
	{
		startY = startY - 1;
	}

	if (startX - 1 < 0)																				//these if statements are to prevent the function going out of range 
	{																								//basically just adds or minuses one if its too close to the edge
		startX + 1;
	}
	else if (startX + 2 >= xCellAmount)
	{
		startX = startX - ((startX + 2) - (xCellAmount - 1));
	}
	else if (startX + 1 <= xCellAmount)
	{
		startX = startX - 1;
	}

	if (startX < 0)
	{
		startX = 0;
	}
	if (startY < 0)
	{
		startY = 0;
	}


	for (int y = 0; y < 3; y++)
	{																														//because it is only an estimate position I decided to loop
		for (int x = 0; x < 3; x++)																							//through a grid of 9
		{
			smallRadius = nodes[startY + y][startX + x]->GetSmallHexagonHeight();
			largeRadius = nodes[startY + y][startX + x]->GetHexagonWidth() / 2;												//gets the small and large radius of the cell
			origin = nodes[startY + y][startX + x]->GetOrigin();
			originX = origin.m_x;
			originY = origin.m_y;
			if ((pow(originX - _mouseX, 2) + (pow(originY - _mouseY, 2))) < pow(smallRadius, 2))							//using circle theorum checks if its inside a small radiu 
			{
				_x = startX + x;
				_y = startY + y;																							//if it is it has found the hexagon clicked on
				success = true;
			}
			else if ((pow(originX - _mouseX, 2) + (pow(originY - _mouseY, 2))) < pow(largeRadius, 2))						//if it is not then it looks at the larger circle 
			{
				tempDistanceFromSmallRadius = pow((originX - _mouseX, 2) + (pow(originY - _mouseY, 2)), 0.5);				//gets the distance from the origin
				if (distanceFromRadius == 0)
				{
					distanceFromRadius = tempDistanceFromSmallRadius;
				}
				if (tempDistanceFromSmallRadius <= distanceFromRadius)														//if it is the smallest distance from origin so far
				{
					distanceFromRadius = tempDistanceFromSmallRadius;														//updates it to being the hexagon clicked on
					_x = startX + x;
					_y = startY + y;
					success = true;
				}
			}
		}
	}
	if (success == true)																									//if it has found the cell clicked on
	{
		Cell::Position tempPosition;
		tempPosition.m_x = _x;
		tempPosition.m_y = _y;																								//sets those values
		return tempPosition;
	}
	else
	{
		Cell::Position tempPosition;
		tempPosition.m_x = -1;																								//otrherwise sets it to nothing 
		tempPosition.m_y = -1;
		return tempPosition;
	}

}
