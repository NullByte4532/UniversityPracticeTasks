#ifndef menu_h
#define menu_h
#include "engine.hpp"
class game_test;
#include <SFML/Graphics.hpp>
class ConsoleText: public ConsoleObject{
	public:
	void redraw(sf::RenderTexture &frame) override;
	void setText(std::string str);
	void setColor(sf::Color color);
	void select();
	void deselect();
	ConsoleText(sf::Vector2f position, sf::Vector2f size, ConsoleScreen* cs, std::string str, int charsize, sf::Color color, std::string str_sel, sf::Color color_sel);
	ConsoleText(sf::Vector2f position, sf::Vector2f size, ConsoleScreen* cs, std::string str, int charsize, sf::Color color);
	private:
	sf::Text text;
	sf::Font font;
	sf::Color color;
	sf::Color color_sel;
	std::string str;
	std::string str_sel;

};

class MainMenuScreen: public ConsoleScreen{
	public:
	void updatePhysics(PlayerInput &pin) override;
	MainMenuScreen(sf::VideoMode &vm, game_test* game);
	protected:
	ConsoleText* title;
	ConsoleText* options[3];
	int selected;
	sf::Clock clock;
	float keyTimeout;
	PlayerInput lastPin;
	game_test* game;


};

class PauseMenuScreen: public ConsoleScreen{
	public:
	void updatePhysics(PlayerInput &pin) override;
	PauseMenuScreen(sf::VideoMode &vm, game_test* game);
	protected:
	ConsoleText* title;
	ConsoleText* options[5];
	int selected;
	sf::Clock clock;
	float keyTimeout;
	PlayerInput lastPin;
	game_test* game;


};
#endif

