
#ifndef levels_h
#define levels_h
#include "engine.hpp"
#include "objects.hpp"
#include "game.hpp"
class lv_test: public GameLevel{
	public:
	lv_test(sf::VideoMode &vm, game_test* game, float dif, LevelId next);
	void updatePhysics(PlayerInput &pin) override;
	void redraw() override;
	void redraw_static() override;
	void hybernate() override;
	void hurt(float damage) override;
	const sf::Texture& getTexture() override;
	void levelLogic(int id, float param) override;
	private:
	GameWorld gw;
	float hp;
	sf::Shader glitch;
	sf::RenderTexture frame;
	sf::Sprite s;
	sf::Clock clock;
	game_test* game;
	sf::Clock spawnTimer;
	float dif;
	LevelId next;
	int crystals_remaining;
	Player* player;
};


#endif
