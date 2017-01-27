#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <cmath>
#include "allegro5\allegro.h"
#include "allegro5\allegro_primitives.h"			//defines all the other files that will be used
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include <vector>
#include <memory>
#include "Cell.h"
#include "Agent.h"										//probably have too many header files listed here, would be better to put them in a new file
#include "Player.h"
#include "ZombieBase.h"
#include "SmartZombie.h"
#include "MouseClick.h"
#include "IdleState.h"
#include "SearchState.h"
#include <fstream>
#include <iterator>

// nice colour (42, 120, 242);

const float FPS = 60;
const int SCREEN_W = 900;					//sets some default values for the program 
const int SCREEN_H = 900;
const int BOUNCER_SIZE = 32;

//list of functions so main can use them, would be better in a different file 

int InitialiseAllegro(ALLEGRO_TIMER * timer, ALLEGRO_DISPLAY * display);
void CreateCells(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int _desiredHexagonAmountX, int _desiredHexagonAmountY, int _hexagonWidth, int _hexagonSpacing, ALLEGRO_COLOR _colour);
void Draw(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int amountOfCellsY, int amountOfCellsX, ALLEGRO_FONT* font, std::vector<std::shared_ptr<ZombieBase>>& agentList, std::shared_ptr<Player>& player);
void UserInput(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int hexagonWidth, int amountOfCellsX, int amountOfCellsY, std::vector<std::shared_ptr<ZombieBase>>& agentList, std::shared_ptr<Player>& player, double deltaTime);
void UpdatePlayerPosition(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int hexagonWidth, int amountOfCellsX, int amountOfCellsY, std::shared_ptr<Player>& player, std::vector<std::shared_ptr<ZombieBase>>& agentList);
void SpawnZombies(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int hexagonWidth, int amountOfCellsX, int amountOfCellsY, std::vector<std::shared_ptr<ZombieBase>>& agentList, int zombieAmount, int difficultyPercent);

int main(int argc, char **argv)															//start of main 
{
	int amountOfCells = 400;															//sets the amount of cells you want
	int amountOfCellsX = pow(amountOfCells, 0.5);										//gets the amount of cells in y direction by square routing the total cells
	int amountOfCellsY = amountOfCellsX;												//same value as y
	int hexagonWidth = (SCREEN_W * 0.95) / amountOfCellsX;								//gets the size by dividng the screen_w / total cells in row 
	hexagonWidth = SCREEN_H / amountOfCellsY * (2 / pow(3, 0.5));
	int hexagonSpacing = 1;
	bool done = false;
	std::shared_ptr<bool> startPathfinding = std::make_shared<bool>(false);					//more default variables 

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;														//more default variables 
	ALLEGRO_EVENT ev;

	double deltaTime = 0;
	double totalTime = 0;
	std::clock_t begin, end;

	srand(time(NULL));
	InitialiseAllegro(timer, display);     //initialises allegro, randomness and starts the display									

	Player tempPlayer(hexagonWidth);
	std::shared_ptr<Player> player = std::make_shared<Player>(tempPlayer);

	std::vector<std::shared_ptr<ZombieBase>> agentList;            //more default variables 

	ALLEGRO_FONT *font = al_load_ttf_font("TimesNewRoman.ttf", 10, 0);				//used for the text 
	std::vector<std::vector<std::shared_ptr<Cell>>> nodes;

	CreateCells(nodes, amountOfCellsX, amountOfCellsY, hexagonWidth, hexagonSpacing, al_map_rgb(100, 100, 100));			//creates the cells
	SpawnZombies(nodes, hexagonWidth, amountOfCellsX, amountOfCellsY, agentList, 10, 80);



	while (player->GetScore() < 40 && player->GetHealth() > 0)																										//loop through until done
	{
		begin = clock();
		Draw(nodes, amountOfCellsY, amountOfCellsX, font, agentList, player);
		UserInput(nodes, hexagonWidth, amountOfCellsX, amountOfCellsY, agentList, player, deltaTime);						//calls the input function
		player->FleeTimer(deltaTime);									
		for (size_t i = 0; i < agentList.size(); i++)
		{		
			agentList[i]->manager->update(std::make_shared<double>(deltaTime), player);
		}

		end = clock();
		deltaTime = double(end - begin) / CLOCKS_PER_SEC;															//this is used for delta time and timers
		totalTime += deltaTime;
	}


	////END DISPLAY TEXT
	al_flip_display();
	al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 60, ALLEGRO_ALIGN_CENTRE, "Player Score: %d", player->GetScore());
	al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 70, ALLEGRO_ALIGN_CENTRE, "Player Health: %d", (int)player->GetHealth());
	al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 80, ALLEGRO_ALIGN_CENTRE, "Total Time: %d", (int)totalTime);
	al_flip_display();
	al_rest(5);

	return 0;
}



int InitialiseAllegro(ALLEGRO_TIMER * timer, ALLEGRO_DISPLAY * display)
{
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize the prar!\n");
		return -1;
	}

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}
	return 0;
}

void CreateCells(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int _desiredHexagonAmountX, int _desiredHexagonAmountY, int _hexagonWidth, int _hexagonSpacing, ALLEGRO_COLOR _colour)
{
	for (size_t y = 0; y < _desiredHexagonAmountY; y++)												//goes throough the deisred x and y amount
	{
		std::vector<std::shared_ptr<Cell>> row;
		for (size_t x = 0; x < _desiredHexagonAmountX; x++)
		{
			row.push_back(nullptr);											//sets the row to null
		}
		nodes.push_back(row);													//pushes that row into another row (2d vector)
	}


	for (size_t y = 0; y < _desiredHexagonAmountY; y++)
	{
		for (size_t x = 0; x < _desiredHexagonAmountX; x++)					//goes through again
		{
			std::shared_ptr<Cell> tempCell(new Cell(_hexagonWidth, x, y, NULL, NULL, _colour, Cell::Tile, _hexagonSpacing));
			nodes[y][x] = tempCell;																							//making each cell default to a default 'tile'
		}
	}

	srand(time(NULL));
	int count = 0;
	int x, y;
	do
	{
		x = rand() % _desiredHexagonAmountX;
		y = rand() % _desiredHexagonAmountY;
		if (nodes[y][x]->GetBlock() != Cell::Value::Score)
		{
			nodes[y][x]->SetBlock(Cell::Value::Score);
			count += 1;
		}
	} while (count < 40);

	count = 0;
	do
	{
		x = rand() % _desiredHexagonAmountX;
		y = rand() % _desiredHexagonAmountY;
		if (nodes[y][x]->GetBlock() != Cell::Value::Powerup && nodes[y][x]->GetBlock() != Cell::Value::Score)
		{
			nodes[y][x]->SetBlock(Cell::Value::Powerup);
			count += 1;
		}
	} while (count < 4);

}

void Draw(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int amountOfCellsY, int amountOfCellsX, ALLEGRO_FONT* font, std::vector<std::shared_ptr<ZombieBase>>& agentList, std::shared_ptr<Player>& player)
{

	Cell::Position origin(NULL, NULL), parentPosition(NULL, NULL);						//sets some temporary values
	int r, r2;

	for (size_t y = 0; y < amountOfCellsY; y++)
	{																					//loops through the 2d vector
		for (size_t x = 0; x < amountOfCellsX; x++)
		{
			for (size_t i = 0; i < 4; i++)													//as there are 4 triangles to be drawn, it loops through 4 times 
			{
				al_draw_filled_triangle(nodes[y][x]->ReturnVertex(0, 0), nodes[y][x]->ReturnVertex(0, 1), nodes[y][x]->ReturnVertex(1 + i, 0), nodes[y][x]->ReturnVertex(1 + i, 1),
					nodes[y][x]->ReturnVertex(2 + i, 0), nodes[y][x]->ReturnVertex(2 + i, 1), nodes[y][x]->GetColour());
			}
			origin = nodes[y][x]->GetOrigin();

			if (nodes[y][x]->GetBlock() == Cell::Value::Score)
			{
				al_draw_filled_circle(origin.m_x, origin.m_y, 5, al_map_rgb(73, 166, 40));
			}

			if (nodes[y][x]->GetBlock() == Cell::Value::Powerup)
			{
				al_draw_filled_circle(origin.m_x, origin.m_y, 5, al_map_rgb(42, 120, 242));
			}

			//DRAWING FUNCTIONS USED FOR TESTING AND DISPLAYING DIFFERENT DATA, UNCOMMENT TO SHOW THE DATA

			////al_draw_textf(font, al_map_rgb(255, 255, 255), origin.m_x, origin.m_y, ALLEGRO_ALIGN_CENTRE, "%d : %d", x, y);
			//al_draw_textf(font, al_map_rgb(255, 255, 255), origin.m_x, origin.m_y, ALLEGRO_ALIGN_CENTRE, "%d", nodes[y][x]->HasCellBeenTransversed());
		}
	}

	player->Draw();

	for (size_t i = 0; i < agentList.size(); i++)
	{
		agentList[i]->Draw();
		//agentList[i]->DrawPath(nodes);
	}

	al_draw_textf(font, al_map_rgb(255, 255, 255), 30, 810, ALLEGRO_ALIGN_CENTRE, "Player Score: %d", player->GetScore());
    al_draw_textf(font, al_map_rgb(255, 255, 255), 30, 820, ALLEGRO_ALIGN_CENTRE, "Player Health: %d", (int)player->GetHealth());


	al_flip_display();								//flip the display
	al_clear_to_color(al_map_rgb(0, 0, 0));			//clear to col
}

void UserInput(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int hexagonWidth, int amountOfCellsX, int amountOfCellsY, std::vector<std::shared_ptr<ZombieBase>>& agentList, std::shared_ptr<Player>& player, double deltaTime)
{
	enum MousePressed
	{
		LeftClick,
		RightClick,
		MiddleClick,																				//enum value for the different mouse clicks
		None
	};
	enum KeyboardPressed
	{
		SpaceBar,
		Ctrl,
		W,
		A,
		S,
		D,                                                                                          //enum value for the different keyboard clicks
		_None
	};
	ALLEGRO_MOUSE_STATE mouseState;
	ALLEGRO_KEYBOARD_STATE keyboardState;
	bool pressed = false;
	MousePressed buttonPressed = None;																//some variables 
	KeyboardPressed keyPressed = _None;
	Cell::Position mouseClickPosition;
	Cell::Position currentPlayer;
	MouseClick tempMouse;
	

	al_get_mouse_state(&mouseState);																//gets the current keyboard and mouse state 
	al_get_keyboard_state(&keyboardState);

	if (mouseState.buttons & 1 && pressed == false)													//if you have pressed mouse 1 
	{
		pressed = true;
		buttonPressed = LeftClick;
	}
	if (mouseState.buttons & 2 && pressed == false)													//using pressed == false to prevent it program doing multiple iterations
	{																								//if the mouse button is held down
		pressed = true;																					//should probably look into using events 
		buttonPressed = RightClick;
	}
	if (mouseState.buttons & 4 && pressed == false)
	{
		pressed = true;
		buttonPressed = MiddleClick;
	}
	if (al_key_down(&keyboardState, ALLEGRO_KEY_SPACE))
	{
		pressed = true;																				//space bar and middle mouse and right click above
		keyPressed = SpaceBar;
	}
	if (al_key_down(&keyboardState, ALLEGRO_KEY_W))
	{
		pressed = true;
		keyPressed = W;
	}
	if (al_key_down(&keyboardState, ALLEGRO_KEY_A))
	{
		pressed = true;
		keyPressed = A;
	}
	if (al_key_down(&keyboardState, ALLEGRO_KEY_S))
	{
		pressed = true;
		keyPressed = S;
	}
	if (al_key_down(&keyboardState, ALLEGRO_KEY_D))
	{
		pressed = true;
		keyPressed = D;
	}

	

	Cell::Position startCell = Cell::Position(-1, -1);
	Cell::Position endCell = Cell::Position(-1, -1);	

	if (pressed == true)																														//if you have pressed something
	{
		mouseClickPosition = tempMouse.GetMouseClickPosition(nodes, hexagonWidth, mouseState.x, mouseState.y, amountOfCellsX, amountOfCellsY);			//gets the mouse click position
		pressed = false;
		if (buttonPressed == LeftClick)
		{
			if (mouseClickPosition.m_y != -1 && mouseClickPosition.m_x != -1)
			{
				ZombieBase tempAgent(hexagonWidth);
				if (startCell.m_x != -1 && startCell.m_y != -1)
				{
					nodes[startCell.m_y][startCell.m_x]->SetBlock(Cell::Value::Tile);
				}
				tempAgent.SetPosition(mouseClickPosition);
				tempAgent.SetTargetPosition(player->GetHexagonPosition());
				tempAgent.SetDrawPosition(nodes[mouseClickPosition.m_y][mouseClickPosition.m_x]->GetOrigin());

				std::shared_ptr<ZombieBase> z = std::make_shared<ZombieBase>(tempAgent);
				agentList.push_back(z);

				z->manager->setState(std::shared_ptr<SearchState>(new SearchState(z->manager, z, nodes)));
			}
		}
		else if (buttonPressed == RightClick)
		{
			if (mouseClickPosition.m_y != -1 && mouseClickPosition.m_x != -1)
			{
				nodes[mouseClickPosition.m_y][mouseClickPosition.m_x]->SetBlock(Cell::Value::Wall);						//does different things depending on which button was pressed 
				for (size_t i = 0; i < agentList.size(); i++)
				{
					agentList[i]->StartPathFind();
				}
			}
		}

		UpdatePlayerPosition(nodes, hexagonWidth, amountOfCellsX, amountOfCellsY, player, agentList);		

		if (keyPressed == KeyboardPressed::SpaceBar)
		{

		}
		if (keyPressed == KeyboardPressed::W)
		{
			player->ChangePosition(Cell::Position(0, -1), deltaTime, nodes, hexagonWidth, amountOfCellsX, amountOfCellsY);
		}
		if (keyPressed == KeyboardPressed::A)
		{
			player->ChangePosition(Cell::Position(-1, 0), deltaTime, nodes, hexagonWidth, amountOfCellsX, amountOfCellsY);
		}
		if (keyPressed == KeyboardPressed::S)
		{
			player->ChangePosition(Cell::Position(0, 1), deltaTime, nodes, hexagonWidth, amountOfCellsX, amountOfCellsY);
		}
		if (keyPressed == KeyboardPressed::D)
		{
			player->ChangePosition(Cell::Position(1, 0), deltaTime, nodes, hexagonWidth, amountOfCellsX, amountOfCellsY);
		}
	}
}

void UpdatePlayerPosition(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int hexagonWidth, int amountOfCellsX, int amountOfCellsY, std::shared_ptr<Player>& player, std::vector<std::shared_ptr<ZombieBase>>& agentList)
{
	MouseClick tempMouse;
	Cell::Position temp;
	temp = player->GetHexagonPosition();

	if (player->GetHexagonPosition().m_y != -1 && player->GetHexagonPosition().m_x != -1)
	{
		nodes[player->GetHexagonPosition().m_y][player->GetHexagonPosition().m_x]->SetBlock(Cell::Value::Tile);
	}

	Cell::Position playerPos = player->GetPosition();
	playerPos = tempMouse.GetMouseClickPosition(nodes, hexagonWidth, playerPos.m_x, playerPos.m_y, amountOfCellsX, amountOfCellsY);

	player->SetHexagonPosition(playerPos);

	if (playerPos.m_y != -1 && playerPos.m_x != -1)
	{
		if (playerPos.m_x < amountOfCellsX && playerPos.m_y < amountOfCellsY && playerPos.m_x >= 0 && playerPos.m_y >= 0)
		{
			nodes[playerPos.m_y][playerPos.m_x]->SetBlock(Cell::Value::Tile);
		}
	}

	if (temp.m_x != playerPos.m_x || temp.m_y != playerPos.m_y)
	{
		for (size_t i = 0; i < agentList.size(); i++)
		{
			if (playerPos.m_x < amountOfCellsX && playerPos.m_y < amountOfCellsY && playerPos.m_x >= 0 && playerPos.m_y >= 0)
			{
				agentList[i]->SetTargetPosition(playerPos);
			}
			agentList[i]->StartPathFind();
		}
	}
}

void SpawnZombies(std::vector<std::vector<std::shared_ptr<Cell>>>& nodes, int hexagonWidth,  int amountOfCellsX, int amountOfCellsY, std::vector<std::shared_ptr<ZombieBase>>& agentList, int zombieAmount, int difficultyPercent)
{
	float cleverZombieAmount = 0;
	cleverZombieAmount = (difficultyPercent * zombieAmount) / 100;			//the amount with the percentage
	int stupidZombieAmount = zombieAmount - cleverZombieAmount;
	int x, y;
	int counter = 0;

	srand(time(NULL));
	do
	{
		SmartZombie tempAgent(hexagonWidth);
		x = rand() % amountOfCellsX;
		y = rand() % amountOfCellsY;

		if (nodes[y][x]->GetBlock() != Cell::Value::Wall)
		{
			tempAgent.SetPosition(Cell::Position(x, y));
			tempAgent.SetDrawPosition(nodes[y][x]->GetOrigin());
			std::shared_ptr<SmartZombie> z = std::make_shared<SmartZombie>(tempAgent);										//make a temporary variabe
			z->SetSpawnPoint(Cell::Position(x, y));
			agentList.push_back(z);																							//add the zombie to the list of zombies
			z->manager->setState(std::shared_ptr<SearchState>(new SearchState(z->manager, z, nodes)));						//set the default start state in the list of states to search state
			counter += 1;
		}
	} while (counter < cleverZombieAmount);

	counter = 0;

	do
	{
		ZombieBase tempAgent(hexagonWidth);
		x = rand() % amountOfCellsX;
		y = rand() % amountOfCellsY;

		if (nodes[y][x]->GetBlock() != Cell::Value::Wall)
		{
			tempAgent.SetPosition(Cell::Position(x, y));
			tempAgent.SetDrawPosition(nodes[y][x]->GetOrigin());															//same as other while loop
			std::shared_ptr<ZombieBase> z = std::make_shared<ZombieBase>(tempAgent);
			z->SetSpawnPoint(Cell::Position(x, y));
			agentList.push_back(z);
			z->manager->setState(std::shared_ptr<IdleState>(new IdleState(z->manager, z, nodes)));							//set the zombie state to idle
			counter += 1;
		}
	} while (counter < stupidZombieAmount);
}