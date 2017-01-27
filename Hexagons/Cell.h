#ifndef CELL_H
#define CELL_H		//defines the header file 

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <vector>								//uses these addons 
#include <memory>

class Cell		//defines cell class 
{
public:
	enum Value
	{
		Wall,
		Tile,
		CheckedTile,					//an enum which defines the type of cell it is 
		Start,
		Powerup,
		Score,
		End
	};
	struct Position
	{
		double m_x, m_y;					//instead of using returnx, returny, i can define functions which return the x and y together 
		Position()
		{

		}
		Position(int _x, int _y)
		{
			m_x = _x;
			m_y = _y;
		}		
	};
	Cell();
	Cell(int _width, int _x, int _y, int _parentX, int _parentY, ALLEGRO_COLOR _colour, enum Value _value, int _hexagonSpacing);
	~Cell();
	int GetHexagonWidth();
	void SetHexagonWidth(int _width);
	Position GetPosition();
	void SetPosition(int _x, int _y);
	ALLEGRO_COLOR GetColour();
	void SetColour(int _r, int _g, int _b);				//a definition of all the functions used in the classes 
	Position GetParentPosition();
	void SetParentPosition(int _x, int _y);
	bool HasCellBeenChecked();
	void CellBeenChecked(bool _checked);
	bool HasCellBeenTransversed();
	void CellBeenTransversed(bool _transversed);
	bool ShouldTheCellDisplayAPointerToParent();
	void DisplayPointer(bool _pointer);
	void SetCostToGoal(Position _goalNode);
	void SetCostFromNode(std::shared_ptr<Cell::Position> &cellPosition, std::vector<std::vector<std::shared_ptr<Cell>>>& nodes);
	int GetCostToGoal();
	int GetCostFromNode();
	int GetTotalCost();
	int GetSmallHexagonHeight();
	int ReturnVertex(int _vertex, int xOrY);	
	Position GetOrigin();
	void SetBlock(Value _value);
	Value GetBlock();
	void SetTotalCost();
	void ResetCell();
private:	
	Position m_vertices[6];
	ALLEGRO_COLOR m_colour;
	Position m_position, m_parentNode, m_origin;								//the variables used in the class, all set to private 
	Value m_cellValue;
	bool m_beenChecked, m_beenTransversed, m_displayPointerToParent;
	int m_costToGoal, m_costToNode, m_totalCost, m_hexagonWidth;
	float m_smallHexagonHeight;
};
#endif

