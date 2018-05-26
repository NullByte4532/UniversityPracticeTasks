#include "engine.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>

void GameWorld::updatePhysics(PlayerInput &pin){
	int i;

	sf::Time time = this->clock.getElapsedTime();
	this->testCollisions();
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
			if(this->hybernated) this->objs[i]->resetClocks();
			this->objs[i]->updatePhysics(time, this, pin);

		}
	}
	if(this->hybernated) this->hybernated=0;


}
//#define LIGHT_DEBUG
#define LIGHT_DOWNSCALING 1.f
#define SHADOW_BLUR 0.005f
void GameWorld::redraw(){
	int i;
	this->frame.draw(this->static_background);
	this->frame.display();
	this->lightmap.clear(sf::Color(30,30,20));
	this->lightmap.display();
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
			this->objs[i]->redraw(this->frame, this->lightingManager);

		}
	}
	this->frame.display();
	this->lightingManager->renderLight(this->lightmap);
	this->lightmap.display();
	sf::Sprite l(this->lightmap.getTexture());
	l.setScale(LIGHT_DOWNSCALING, LIGHT_DOWNSCALING);
	this->blend.setUniform("texture", frame.getTexture()) ;
	#ifndef LIGHT_DEBUG
	//sf::Sprite f(this->frame.getTexture());
	frame.draw(l, &(this->blend));
	#else
	//sf::Sprite f(this->lightmap.getTexture());
	frame.draw(l);
	#endif
	frame.display();


}
void GameWorld::testCollisions(){
	int i, j;
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
				for(j=i+1; j<MAX_OBJECTS; j++){
					if(this->objs[j]){
						if(vm::length(this->objs[j]->getGlobal(sf::Vector2f(0,0))-this->objs[i]->getGlobal(sf::Vector2f(0,0)))<= (vm::length(this->objs[j]->getSize())+vm::length(this->objs[i]->getSize()))/4){
							sf::Vector2f tmpvel=this->objs[j]->getVelocity();
							this->objs[j]->collision(this->objs[i], this->objs[i]->getVelocity());
							this->objs[i]->collision(this->objs[j], tmpvel);
						}

					}
				}

		}
	}


}

void GameWorld::redraw_static(){
	int i;
	this->static_background_.draw(this->background);
	this->static_background_.display();
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
			this->objs[i]->redraw_static(this->static_background_, this->lightingManager);

		}
	}
	this->static_background_.display();
	this->static_background.setTexture(this->static_background_.getTexture());
}
void GameWorld::addObject(GameObject* obj){
	int i;
	for(i=0; i<MAX_OBJECTS; i++){
		if(!this->objs[i]){
			this->objs[i]=obj;
			break;
		}

	}


}
void GameWorld::delObject(GameObject* obj){
	int i;
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]==obj){
			delete this->objs[i];
			this->objs[i]=NULL;
			break;
		}

	}


}
GameWorld::GameWorld(sf::VideoMode &vm, std::string background, GameLevel* level): objs{NULL}{
	this->frame.create(vm.width,vm.height);
	std::cout<<vm.width<<std::endl;
	this->lightmap.create(vm.width/LIGHT_DOWNSCALING,vm.height/LIGHT_DOWNSCALING);
	this->static_background_.create(vm.width,vm.height);
	this->blend.loadFromFile("data/shaders/blend.glsl", sf::Shader::Fragment);
	this->blend.setUniform("lightmap", sf::Shader::CurrentTexture) ;
	this->texture.loadFromFile(background);
	this->texture.setRepeated(true);
	this->background.setTexture(this->texture);
	this->background.setTextureRect(sf::IntRect(0,0,vm.width,vm.height));
	this->lightingManager=new LightingManager(vm);
	this->level=level;
}
GameWorld::~GameWorld(){
	int i;
	std::cout<<"[GameWorld Destructor] "<<"I've been called!!"<<std::endl;
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
			delete this->objs[i];
			this->objs[i]=NULL;
		}

	}
	delete this->lightingManager;
}

ConsoleScreen::ConsoleScreen(sf::VideoMode &vm): objs{NULL}{
	this->frame.create(vm.width,vm.height);
	this->static_background_.create(vm.width,vm.height);
	this->scanline.loadFromFile("data/shaders/scanline.glsl", sf::Shader::Fragment);
	this->scanline.setUniform("texture", sf::Shader::CurrentTexture);
	this->glitch.loadFromFile("data/shaders/glitch.glsl", sf::Shader::Fragment);
	this->glitch.setUniform("img", sf::Shader::CurrentTexture) ;
	this->glitch.setUniform("center", 0.5f) ;
	this->glitch.setUniform("offsets", sf::Vector3f(0.01f, 0.005f, -0.01f)) ;
	this->glitch.setUniform("add", sf::Vector3f(0.f,0.f,0.f)) ;
	this->to=0.2;
	this->k=0.1;
}
ConsoleScreen::~ConsoleScreen(){
	int i;
	std::cout<<"[ConsoleScreen Destructor] "<<"I've been called!!"<<std::endl;
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
			delete this->objs[i];
			this->objs[i]=NULL;
		}
	}
}
void ConsoleScreen::updatePhysics(PlayerInput &pin){
	int i;
	if(this->hybernated){
		this->clock.restart();
		this->hybernated=0;
	}
	sf::Time time = this->clock.getElapsedTime();
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
			this->objs[i]->updatePhysics(time, this, pin);

		}
	}



}
void ConsoleScreen::hybernate(){
	this->hybernated=1;
}
void GameWorld::hybernate(){
	this->hybernated=1;
}
void ConsoleScreen::redraw(){
	int i;
	this->frame.draw(this->static_background);
	this->frame.display();
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
			this->objs[i]->redraw(this->frame);

		}
	}
	this->frame.display();

	this->tmp.setTexture(frame.getTexture());
	this->frame.draw(this->tmp, &(this->scanline));
	this->frame.display();
	this->tmp.setTexture(this->frame.getTexture());
	float t=this->clock.getElapsedTime().asSeconds();
	if(t>=0.1)this->k=0;
	if(t>=this->to){
		this->glitch.setUniform("center", ((float) std::rand() / (RAND_MAX)));
		this->k=0.1;
		this->to=((float) std::rand() / (RAND_MAX))*3;
		this->clock.restart();

	}
	this->glitch.setUniform("offsets", sf::Vector3f(0.01f, 0.005f, -0.01f)*(1.5f+((float) std::rand() / (RAND_MAX))/3)*this->k);
	frame.draw(this->tmp, &glitch);
	frame.display();

}
void ConsoleScreen::redraw_static(){
	int i;
	this->static_background_.clear(sf::Color(25,25,30));
	this->static_background_.display();
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]){
			this->objs[i]->redraw_static(this->static_background_);

		}
	}
	this->static_background_.display();
	this->static_background.setTexture(this->static_background_.getTexture());
}
void ConsoleScreen::addObject(ConsoleObject* obj){
	int i;
	for(i=0; i<MAX_OBJECTS; i++){
		if(!this->objs[i]){
			this->objs[i]=obj;
			break;
		}

	}


}
void ConsoleScreen::delObject(ConsoleObject* obj){
	int i;
	for(i=0; i<MAX_OBJECTS; i++){
		if(this->objs[i]==obj){
			delete this->objs[i];
			this->objs[i]=NULL;
			break;
		}

	}


}
ConsoleObject::ConsoleObject(sf::Vector2f position, sf::Vector2f size, ConsoleScreen* cs){
	this->position=position;
	this->size=size;
	this->forward=sf::Vector2f(0,1);
	this->cs=cs;
}
ConsoleObject::~ConsoleObject(){};
void ConsoleObject::updatePhysics(sf::Time &timeAbs, ConsoleScreen* cs, PlayerInput &pin){}
void ConsoleObject::redraw(sf::RenderTexture &frame){}
void ConsoleObject::redraw_static(sf::RenderTexture &frame){}

GameObject::GameObject(std::string texture, sf::Vector2f position, sf::Vector2f size, GameWorld* gw){
	this->texture.loadFromFile(texture);
	this->sprite.setTexture(this->texture);
	sf::FloatRect box = this->sprite.getLocalBounds();
	this->sprite.setScale(size.x / box.width, size.y / box.height);
	this->sprite.setPosition(position);
	this->sprite.setOrigin(this->texture.getSize().x/2.f,this->texture.getSize().y/2.f);
	this->position=position;
	this->size=size;
	this->forward=sf::Vector2f(0,1);
	this->velocity=sf::Vector2f(0,0);
	this->acceleration=sf::Vector2f(0,0);
	this->gw=gw;
	this->type=0;
}
GameObject::~GameObject(){};
void GameObject::updatePhysics(sf::Time &timeAbs, GameWorld* gw, PlayerInput &pin){
	sf::Time time = this->clock.getElapsedTime();
	this->velocity+=this->acceleration*time.asSeconds();
	this->position+=this->velocity*time.asSeconds();
	this->acceleration.x=0;
	this->acceleration.y=0;
	this->clock.restart();


}
void GameObject::redraw(sf::RenderTexture &frame, LightingManager* lightingManager){
	sf::FloatRect box = this->sprite.getLocalBounds();
	this->sprite.setScale(this->size.x / box.width, this->size.y / box.height);
	this->sprite.setPosition(this->position);
	frame.draw(this->sprite);


}
void GameObject::redraw_static(sf::RenderTexture &frame, LightingManager* lightingManager){}
void GameObject::resetClocks(){
	this->clock.restart();
}

void GameObject::rotate(float angle){
	this->sprite.rotate(180/M_PI*angle);
	vm::rotate(this->forward, angle);
	this->forward=vm::normalize(this->forward);

}
sf::Vector2f GameObject::getGlobal(sf::Vector2f position){
	return this->position + position.y*this->forward + position.x*vm::normal(this->forward);
}
Light::Light(sf::Vector2f pos, sf::Vector2f dir, sf::Vector3f color, float intensity, float min_cos, float decay, float shadowdst){
	this->pos=pos;
	this->dir=dir;
	this->color=color;
	this->intensity=intensity;
	this->min_cos=min_cos;
	this->decay=decay;
	this->shadowdst=shadowdst;
}
Obstruction::Obstruction(sf::Vector2f pos, sf::Vector3f opacity, float radius){
	this->pos=pos;
	this->opacity=opacity;
	this->radius=radius;
}
sf::Vector2f Obstruction::getPoint_1(sf::Vector2f lightPos){
	return this->pos+vm::normalize(vm::normal(this->pos-lightPos))*this->radius;
}
sf::Vector2f Obstruction::getPoint_2(sf::Vector2f lightPos){
	return this->pos-vm::normalize(vm::normal(this->pos-lightPos))*this->radius;
}
LightingManager::LightingManager(sf::VideoMode &vm): lights{NULL}, obstructions{NULL}{
	this->numLights=0;
	this->numObstructions=0;
	this->shader.loadFromFile("data/shaders/DirectionalLight.glsl", sf::Shader::Fragment);
	this->shader.setUniform("texture", sf::Shader::CurrentTexture) ;
	this->blur.loadFromFile("data/shaders/blur.glsl", sf::Shader::Fragment);
	this->blur.setUniform("texture", sf::Shader::CurrentTexture) ;
	this->blur.setUniform("blur_radius", SHADOW_BLUR) ;
	this->shadowMask.create(vm.width/LIGHT_DOWNSCALING, vm.height/LIGHT_DOWNSCALING);
}

void LightingManager::addLight(Light* light){
	int i;
	for(i=0; i<MAX_LIGHTS; i++){
		if(!this->lights[i]){
			this->lights[i]=light;
			this->numLights++;
			break;
		}

	}


}
void LightingManager::delLight(Light* light){
	int i;
	for(i=0; i<MAX_LIGHTS; i++){
		if(this->lights[i]==light){
			this->lights[i]=NULL;
			this->numLights--;
			break;
		}

	}


}

void LightingManager::addObstruction(Obstruction* obstruction){
	int i;
	for(i=0; i<MAX_SHADOWS; i++){
		if(!this->obstructions[i]){
			this->obstructions[i]=obstruction;
			this->numObstructions++;
			break;
		}

	}


}
void LightingManager::delObstruction(Obstruction* obstruction){
	int i;
	for(i=0; i<MAX_SHADOWS; i++){
		if(this->obstructions[i]==obstruction){
			this->obstructions[i]=NULL;
			this->numObstructions--;
			break;
		}

	}


}
void LightingManager::renderLight(sf::RenderTexture &lightmap){
	sf::Sprite l;
	int k=0;
	int height=lightmap.getSize().y;
	
	for(int i=0; i<MAX_LIGHTS; i++){
		if(lights[i]){
			k++;
			Light* light = lights[i];
			shadowMask.clear(sf::Color(255,255,255));
			this->drawShadow(shadowMask, light);
			shadowMask.display();
			lightmap.display();
			
			
			shader.setUniform("light", sf::Vector3f(light->pos.x/LIGHT_DOWNSCALING,height-light->pos.y/LIGHT_DOWNSCALING,light->intensity)) ;
			shader.setUniform("dir", sf::Vector3f(light->dir.x, -light->dir.y, light->min_cos)) ;
			shader.setUniform("lcolor", light->color) ;
			shader.setUniform("decay", light->decay);
			l.setTexture(shadowMask.getTexture());
			shadowMask.draw(l, &(this->blur));
			shadowMask.display();
			this->shader.setUniform("shadow", shadowMask.getTexture()) ;
			l.setTexture(lightmap.getTexture());
			lightmap.draw(l, &(this->shader));
			lightmap.display();
			
		}
		if(k>=this->numLights) break;
	}
	
}
sf::Vector2f getIntersect(sf::Vector2f point, sf::Vector2f dir, sf::Vector2u dim){
	sf::Vector2f n, p1, p2;
	float c;
	n=vm::normal(dir);
	c=vm::scalarProduct(n, point);
	if(dir.x>0)
		p1=vm::intersect(sf::Vector3f(n.x, n.y, c),sf::Vector3f(1,0,dim.x));
	else
		p1=vm::intersect(sf::Vector3f(n.x, n.y, c),sf::Vector3f(1,0,0));
	if(dir.y>0)
		p2=vm::intersect(sf::Vector3f(n.x, n.y, c),sf::Vector3f(0,1,dim.y));
	else
		p2=vm::intersect(sf::Vector3f(n.x, n.y, c),sf::Vector3f(0,1,0));
	if((p1.x-point.x)/dir.x>(p2.x-point.x)/dir.x) return p1;
	else return p2;

}

void LightingManager::drawShadow(sf::RenderTexture &shadowmap, Light* light){
	int i, j, n ,k=0;
	sf::Vector2f p_1, p_2, v1, v2, p_c1, p_c2, p_c3, v3;
	bool c1, c2, c3, c4, w1, w2;
	sf::Vector2u s=shadowmap.getSize();
	for(i=0; i<MAX_SHADOWS; i++){
		if(obstructions[i]){
			k++;
			c1=0;  c2=0; c3=0; c4=0;
			n=4;
			sf::ConvexShape shadow;
			p_1=obstructions[i]->getPoint_1(light->pos);
			p_2=obstructions[i]->getPoint_2(light->pos);
			if(vm::length(p_1-light->pos)<light->shadowdst || vm::length(p_2-light->pos)<light->shadowdst){
				v1=vm::normalize(p_1-light->pos);
				v2=vm::normalize(p_2-light->pos);
				p_c1=getIntersect(p_1, v1, s*(unsigned int)LIGHT_DOWNSCALING);
				p_c2=getIntersect(p_2, v2, s*(unsigned int)LIGHT_DOWNSCALING);
				if(vm::between(sf::Vector2f(0,0)-light->pos, v1, v2)) {n++; c1=1;}
				if(vm::between(sf::Vector2f(s.x*LIGHT_DOWNSCALING,0)-light->pos, v1, v2)) {n++; c2=1;}
				if(vm::between(sf::Vector2f(0,s.y*LIGHT_DOWNSCALING)-light->pos, v1, v2)) {n++; c3=1;}
				if(vm::between(sf::Vector2f(s.x,s.y)*LIGHT_DOWNSCALING-light->pos, v1, v2)) {n++; c4=1;}
				shadow.setPointCount(n);
				j=0;
				shadow.setPoint(j++, p_1/LIGHT_DOWNSCALING);
				shadow.setPoint(j++, p_c1/LIGHT_DOWNSCALING);
				w1=(v1.x<0);
				w2=(v1.y<0);
				if(c1 && w1 && w2){
					shadow.setPoint(j++, sf::Vector2f(0,0));
					if(c2) shadow.setPoint(j++, sf::Vector2f(s.x,0));
					if(c3) shadow.setPoint(j++, sf::Vector2f(0,s.y));
					if(c4) shadow.setPoint(j++, sf::Vector2f(s.x,s.y));

				}
				else if(c2 && !w1 && w2){
					shadow.setPoint(j++, sf::Vector2f(s.x,0));
					if(c1) shadow.setPoint(j++, sf::Vector2f(0,0));
					if(c4) shadow.setPoint(j++, sf::Vector2f(s.x,s.y));
					if(c3) shadow.setPoint(j++, sf::Vector2f(0,s.y));

				}
				else if(c3 && w1 && !w2){
					shadow.setPoint(j++, sf::Vector2f(0, s.y));
					if(c1) shadow.setPoint(j++, sf::Vector2f(0,0));
					if(c4) shadow.setPoint(j++, sf::Vector2f(s.x,s.y));
					if(c2) shadow.setPoint(j++, sf::Vector2f(s.x,0));

				}
				else if(c4 && !w1 && !w2){
					shadow.setPoint(j++, sf::Vector2f(s.x, s.y));
					if(c3) shadow.setPoint(j++, sf::Vector2f(0,s.y));
					if(c2) shadow.setPoint(j++, sf::Vector2f(s.x,0));
					if(c1) shadow.setPoint(j++, sf::Vector2f(0,0));

				}
				shadow.setPoint(j++, p_c2/LIGHT_DOWNSCALING);
				shadow.setPoint(j++, p_2/LIGHT_DOWNSCALING);
				shadow.setFillColor(sf::Color(0, 0, 0));
				shadowmap.draw(shadow);
			}
		}
		if(k>=this->numObstructions) break;
	}
	

}





TransitionEffect::TransitionEffect(){
	this->active=0;
	this->transition.loadFromFile("data/shaders/transition.glsl", sf::Shader::Fragment);
	this->transition.setUniform("img1", sf::Shader::CurrentTexture) ;
	this->st=0;
}
void TransitionEffect::restart(){
	this->clock.restart();
	this->active=1;
	this->st=0;
}
bool TransitionEffect::isActive(){
	return this->active;
}
void TransitionEffect::draw(sf::RenderTexture &window, const sf::Texture *txt1, const sf::Texture *txt2){
	if(this->st<=1.3&&this->clock.getElapsedTime().asSeconds()>0.03){
		for(int i=0; i<16; i++) this->off[i]=vm::min(((float) std::rand() / (RAND_MAX))*this->st*2, 1.0f);
		float stm=vm::min(this->st, 1.0);
		this->transition.setUniform("coffsets", sf::Vector3f(1.f, stm, stm*stm)) ;
		this->st+=0.1;
		this->clock.restart();
	}else if(this->st>1.3){
		for(int i=0; i<16; i++) this->off[i]=1.0f;
		this->active=0;
	}
	this->transition.setUniform("img2", *txt2);
	this->transition.setUniformArray("loffsets", this->off, 16);
	this->s.setTexture(*txt1);
	window.draw(this->s, &(this->transition));
	window.display();
}


void GameLevel::updatePhysics(PlayerInput &pin){}
void GameLevel::redraw(){}
void GameLevel::redraw_static(){}
void GameLevel::hybernate(){}
const sf::Texture& GameLevel::getTexture(){}
