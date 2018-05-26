#include "menu.hpp"
#include "game.hpp"
#include <iostream>
ConsoleText::ConsoleText(sf::Vector2f position, sf::Vector2f size, ConsoleScreen* cs, std::string str, int charsize, sf::Color color, std::string str_sel, sf::Color color_sel): ConsoleObject(position, size, cs){
	this->font.loadFromFile("data/fonts/BebasNeue Bold.ttf");
	this->text.setFont(this->font);
	this->text.setString(str);
	this->text.setCharacterSize(charsize);
	this->text.setColor(color);
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);
	this->text.setPosition(position);
	this->str=str;
	this->color=color;
	this->str_sel=str_sel;
	this->color_sel=color_sel;


}
ConsoleText::ConsoleText(sf::Vector2f position, sf::Vector2f size, ConsoleScreen* cs, std::string str, int charsize, sf::Color color): ConsoleObject(position, size, cs){
	this->font.loadFromFile("data/fonts/BebasNeue Bold.ttf");
	this->text.setFont(this->font);
	this->text.setString(str);
	this->text.setCharacterSize(charsize);
	this->text.setColor(color);
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);
	this->text.setPosition(position);
}
void ConsoleText::setText(std::string str){
	this->text.setString(str);
}
void ConsoleText::setColor(sf::Color color){
	this->text.setColor(color);
}

void ConsoleText::redraw(sf::RenderTexture &frame){
	frame.draw(this->text);
}
void ConsoleText::select(){
	this->text.setColor(this->color_sel);
	this->text.setString(this->str_sel);
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width/2.0f,
		       textRect.top  + textRect.height/2.0f);
}
void ConsoleText::deselect(){
	this->text.setColor(this->color);
	this->text.setString(this->str);
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width/2.0f,
		       textRect.top  + textRect.height/2.0f);
}
MainMenuScreen::MainMenuScreen(sf::VideoMode &vm, game_test* game): ConsoleScreen(vm){
	this->lastPin={0,0,0,0,0,0,0};
	this->game=game;
	this->keyTimeout=0.15;
	this->selected=0;
	this->title = new ConsoleText(sf::Vector2f(0.5*vm.width,0.2*vm.height), sf::Vector2f(0.1*vm.width,0.1*vm.height), this, "Game", 40, sf::Color(200,200,200));
	this->options[0] = new ConsoleText(sf::Vector2f(0.5*vm.width,0.35*vm.height), sf::Vector2f(0.1*vm.width,0.1*vm.height), this, "Play Game", 35, sf::Color(200,200,200), ">Play Game<", sf::Color(180,250,190));
	this->options[1] = new ConsoleText(sf::Vector2f(0.5*vm.width,0.35*vm.height+50), sf::Vector2f(0.1*vm.width,0.1*vm.height), this, "Quit", 35, sf::Color(200,200,200), ">Quit<", sf::Color(250,190,180));
	this->options[0]->select();
	this->addObject(this->title);
	this->addObject(this->options[0]);
	this->addObject(this->options[1]);



}
void MainMenuScreen::updatePhysics(PlayerInput &pin){
	ConsoleScreen::updatePhysics(pin);
	if(pin.select&&(!lastPin.select)){
		if(this->selected==0){
			this->game->loadLevel(LV_1);
			this->game->setState(GS_MENU_TO_WORLD);
		}
		if(this->selected==1) this->game->setState(GS_EXIT);
	}
	if((pin.forward&&(!lastPin.forward))||(pin.back&&(!lastPin.back))){
			this->options[this->selected]->deselect();
			if(pin.forward)
			this->selected=(this->selected-1);
			else
			this->selected=(this->selected+1);
			if(this->selected>1)this->selected=0;
			if(this->selected<0)this->selected=1;
			this->options[this->selected]->select();
	}
	lastPin=pin;
}


PauseMenuScreen::PauseMenuScreen(sf::VideoMode &vm, game_test* game): ConsoleScreen(vm){
	this->lastPin={0,0,0,0,0,0,0};
	this->game=game;
	this->keyTimeout=0.15;
	this->selected=0;
	this->title = new ConsoleText(sf::Vector2f(0.5*vm.width,0.2*vm.height), sf::Vector2f(0.1*vm.width,0.1*vm.height), this, "Game Paused", 40, sf::Color(200,200,200));
	this->options[0] = new ConsoleText(sf::Vector2f(0.5*vm.width,0.35*vm.height), sf::Vector2f(0.1*vm.width,0.1*vm.height), this, "Return to Game", 35, sf::Color(200,200,200), ">Return to Game<", sf::Color(180,250,190));
	this->options[1] = new ConsoleText(sf::Vector2f(0.5*vm.width,0.35*vm.height+50), sf::Vector2f(0.1*vm.width,0.1*vm.height), this, "New Game", 35, sf::Color(200,200,200), ">New Game<", sf::Color(180,250,190));
	this->options[2] = new ConsoleText(sf::Vector2f(0.5*vm.width,0.35*vm.height+100), sf::Vector2f(0.1*vm.width,0.1*vm.height), this, "Quit to Menu", 35, sf::Color(200,200,200), ">Quit to Menu<", sf::Color(250,190,180));
	this->options[0]->select();
	this->addObject(this->title);
	this->addObject(this->options[0]);
	this->addObject(this->options[1]);
	this->addObject(this->options[2]);

}
void PauseMenuScreen::updatePhysics(PlayerInput &pin){
	ConsoleScreen::updatePhysics(pin);
	if(pin.select&&(!lastPin.select)){
		if(this->selected==0)this->game->setState(GS_PAUSE_TO_WORLD);
		if(this->selected==1){
			this->game->loadLevel(LV_1);
			this->game->setState(GS_PAUSE_TO_WORLD);
		}
		if(this->selected==2) this->game->setState(GS_PAUSE_TO_MENU);
	}
	if((pin.forward&&(!lastPin.forward))||(pin.back&&(!lastPin.back))){
			this->options[this->selected]->deselect();
			if(pin.forward)
			this->selected=(this->selected-1);
			else
			this->selected=(this->selected+1);
			if(this->selected>2)this->selected=0;
			if(this->selected<0)this->selected=2;
			this->options[this->selected]->select();
	}
	lastPin=pin;
}
