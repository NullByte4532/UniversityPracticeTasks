#include "levels.hpp"
#include <unistd.h>
lv_test::lv_test(sf::VideoMode &vm, game_test* game, float dif, LevelId next): 
	gw(vm, "data/textures/grass2.jpg", this)
{
	this->next=next;
	this->dif=dif;
	this->game=game;
	this->glitch.loadFromFile("data/shaders/glitch.glsl", sf::Shader::Fragment);
	this->glitch.setUniform("img", sf::Shader::CurrentTexture) ;
	this->glitch.setUniform("center", 0.5f) ;
	this->glitch.setUniform("offsets", sf::Vector3f(0.00f, 0.00f, 0.00f));
	this->frame.create(vm.width,vm.height);
	this->crystals_remaining=5;
	player=new Player("data/textures/robot.png", sf::Vector2f(vm.width/2,vm.height/2), sf::Vector2f(50,50), &gw);
	gw.addObject(new GlowingBush("data/textures/crystal-cluster128.png", sf::Vector3f(240, 100, 70), sf::Vector2f(vm.width/3,vm.height/3), sf::Vector2f(50,50), &gw));
	gw.addObject(new GlowingBush("data/textures/crystal-cluster128.png", sf::Vector3f(240, 100, 70), sf::Vector2f(vm.width/3*2,vm.height/5*4), sf::Vector2f(50,50), &gw));
	gw.addObject(new GlowingBush("data/textures/crystal-cluster128.png", sf::Vector3f(240, 100, 70), sf::Vector2f(vm.width/2,vm.height/8*3), sf::Vector2f(50,50), &gw));
	gw.addObject(new GlowingBush("data/textures/crystal-cluster128.png", sf::Vector3f(240, 100, 70), sf::Vector2f(vm.width/16*5,vm.height/7*6), sf::Vector2f(50,50), &gw));
	gw.addObject(new GlowingBush("data/textures/crystal-cluster128.png", sf::Vector3f(240, 100, 70), sf::Vector2f(vm.width/8,vm.height/5*2), sf::Vector2f(50,50), &gw));
	gw.addObject(player);
	this->hp=100.0;

	gw.redraw_static();

}
void lv_test::levelLogic(int id, float param){
	if(id==1){
		this->crystals_remaining-=(int)param;
		if(crystals_remaining==0){
			game->loadLevel(next);
			if(next==LV_NULL) game->setState(GS_MENU);
		}
	}
}
void lv_test::updatePhysics(PlayerInput &pin){
	if(spawnTimer.getElapsedTime().asSeconds()>=dif){
		Spider* sp= new Spider(sf::Vector2f(game->vm.width*0.9,0), sf::Vector2f(40,40), &gw);
		sp->setTarget(player);
		gw.addObject(sp);
		spawnTimer.restart();
	}
	if(hp>0)gw.updatePhysics(pin);
}
void lv_test::redraw(){
	if(hp>0)gw.redraw();
	s.setTexture(gw.frame.getTexture());
	float t=this->clock.getElapsedTime().asSeconds();


	this->glitch.setUniform("offsets", sf::Vector3f(0.01f, 0.005f, -0.01f)*(1.5f+((float) std::rand() / (RAND_MAX))/3)*(1-hp/100));
	this->glitch.setUniform("add", sf::Vector3f(0.1f, 0.f, 0.f)*(1-hp/100));
	frame.draw(s, &glitch);
	frame.display();
}
void lv_test::redraw_static(){
	if(hp>0)gw.redraw_static();
}
void lv_test::hybernate(){
	gw.hybernate();
}
void lv_test::hurt(float damage){
	hp-=damage;
	if(hp<0)hp=0;
}
const sf::Texture& lv_test::getTexture(){
	return this->frame.getTexture();
}
