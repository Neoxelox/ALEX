#include "Game.hpp"
#include "SplashState.hpp"
#include <stdlib.h>
#include <time.h>

namespace Alex
{
	Game::Game(int width, int height, std::string title)
	{
		srand(time(NULL));
		
		if(FULLSCREEN) _data->window.create(sf::VideoMode::getFullscreenModes()[0], title, sf::Style::Fullscreen);
		else _data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

		_data->window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
		//If you comment this, you will have very smooth game but also very consuming
		_data->window.setFramerateLimit(FRAME_LIMIT);
		//_data->window.setVerticalSyncEnabled(false);

		_data->window.setMouseCursorVisible(MOUSE_VISIBLE);
		
		_data->machine.AddState(StateRef(new SplashState(this->_data)));

		this->Run();
	}

	void Game::Run()
	{
		float newTime, frameTime, interpolation;
		float currentTime = this->_clock.getElapsedTime().asSeconds();
		float accumulator = 0.0f;

		while (this->_data->window.isOpen())
		{
			this->_data->machine.ProcessStateChanges();

			newTime = this->_clock.getElapsedTime().asSeconds();
			frameTime = newTime - currentTime;

			if (frameTime > 0.25f) {
				frameTime = 0.25f;
			}

			currentTime = newTime;
			accumulator += frameTime;

			while (accumulator >= dt) {
				this->_data->machine.GetActiveState()->HandleInput();
				this->_data->machine.GetActiveState()->Update(dt);

				accumulator -= dt;
			}

			interpolation = accumulator / dt;
			this->_data->machine.GetActiveState()->Draw(interpolation);
		}
	}
}