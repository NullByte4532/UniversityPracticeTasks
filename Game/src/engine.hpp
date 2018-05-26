#ifndef engine_h
#define engine_h
#include <SFML/Graphics.hpp>
#include <math.h>
#include "VectorMath.hpp"

class GameObject;
class GameWorld;
class ConsoleScreen;
class ConsoleObject;
class LightingManager;
class Light;
class Obstruction;
class Player;
class Fireball;
class Stump;
struct PlayerInput;
class GameLevel;
#define MAX_LIGHTS	50
#define MAX_OBJECTS	80
#define MAX_SHADOWS	50
struct PlayerInput{
	bool forward;
	bool back;
	bool right;
	bool left;
	bool stop;
	bool flashlight;
	bool fire;
	bool menu;
	bool select;
};
class GameObject{
	public:
		virtual void updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin);
		virtual void redraw(sf::RenderTexture &frame, LightingManager* lightingManager);
		virtual void redraw_static(sf::RenderTexture &frame, LightingManager* lightingManager);
		virtual void resetClocks();
		GameObject(std::string texture, sf::Vector2f position, sf::Vector2f size, GameWorld* gw);
		virtual ~GameObject();
		sf::Vector2f getGlobal(sf::Vector2f position);
		void rotate(float angle);
		sf::Vector2f getSize(){ return size;};
		sf::Vector2f getVelocity(){ return velocity;};
		virtual void collision(GameObject* fellow, sf::Vector2f newvel){};
		int getType(){ return type;};
	protected:
		sf::Texture texture;
		sf::Sprite sprite;
		sf::Clock clock;
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Vector2f forward;
		sf::Vector2f velocity;
		sf::Vector2f acceleration;
		int type;
		GameWorld* gw;
};
class GameWorld{
	public:
		void addObject(GameObject* obj);
		void delObject(GameObject* obj);
		virtual void updatePhysics(PlayerInput &pin);
		virtual void redraw();
		virtual void redraw_static();
		void testCollisions();
		void hybernate();
		GameWorld(sf::VideoMode &vm, std::string background, GameLevel* level);
		~GameWorld();
		LightingManager* lightingManager;
		sf::RenderTexture frame;
		GameLevel* level;
	protected:
		GameObject* objs[MAX_OBJECTS];
		sf::Clock clock;
		sf::RenderTexture lightmap;
		sf::RenderTexture static_background_;
		sf::Sprite static_background;
		sf::Shader blend;
		sf::Sprite background;
		sf::Texture texture;
		bool hybernated;
};
class Light{
	public:
	Light(sf::Vector2f pos, sf::Vector2f dir, sf::Vector3f color, float intensity, float min_cos, float decay, float shadowdst);
	sf::Vector2f pos;
	sf::Vector2f dir;
	sf::Vector3f color;
	float shadowdst;
	float intensity;
	float min_cos;
	float decay;
};
class Obstruction{
	public:
	Obstruction(sf::Vector2f pos, sf::Vector3f opacity, float radius);
	sf::Vector2f getPoint_1(sf::Vector2f lightPos);
	sf::Vector2f getPoint_2(sf::Vector2f lightPos);
	sf::Vector2f pos;
	sf::Vector3f opacity;
	float radius;
};
class LightingManager{
	public:
	LightingManager(sf::VideoMode &vm);
	void addLight(Light* light);
	void delLight(Light* light);
	void addObstruction(Obstruction* obstruction);
	void delObstruction(Obstruction* obstruction);
	void renderLight(sf::RenderTexture &lightmap);
	void drawShadow(sf::RenderTexture &shadowmap, Light* light);
	private:
	int numLights;
	int numObstructions;
	sf::Shader shader;
	sf::Shader blur;
	Light* lights[MAX_LIGHTS];
	Obstruction* obstructions[MAX_SHADOWS];
	sf::RenderTexture shadowMask;

};
class ConsoleObject{
	public:
		virtual void updatePhysics(sf::Time &timeAbs, ConsoleScreen* cs, PlayerInput &pin);
		virtual void redraw(sf::RenderTexture &frame);
		virtual void redraw_static(sf::RenderTexture &frame);
		ConsoleObject(sf::Vector2f position, sf::Vector2f size, ConsoleScreen* cs);
		virtual ~ConsoleObject();
	protected:
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Vector2f forward;
		ConsoleScreen* cs;
};
class ConsoleScreen{
	public:
		void addObject(ConsoleObject* obj);
		void delObject(ConsoleObject* obj);
		virtual void updatePhysics(PlayerInput &pin);
		virtual void redraw();
		virtual void redraw_static();
		void hybernate();
		ConsoleScreen(sf::VideoMode &vm);
		~ConsoleScreen();
		sf::RenderTexture frame;
	protected:
		ConsoleObject* objs[MAX_OBJECTS];
		sf::Clock clock;
		sf::RenderTexture static_background_;
		sf::Sprite static_background;
		sf::Shader scanline;
		sf::Shader glitch;
		sf::Sprite tmp;
		float to;
		float k;
		bool hybernated;

};





class TransitionEffect{
	public:
	TransitionEffect();
	void draw(sf::RenderTexture &window, const sf::Texture* txt1, const sf::Texture* txt2);
	bool isActive();
	void restart();
	protected:
	sf::Clock clock;
	bool active;
	sf::Shader transition;
	float st;
	float off[16];
	sf::Sprite s;
	

};
class GameLevel{
	public:
	virtual void updatePhysics(PlayerInput &pin);
	virtual void redraw();
	virtual void redraw_static();
	virtual void hybernate();
	virtual void hurt(float damage){};
	virtual void levelLogic(int id, float param){};
	virtual const sf::Texture& getTexture();

};

#endif
