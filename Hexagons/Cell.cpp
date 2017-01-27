#include "Cell.h"
#include <allegro5\allegro.h>
#include <math.h>							//defines all the files that will be used
#include <vector>
#include <memory>

Cell::Cell() {} //default constructor
Cell::Cell(int _width, int _x, int _y, int _parentX, int _parentY, ALLEGRO_COLOR _colour, Cell::Value _value, int _hexagonSpacing) //contructor with parameters 
{
	m_beenChecked = false;
	m_beenTransversed = false;
	m_displayPointerToParent = false;
	m_position.m_x = _x;
	m_position.m_y = _y;
	m_parentNode.m_x = _parentX;
	m_parentNode.m_y = _parentY;					//sets the default parameters 
	m_colour = _colour;
	m_hexagonWidth = _width;
	m_cellValue = _value;

	if (_y % 2 == 0)	//this is to check if the hexagon is a left row or a right row, as they slot together 
	{
		m_vertices[0].m_x = (_x * ((((m_hexagonWidth / 2) * pow(3, 0.5))) + _hexagonSpacing));
		m_vertices[0].m_y = (_y * ((m_hexagonWidth * 0.75 + _hexagonSpacing))) + (0.25 * m_hexagonWidth);
		m_vertices[1].m_x = (_x * ((((m_hexagonWidth / 2) * pow(3, 0.5))) + _hexagonSpacing)) + (0.5 * ((m_hexagonWidth / 2) * pow(3, 0.5)));
		m_vertices[1].m_y = (_y * ((m_hexagonWidth * 0.75 + _hexagonSpacing)));
		m_vertices[2].m_x = m_vertices[0].m_x + ((m_hexagonWidth / 2) * pow(3, 0.5));
		m_vertices[2].m_y = m_vertices[0].m_y;														//sets the values for the vertices 
		m_vertices[3].m_x = m_vertices[2].m_x;
		m_vertices[3].m_y = m_vertices[2].m_y + (0.5 * m_hexagonWidth);
		m_vertices[4].m_x = m_vertices[1].m_x;
		m_vertices[4].m_y = m_vertices[1].m_y + m_hexagonWidth;
		m_vertices[5].m_x = m_vertices[0].m_x;
		m_vertices[5].m_y = m_vertices[3].m_y;
	}
	else
	{
		m_vertices[0].m_x = (_x * ((((m_hexagonWidth / 2) * pow(3, 0.5))) + _hexagonSpacing)) + (0.5 * ((m_hexagonWidth / 2) * pow(3, 0.5)));
		m_vertices[0].m_y = (_y * ((m_hexagonWidth * 0.75 + _hexagonSpacing))) + (0.25 * m_hexagonWidth);
		m_vertices[1].m_x = (_x * ((((m_hexagonWidth / 2) * pow(3, 0.5))) + _hexagonSpacing)) + (2 * (0.5 * ((m_hexagonWidth / 2) * pow(3, 0.5))));
		m_vertices[1].m_y = (_y * ((m_hexagonWidth * 0.75 + _hexagonSpacing)));
		m_vertices[2].m_x = m_vertices[0].m_x + ((m_hexagonWidth / 2) * pow(3, 0.5));
		m_vertices[2].m_y = m_vertices[0].m_y;													//sets the values for the vertices 
		m_vertices[3].m_x = m_vertices[2].m_x;
		m_vertices[3].m_y = m_vertices[2].m_y + (0.5 * m_hexagonWidth);
		m_vertices[4].m_x = m_vertices[1].m_x;
		m_vertices[4].m_y = m_vertices[1].m_y + m_hexagonWidth;
		m_vertices[5].m_x = m_vertices[0].m_x;
		m_vertices[5].m_y = m_vertices[3].m_y;
	}

	m_costToGoal = NULL;
	m_costToNode = NULL;				//sets these values to NULL to prevent memory errors later 
	m_totalCost = NULL;

	m_smallHexagonHeight = pow((pow(m_hexagonWidth / 2, 2) - pow(m_hexagonWidth / 4, 2)), 0.5);
	m_origin.m_x = m_vertices[0].m_x + (0.5 * ((m_hexagonWidth / 2) * pow(3, 0.5)));					//works out the different hexagon heights, and the location of the origin
	m_origin.m_y = m_vertices[0].m_y + (m_hexagonWidth / 4);
}


Cell::~Cell() {}			//default constructor 

int Cell::GetHexagonWidth()		//returns the hexagon width
{
	return m_hexagonWidth;
}
void Cell::SetHexagonWidth(int _width)		//sets the hexagon width
{
	m_hexagonWidth = _width;
}
Cell::Position Cell::GetPosition()		//returns the position, which is a struct containing x and y values
{
	return m_position;
}
void Cell::SetPosition(int _x, int _y)  //sets the values in the position struct 
{
	m_position.m_x = _x;
	m_position.m_y = _y;
}
ALLEGRO_COLOR Cell::GetColour() //returns the colour of cell 
{
	return m_colour;
}
void Cell::SetColour(int _r, int _g, int _b)		//allows the option to individually set the cell colour using rgb values 
{
	m_colour = al_map_rgb(_r, _g, _b); //al_map_rgb is the allegro function to create a colour number 
}
Cell::Position Cell::GetParentPosition() //returns the parent position 
{
	return m_parentNode;
}
void Cell::SetParentPosition(int _x, int _y)		//sets the parent position 
{
	m_parentNode.m_x = _x;
	m_parentNode.m_y = _y;
}
bool Cell::HasCellBeenChecked()	//returns the boolean value if the cell has been checked 
{
	return m_beenChecked;
}
void Cell::CellBeenChecked(bool _checked)		//sets the boolean value if the cell has been checked 
{
	m_beenChecked = _checked;
}

//the functions below are self explantary for setting and getting variables 

bool Cell::HasCellBeenTransversed()
{
	return m_beenTransversed;
}
void Cell::CellBeenTransversed(bool _transversed)
{
	m_beenTransversed = _transversed;
}
bool Cell::ShouldTheCellDisplayAPointerToParent()
{
	return m_displayPointerToParent;
}
void Cell::DisplayPointer(bool _displayPointer)
{
	m_displayPointerToParent = _displayPointer;
}


void Cell::SetCostToGoal(Position _goalNode) //sets the cost to the goal
{
	m_costToGoal = pow(((pow(m_position.m_x - _goalNode.m_x, 2)) + (pow(m_position.m_y - _goalNode.m_y, 2))), 0.5) * 10; //this uses pythagoras to get the value, it is then multiplied by 10 to remove the problem of integers not being floats 
}
void Cell::SetCostFromNode(std::shared_ptr<Cell::Position> &cellPosition, std::vector<std::vector<std::shared_ptr<Cell>>>& nodes)
{
	m_costToNode = nodes[m_parentNode.m_y][m_parentNode.m_x]->GetCostFromNode() + 10; //gets the previous parents value from the node and adds 10 to it
}

//the functions below are self explantary for setting and getting variables 

int Cell::GetCostToGoal()
{
	return m_costToGoal;
}
int Cell::GetCostFromNode()
{
	return m_costToNode;
}
int Cell::GetTotalCost()
{
	return m_costToGoal + m_costToNode; //this just adds the two g and h costs together to make an f cost
}
int Cell::GetSmallHexagonHeight()
{
	return m_smallHexagonHeight;
}

int Cell::ReturnVertex(int _vertex, int xOrY) //returns the vertex values of the hexagon 
{
	if (xOrY == 0) //0 to return X
	{
		return m_vertices[_vertex].m_x;
	}
	else
	{
		return m_vertices[_vertex].m_y;
	}
}

void Cell::SetTotalCost() //sets the total cost 
{
	m_totalCost = m_costToGoal + m_costToNode;
}
Cell::Position Cell::GetOrigin() //returns the origin 
{
	return m_origin;
}

void Cell::SetBlock(Cell::Value _value) //by using enums you can set the block, so instead of Block = 1 it uses Value = Start; 
{
	//switch statement to set the different properties of the block 
	switch (_value)
	{
	case Cell::Wall:
		m_colour = al_map_rgb(123, 12, 255);
		m_cellValue = Value::Wall;
		m_displayPointerToParent = false;
		break;
	case Cell::Tile:
		m_colour = al_map_rgb(100, 100, 100);
		m_cellValue = Value::Tile;
		m_displayPointerToParent = true;
		m_beenChecked = false;
		m_beenTransversed = false;
		m_costToGoal = NULL;
		m_costToNode = NULL;
		m_totalCost = NULL;
		break;
	case Cell::CheckedTile:
		//m_colour = al_map_rgb(42, 120, 242); //for testing use, allows the user to see the cells that have been checked 
		m_cellValue = Value::CheckedTile;
		m_beenChecked = true;
		m_displayPointerToParent = true;
		break;
	case Cell::Start:
		//m_colour = al_map_rgb(73, 166, 40);
		m_cellValue = Value::Start;
		m_displayPointerToParent = false;
		m_beenChecked = true;
		break;
	case Cell::End:
		//m_colour = al_map_rgb(166, 10, 10);
		m_cellValue = Value::End;
		m_displayPointerToParent = false;
		break;
	case Cell::Powerup:
		SetBlock(Cell::Tile);
		m_cellValue = Value::Powerup;
		//m_colour = al_map_rgb(42, 120, 242);
		break;
	case Cell::Score:
		SetBlock(Cell::Tile);
		m_cellValue = Value::Score;
		//m_colour = al_map_rgb(73, 166, 40);
	default:
		break;
	}
}

Cell::Value Cell::GetBlock() //returns the block type 
{
	return m_cellValue;
}

void Cell::ResetCell()
{
	m_displayPointerToParent = true;
	m_beenChecked = false;
	m_beenTransversed = false;
	m_costToGoal = NULL;						//function to reset the cells values
	m_costToNode = NULL;
	m_totalCost = NULL;
	m_parentNode = Cell::Position(NULL, NULL);
}