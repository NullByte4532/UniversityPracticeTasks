#ifndef objects_h
#define objects_h
#include "engine.hpp"
class GlowingBush: public GameObject{
	public:
	GlowingBush(std::string texture, sf::Vector3f color, sf::Vector2f position, sf::Vector2f size, GameWorld* gw);
	~GlowingBush();
	void updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin) override;
	void redraw_static(sf::RenderTexture &frame, LightingManager* lightingManager) override;
	void redraw(sf::RenderTexture &frame, LightingManager* lightingManager) override;
	void collision(GameObject* fellow, sf::Vector2f newvel);
	private:
	Obstruction* shadow;
	Light* light;
	float energy;
	sf::Clock flickerClock;
	sf::Clock mineClock;
	bool off;
};

class Spider: public GameObject{
	public:
	Spider(sf::Vector2f position, sf::Vector2f size, GameWorld* gw);
	~Spider();
	void updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin) override;
	void redraw_static(sf::RenderTexture &frame, LightingManager* lightingManager) override;
	void redraw(sf::RenderTexture &frame, LightingManager* lightingManager) override;
	void resetClocks() override;
	void setTarget(GameObject* target){ this->target=target;};
	void setDirection(sf::Vector2f dir);
	void collision(GameObject* fellow, sf::Vector2f newvel);
	void die();
	sf::Clock attackClock;
	private:
	sf::Vector2f acceleration;
	int state;
	GameObject* target;
	Obstruction* shadow;
	sf::Texture animation[6];
	int anim_cur_frame;
	sf::Clock animationClock;
	sf::Clock interframeClock;
	sf::Clock pathClock;
	bool dead;

};

class Fireball: public GameObject{
	public:
	void updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin) override;
	void redraw(sf::RenderTexture &frame, LightingManager* lightingManager) override;
	Fireball(std::string texture, sf::Vector2f position, sf::Vector2f size, sf::Vector2f velocity, float time, GameWorld* gw);
	~Fireball();
	void collision(GameObject* fellow, sf::Vector2f newvel);
	private:
	float time;
	bool exploded;
	sf::Vector2f size_orig;
	sf::Clock explosion;
	Light* light;
};

class Player: public GameObject{
	public:
	void updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin) override;
	void redraw(sf::RenderTexture &frame, LightingManager* lightingManager) override;
	Player(std::string texture, sf::Vector2f position, sf::Vector2f size, GameWorld* gw);
	~Player();
	void setFlashlight(bool state);
	void collision(GameObject* fellow, sf::Vector2f vel);
	private:

	float frictionRoll;
	float frictionAir;
	float frictionEngine;
	float frictionBrake;
	float frictionSlide;
	float accelEngine;
	bool headlight;
	sf::Clock headlight_;
	sf::Clock fire;
	Light* headlight_1;
	Light* headlight_2;
	Light* light;
	Obstruction* shadow;
};
#endif
