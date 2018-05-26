#include "objects.hpp"
GlowingBush::GlowingBush(std::string texture, sf::Vector3f color, sf::Vector2f position, sf::Vector2f size, GameWorld* gw): GameObject(texture, position, size, gw){
	this->shadow=new Obstruction(this->position, sf::Vector3f(0,0,0), vm::length(size)/8);
	gw->lightingManager->addObstruction(this->shadow);
	this->sprite.setColor(sf::Color(color.x, color.y, color.z));
	this->light=new Light(this->position, this->forward, color, 0.02, -1.0, 0.9, 500);
	gw->lightingManager->addLight(this->light);
	this->energy=1;
	off=0;
}
GlowingBush::~GlowingBush(){
	this->gw->lightingManager->delObstruction(this->shadow);
	delete this->shadow;
	this->gw->lightingManager->delLight(this->light);
	delete this->light;
}
void GlowingBush::redraw(sf::RenderTexture &frame, LightingManager* lightingManager){
	if(flickerClock.getElapsedTime().asSeconds()>=0.1){
		this->light->intensity=(0.019+0.001*((double) std::rand() / (RAND_MAX)))*energy;
		flickerClock.restart();
	}


}
void GlowingBush::redraw_static(sf::RenderTexture &frame, LightingManager* lightingManager){
	this->shadow->pos=this->position;
	GameObject::redraw(frame, lightingManager);
}
void GlowingBush::updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin){}
void GlowingBush::collision(GameObject* fellow, sf::Vector2f vel){
	if(energy&&fellow->getType()==1&&mineClock.getElapsedTime().asSeconds()>0.5){
		energy-=0.1;
		if(energy<0) energy=0;
		mineClock.restart();
	}
	if(energy<=0&&!off){
		gw->level->levelLogic(1, 1.0);
		off=1;
	}
}

Spider::Spider(sf::Vector2f position, sf::Vector2f size, GameWorld* gw): GameObject("data/textures/spider/spider_1.png", position, size, gw){
	//this->shadow=new Obstruction(this->position, sf::Vector3f(0,0,0), vm::length(size)/8);
	//gw->lightingManager->addObstruction(this->shadow);
	this->animation[0].loadFromFile("data/textures/spider/spider_1.png");
	this->animation[1].loadFromFile("data/textures/spider/spider_2.png");
	this->animation[2].loadFromFile("data/textures/spider/spider_3.png");
	this->animation[3].loadFromFile("data/textures/spider/spider_4.png");
	this->animation[4].loadFromFile("data/textures/spider/spider_5.png");
	this->animation[5].loadFromFile("data/textures/spider/spider_6.png");
	anim_cur_frame=0;
	this->forward=sf::Vector2f(0,-1);
	this->state=1;
	this->acceleration=sf::Vector2f(0,0);
	this->type=2;
	this->dead=0;
}
Spider::~Spider(){
	//this->gw->lightingManager->delObstruction(this->shadow);
	//delete this->shadow;

}
void Spider::die(){dead=1;}
void Spider::redraw(sf::RenderTexture &frame, LightingManager* lightingManager){
	if(state==1){
	if(animationClock.getElapsedTime().asSeconds()>=0.1){
		anim_cur_frame=(anim_cur_frame+1)%6;
		this->sprite.setTexture(this->animation[anim_cur_frame]);
		animationClock.restart();
	}
	}
	//this->shadow->pos=this->position;
	GameObject::redraw(frame, lightingManager);
	
}
void Spider::redraw_static(sf::RenderTexture &frame, LightingManager* lightingManager){
	
	
}
void Spider::updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin){
	if(dead){
		gw->delObject(this);
		return;
	}
	if(pathClock.getElapsedTime().asSeconds()>=0.1){
		if(this->target){
			this->setDirection(vm::normalize(target->getGlobal(sf::Vector2f(0,0))-this->position));
		}
		pathClock.restart();
	}
	this->acceleration+=300.f*forward;
	this->acceleration-=10.f*velocity;
	this->velocity+=interframeClock.getElapsedTime().asSeconds()*acceleration;
	this->position+=interframeClock.getElapsedTime().asSeconds()*velocity;
	this->acceleration=sf::Vector2f(0,0);
	this->interframeClock.restart();

}
void Spider::setDirection(sf::Vector2f dir){
	this->sprite.rotate(180/M_PI*vm::angleBetween(this->forward, dir));
	this->forward=dir;
}
void Spider::resetClocks(){
	this->interframeClock.restart();

}
void Spider::collision(GameObject* fellow, sf::Vector2f vel){
	this->acceleration+=10.0f*((vm::length(this->size)+vm::length(fellow->getSize()))*vm::normalize(this->position-fellow->getGlobal(sf::Vector2f(0,0))))-(this->position-fellow->getGlobal(sf::Vector2f(0,0)));
}
void Player::collision(GameObject* fellow, sf::Vector2f vel){
	this->acceleration+=10.0f*((vm::length(this->size)+vm::length(fellow->getSize()))*vm::normalize(this->position-fellow->getGlobal(sf::Vector2f(0,0))))-(this->position-fellow->getGlobal(sf::Vector2f(0,0)));
	if (fellow->getType()==2){
		Spider* sp=(Spider*)fellow;
		if(sp->attackClock.getElapsedTime().asSeconds()>0.5){
			this->gw->level->hurt(7.0);
			sp->attackClock.restart();
		}
	}
}

Player::Player(std::string texture, sf::Vector2f position, sf::Vector2f size, GameWorld* gw): GameObject(texture, position, size, gw){
	this->frictionRoll=8;
	this->frictionAir=0.03;
	this->frictionEngine=8;
	this->frictionSlide=300;
	this->frictionBrake=300;
	this->accelEngine=400;
	this->headlight=false;
	this->headlight_1=new Light(this->getGlobal(sf::Vector2f(5, 20)), this->forward, sf::Vector3f(250.f, 250.f, 50.f), 0.08, 0.95, 1.0, 1100);
	this->headlight_2=new Light(this->getGlobal(sf::Vector2f(-5, 20)), this->forward, sf::Vector3f(250.f, 250.f, 50.f), 0.08, 0.95, 1.0, 1100);
	this->shadow=new Obstruction(this->position, sf::Vector3f(0,0,0), 20);
	gw->lightingManager->addObstruction(this->shadow);
	this->type=1;
	
}
Player::~Player(){
	this->gw->lightingManager->delLight(this->headlight_1);
	this->gw->lightingManager->delLight(this->headlight_2);
	this->gw->lightingManager->delObstruction(this->shadow);
	delete this->headlight_1;
	delete this->headlight_2;
	delete this->shadow;
}
void Player::setFlashlight(bool state){
	this->headlight=state;
	if(this->headlight){
		this->gw->lightingManager->addLight(this->headlight_1);
		this->gw->lightingManager->addLight(this->headlight_2);
	}else{
		this->gw->lightingManager->delLight(this->headlight_1);
		this->gw->lightingManager->delLight(this->headlight_2);
	}


}
void Player::updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin){

	sf::Time time = this->clock.getElapsedTime();
	if (vm::length(this->velocity)<0.5) this->velocity=this->velocity*0.f;
	if (vm::length(this->velocity)>0.5){
	this->acceleration+=-1*this->frictionRoll*vm::normalize(this->velocity);
	this->acceleration+=-1*this->frictionAir*this->velocity*vm::length(this->velocity);
	if (vm::length(this->velocity)>1) this->acceleration+=-1*this->frictionSlide*vm::sign(vm::scalarProduct(this->velocity, vm::normal(this->forward)))*vm::normal(this->forward);
	}
	if (pin.forward)
	{
		this->acceleration+=this->accelEngine*this->forward;
	}else if(pin.back){

		this->acceleration+=(-1)*this->accelEngine*this->forward;

	}else{
		if (vm::length(this->velocity)>1) this->acceleration+=-1*this->frictionEngine*vm::sign(vm::scalarProduct(this->velocity, this->forward))*this->forward;

	}
	if (pin.stop)
	{
   		if (vm::length(this->velocity)>1) this->acceleration+=-1*this->frictionBrake*vm::sign(vm::scalarProduct(this->velocity, this->forward))*this->forward;
	}
	if (pin.left)
	{
   		this->rotate(-1.5*time.asSeconds());
	}
	if (pin.right)
	{
   		this->rotate(1.5*time.asSeconds());
	}
	if (pin.flashlight&&(this->headlight_.getElapsedTime().asSeconds()>0.2))
	{
   		this->headlight=!this->headlight;
		if(this->headlight){
			gw->lightingManager->addLight(this->headlight_1);
			gw->lightingManager->addLight(this->headlight_2);
		}else{
			gw->lightingManager->delLight(this->headlight_1);
			gw->lightingManager->delLight(this->headlight_2);
		}
		this->headlight_.restart();
	} 
	if (pin.fire&&(this->fire.getElapsedTime().asSeconds()>0.5))
	{
   		gw->addObject(new Fireball("data/textures/fireball.png", this->getGlobal(sf::Vector2f(0, 40)), sf::Vector2f(10, 10), this->forward*400.f, 0.7, gw));
		this->fire.restart();
	} 

	
	
	GameObject::updatePhysics(timeAbs, gw, pin);

}
void Player::redraw(sf::RenderTexture &frame, LightingManager* lightingManager){
	if(this->headlight){
		this->headlight_1->pos=this->getGlobal(sf::Vector2f(5, 20));
		this->headlight_2->pos=this->getGlobal(sf::Vector2f(-5, 20));
		this->headlight_1->dir=this->forward;
		this->headlight_2->dir=this->forward;
	}
	this->shadow->pos=this->position;
	GameObject::redraw(frame, lightingManager);

}



Fireball::Fireball(std::string texture, sf::Vector2f position, sf::Vector2f size, sf::Vector2f velocity, float time, GameWorld* gw): GameObject(texture, position, size, gw){
	this->velocity=velocity;
	this->time=time;
	this->exploded=false;
	this->size_orig=size;
	this->light=new Light(this->position, this->forward, sf::Vector3f(131.f, 221.f, 255.f), 0.005, -1.0, 1.0, 300);
	gw->lightingManager->addLight(this->light);

}
Fireball::~Fireball(){
	gw->lightingManager->delLight(this->light);
	delete this->light;
}
void Fireball::updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin){
	if(!this->exploded && this->time<=this->explosion.getElapsedTime().asSeconds()){
		this->exploded=true;
		this->explosion.restart();
	}
	if(this->exploded && this->explosion.getElapsedTime().asSeconds() >= 0.2){
		gw->delObject(this);
		return;
	}
	GameObject::updatePhysics(timeAbs, gw, pin);
}
void Fireball::redraw(sf::RenderTexture &frame, LightingManager* lightingManager){
	this->light->pos=this->position;
	if(this->exploded){	
		float t,k;
		t=this->explosion.getElapsedTime().asSeconds()*1.0;
		if(t<=0.1) k=exp(7*t);
		else k=(2-pow(10*t, 0.91))*2;
		this->light->intensity=0.01*k;
		this->size=this->size_orig*k;
	}
	GameObject::redraw(frame, lightingManager);
}
void Fireball::collision(GameObject* fellow, sf::Vector2f newvel){
		if(!exploded){
			this->exploded=true;
			this->explosion.restart();
			this->velocity=sf::Vector2f(0,0);
		}
		if(fellow->getType()==2){
			Spider* sp=(Spider*)fellow;
			sp->die();
		}

}
