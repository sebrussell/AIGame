#include "Player.h"

Player::Player(int hexagonWidth)
{
	SetBitmap("Images\\Player.bmp");
	SetImageValues(0, 64, 56, 65, 0, 0, 500, 519, 0.001, hexagonWidth);
	playerPosition = Cell::Position(100, 100);
	playerSpeed = 200;		
	playerHealth = 100;
	pickedUpAttacker = false;
	fleeCoolDownAmount = 3;
}																			//constructor and deconstructor

Player::~Player()
{
}

void Player::Draw()
{
	DrawAnimation(playerPosition);					//calls the draw function passing in the players position
}

void Player::ChangePosition(Cell::Position positionChangedBy, double deltaTime, std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int hexagonWidth, int cellsX, int cellsY)
{
	MouseClick tempMouse;
	Cell::Position gridToGoInto;

	Cell::Position tempPlayer = playerPosition;
	tempPlayer.m_x += positionChangedBy.m_x * playerSpeed * deltaTime;						//works out what position they are going to go to
	tempPlayer.m_y += positionChangedBy.m_y * playerSpeed * deltaTime;

	gridToGoInto = tempMouse.GetMouseClickPosition(nodes, hexagonWidth, tempPlayer.m_x, tempPlayer.m_y, cellsX, cellsY);	//gets the hexagon value that they are going to

	if (gridToGoInto.m_x == -1 && gridToGoInto.m_y == -1)
	{
		gridToGoInto = hexagonPosition;							//checks that they arent going to an invalid location, if they are sets it to the current hexagon position
	}

	if (gridToGoInto.m_x >= 0 && gridToGoInto.m_x < cellsX)
	{																		//if you arent going to go out of bounds
		if (gridToGoInto.m_y >= 0 && gridToGoInto.m_y < cellsY)
		{
			if (nodes[gridToGoInto.m_y][gridToGoInto.m_x]->GetBlock() != Cell::Value::Wall)
			{
				playerPosition.m_x += positionChangedBy.m_x * playerSpeed * deltaTime;
				playerPosition.m_y += positionChangedBy.m_y * playerSpeed * deltaTime;							//depending on which cell you are in
			}																									//do a certain function
			if (nodes[gridToGoInto.m_y][gridToGoInto.m_x]->GetBlock() == Cell::Value::Powerup)
			{
				pickedUpAttacker = true;
				fleeCoolDownSoFar = 0;
			}
			if (nodes[gridToGoInto.m_y][gridToGoInto.m_x]->GetBlock() == Cell::Value::Score)
			{
				playerScore += 1;
			}
		}
	}

}

Cell::Position Player::GetPosition()
{
	return playerPosition;
}

Cell::Position Player::GetHexagonPosition()
{
	return hexagonPosition;
}

void Player::SetHexagonPosition(Cell::Position pos)
{
	hexagonPosition = pos;
}																//getters and setters

void Player::TakeDamage(float damageAmount)
{
	playerHealth -= damageAmount;
}

float Player::GetHealth()
{
	return playerHealth;
}

bool Player::ShouldZombieFlee()
{
	return pickedUpAttacker;
}

int Player::GetScore()
{
	return playerScore;
}

void Player::FleeTimer(double deltaTime)
{
	fleeCoolDownSoFar += deltaTime;										//increase the time by delta time
	if (fleeCoolDownSoFar > fleeCoolDownAmount)			//if the powerup has cooled down
	{
		pickedUpAttacker = false;							//zombies no longer need to flee
	}
}
