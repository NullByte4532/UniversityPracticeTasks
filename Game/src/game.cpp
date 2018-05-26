#include "game.hpp"
#include <iostream>
game_test::game_test(sf::VideoMode &vm):
	cs(vm, this),
	ps(vm, this)
{
	frame.create(vm.width,vm.height);
	cs.redraw_static();
	ps.redraw_static();
	this->vm=vm;
	this->setState(GS_MENU);
}
game_test::~game_test(){ delete this->level;}

void game_test::loadLevel(LevelId lvl){
	switch(lvl){
		case LV_1:
			if(this->level) delete this->level;
			this->level = new lv_test(this->vm, this, 5.0, LV_2);
			break;
		case LV_2:
			if(this->level) delete this->level;
			this->level = new lv_test(this->vm, this, 2.0, LV_3);
			break;
		case LV_3:
			if(this->level) delete this->level;
			this->level = new lv_test(this->vm, this, 1.0, LV_NULL);
			break;
		default:
			if(this->level) delete this->level;
			this->level = NULL;
			break;
	}


}
void game_test::setState(GameState st){
	this->state=st;
	switch(state){
		case GS_WORLD_TO_MENU:
			std::cout<<"Setting game state to GS_WORLD_TO_MENU \n";
			transition.restart();
			if(level)level->hybernate();
			break;
		case GS_MENU_TO_WORLD:
			std::cout<<"Setting game state to GS_MENU_TO_WORLD \n";
			transition.restart();
			cs.hybernate();
			break;
		case GS_PAUSE_TO_WORLD:
			std::cout<<"Setting game state to GS_PAUSE_TO_WORLD \n";
			transition.restart();
			ps.hybernate();
			break;
		case GS_PAUSE_TO_MENU:
			std::cout<<"Setting game state to GS_PAUSE_TO_MENU \n";
			transition.restart();
			ps.hybernate();
			break;
		case GS_WORLD_TO_PAUSE:
			std::cout<<"Setting game state to GS_WORLD_TO_PAUSE\n";
			transition.restart();
			if(level)level->hybernate();
			break;
		case GS_WORLD:
			std::cout<<"Setting game state to GS_WORLD\n";
			ps.hybernate();
			cs.hybernate();
			break;
		case GS_PAUSE:
			std::cout<<"Setting game state to GS_PAUSE\n";
			if(level)level->hybernate();
			cs.hybernate();
			break;
		case GS_MENU:
			if(level)level->hybernate();
			ps.hybernate();
			std::cout<<"Setting game state to GS_MENU\n";
			break;
	}
}
bool game_test::tick(PlayerInput &pin){
	switch(state){
		case GS_WORLD:
			level->updatePhysics(pin);
			level->redraw();
			s.setTexture(level->getTexture());
			if(pin.menu){
				this->setState(GS_WORLD_TO_PAUSE);
				
			}
			break;
		case GS_WORLD_TO_MENU:

			transition.draw(frame, &(level->getTexture()), &cs.frame.getTexture());
			s.setTexture(frame.getTexture());
			if(!transition.isActive()) this->setState(GS_MENU);
			break;
		case GS_MENU:
			cs.updatePhysics(pin);
			cs.redraw();
			s.setTexture(cs.frame.getTexture());
			break;
		case GS_MENU_TO_WORLD:

			transition.draw(frame, &cs.frame.getTexture(), &(level->getTexture()));
			s.setTexture(frame.getTexture());
			if(!transition.isActive()) this->setState(GS_WORLD);
			break;
		case GS_PAUSE:
			ps.updatePhysics(pin);
			ps.redraw();
			s.setTexture(ps.frame.getTexture());
			if(pin.menu){
				this->setState(GS_PAUSE_TO_WORLD);
				
			}
			break;
		case GS_PAUSE_TO_WORLD:

			transition.draw(frame, &ps.frame.getTexture(), &(level->getTexture()));
			s.setTexture(frame.getTexture());
			if(!transition.isActive()) this->setState(GS_WORLD);
			break;
		case GS_WORLD_TO_PAUSE:

			transition.draw(frame, &(level->getTexture()), &ps.frame.getTexture());
			s.setTexture(frame.getTexture());
			if(!transition.isActive()) this->setState(GS_PAUSE);
			break;
		case GS_PAUSE_TO_MENU:
			transition.draw(frame, &ps.frame.getTexture(), &cs.frame.getTexture());
			s.setTexture(frame.getTexture());
			if(!transition.isActive()){
				this->loadLevel(LV_NULL);
				this->setState(GS_MENU);
			}
			break;
		case GS_EXIT:
			return 1;
		
	}
	return 0;
}
sf::Sprite& game_test::getFrame(){
	return s;  
}
