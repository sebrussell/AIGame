#pragma once
#include "Image.h"					//header files to include
#include "Cell.h"
#include "MouseClick.h"



class Player : public Image			//inherits from the image class
{
public:
	Player(int hexagonWidth);
	~Player();											//the functions that are used
	void Draw();
	
	void ChangePosition(Cell::Position positionChangedBy, double deltaTime, std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int hexagonWidth, int cellsX, int cellsY);
	Cell::Position GetPosition();
	Cell::Position GetHexagonPosition();
	void SetHexagonPosition(Cell::Position pos);
	void TakeDamage(float damageAmount);
	
	bool ShouldZombieFlee();
	int GetScore();
	float GetHealth();
	void FleeTimer(double deltaTime);
private:
	Cell::Position playerPosition, hexagonPosition;
	float playerSpeed;
	float playerHealth;
	bool pickedUpAttacker;											//the private variables that are used
	int playerScore;

	int fleeCoolDownAmount;
	double fleeCoolDownSoFar;
};

