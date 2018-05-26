#ifndef game_h
#define game_h
enum GameState { GS_WORLD, GS_MENU, GS_PAUSE, GS_WORLD_TO_MENU, GS_MENU_TO_WORLD, GS_WORLD_TO_PAUSE, GS_PAUSE_TO_WORLD, GS_PAUSE_TO_MENU, GS_EXIT };
enum LevelId { LV_NULL, LV_1, LV_2, LV_3};
class game_test;
#include "engine.hpp"
#include "levels.hpp"
#include "menu.hpp"
#include <SFML/Graphics.hpp>

class game_test{
	public:
	game_test(sf::VideoMode &vm);
	~game_test();
	void loadLevel(LevelId lvl);
	bool tick(PlayerInput &pin);
	sf::Sprite& getFrame();
	void setState(GameState st);
	sf::VideoMode vm;
	private:
	GameLevel* level;
	MainMenuScreen cs;
	PauseMenuScreen ps;
	TransitionEffect transition;
	GameState state;
	sf::Sprite s;
	sf::RenderTexture frame;

};
#endif
