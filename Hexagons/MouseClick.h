#pragma once

#include "Cell.h"			//header is uses

class MouseClick
{								//the one function that is used
public:
	Cell::Position GetMouseClickPosition(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int _hexagonWidth, int _mouseX, int _mouseY, int xCellAmount, int yCellAmount);
};

