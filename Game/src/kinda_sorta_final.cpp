#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include "engine.hpp"
#include "menu.hpp"
#include "game.hpp"
#define MAX_FPS 60.0


int main () {
	sf::Clock frameclock;
	sf::VideoMode vm;
	vm=vm.getFullscreenModes()[0];
	sf::RenderWindow window(vm, "My window", sf::Style::Fullscreen);
	sf::Clock clock3;
	PlayerInput pin;	
	game_test game(vm);
	bool close = 0;
	std::cout<<"Entering main loop \n";
	while (window.isOpen())
	{
		if(close){
			window.close();
			break;

		}
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed){
				window.close();
				break;
			}
		}
		pin.forward=sf::Keyboard::isKeyPressed(sf::Keyboard::W)||sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		pin.back=sf::Keyboard::isKeyPressed(sf::Keyboard::S)||sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		pin.left=sf::Keyboard::isKeyPressed(sf::Keyboard::A)||sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		pin.right=sf::Keyboard::isKeyPressed(sf::Keyboard::D)||sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		pin.stop=sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		pin.flashlight=sf::Keyboard::isKeyPressed(sf::Keyboard::F);
		pin.fire=sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		pin.menu=sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
		pin.select=sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
		close=game.tick(pin);
		window.draw(game.getFrame());
		window.display();
		sf::Time frametime = frameclock.getElapsedTime();
		if(1/MAX_FPS-frametime.asSeconds()>0.001) usleep((1/MAX_FPS-frametime.asSeconds())*1000000);
		frametime = frameclock.getElapsedTime();
		window.setTitle("FPS: " + std::to_string((int)(1/frametime.asSeconds())));
		frameclock.restart();


	}

}
