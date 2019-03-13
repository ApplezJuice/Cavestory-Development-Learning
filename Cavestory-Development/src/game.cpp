/* Game class
	Holds all info for our main game loop
*/
#include <glew.h>
#include <SDL.h>
#include "headers/game.h"
#include "headers/graphics.h"
#include "headers/input.h"

#include <algorithm>
#include <iostream>

namespace
{
	const int FPS = 50;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game()
{
	_gameState = GameState::PLAY;
	// SDL init is used when you want to modify initializations
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop(); // calls the gameloop from our constructor
}

Game::~Game() // destructor - destroys the game
{

}

void Game::gameLoop() // happens every frame. very important
{
	// create the graphics object
	Graphics graphics;

	this->_player = Sprite(graphics, "src/content/sprites/MyChar.png", 0, 0, 16, 16, 100, 100);

	int lastUpdateTime = SDL_GetTicks(); // gets the number ms since the sdl library was initialized

	// start the game loop
	while (_gameState != GameState::EXIT)
	{
		processInput();
		graphics.drawGame();

		// Right before the game closes, it gets the time the whole game loop took
		const int CURRENT_TIME_MS = SDL_GetTicks();
		int elapsedTimeMS = CURRENT_TIME_MS - lastUpdateTime;
		// max frame time is the maximum amount of time you are allowing for a frame
		this->update(std::min(elapsedTimeMS, MAX_FRAME_TIME));
		lastUpdateTime = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::draw(Graphics &graphics)
{
	graphics.clear();

	this->_player.draw(graphics, 100, 100);

	graphics.flip();
}

void Game::update(float elapsedTime)
{

}

void Game::processInput()
{
	Input input;
	// SDL event object, will hold any event that happend during that frames
	SDL_Event evnt;

	// first thing to do, is begin a new frame
	input.beginNewFrame();

	if (SDL_PollEvent(&evnt)) // runs this if statement if there is an event
	{
		switch(evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

		case SDL_KEYDOWN:
			if (evnt.key.repeat == 0) // checks to see if a key is not held down
			{
				input.keyDownEvent(evnt);
			}
			break;

		case SDL_KEYUP:
			input.keyUpEvent(evnt);
			break;

		case SDL_MOUSEMOTION:
			std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
		}
	}
	if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
	{
		_gameState = GameState::EXIT;
	}
}