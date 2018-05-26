#include <SFML/Graphics.hpp>
#include "VectorMath.hpp"
#include <math.h>
#include <iostream>
	float vm::length(const sf::Vector2f& source){
	return sqrt((source.x * source.x) + (source.y * source.y));

	}
	sf::Vector2f vm::normalize(const sf::Vector2f& source)
	{
	    float l = length(source);
	    if (l != 0)
		return sf::Vector2f(source.x / l, source.y / l);
	    else
		return source;
	}
	sf::Vector2f vm::normal(const sf::Vector2f& source)
	{
	    float l = length(source);
	    if (l != 0)
		return sf::Vector2f(source.y, -source.x);
	    else
		return source;
	}
	float vm::scalarProduct(const sf::Vector2f& v1, const sf::Vector2f& v2){
		return v1.x*v2.x+v1.y*v2.y;
	}
	int vm::sign(float v){
    		return (0 < v) - (v < 0);
	}


	void vm::rotate(sf::Vector2f &v, const float &angle){
		v.x=cos(angle)*v.x-sin(angle)*v.y;
		v.y=cos(angle)*v.y+sin(angle)*v.x;
	}
	sf::Vector2f vm::intersect(sf::Vector3f line1, sf::Vector3f line2){
		float x, y;
		x=(line1.z*line2.y-line2.z*line1.y)/(line1.x*line2.y-line2.x*line1.y);
		y=(line1.x*line2.z-line2.x*line1.z)/(line1.x*line2.y-line2.x*line1.y);
		return sf::Vector2f(x, y);
	}
	bool vm::between(float a, float v1, float v2){
		return (a<v1&&a>v2||a<v2&&a>v1);
		
	}
	bool vm::between(sf::Vector2f a, sf::Vector2f v1, sf::Vector2f v2){

		return (vm::between(a.x, v1.x, v2.x)&&vm::between(a.y, v1.y, v2.y));


	}
	float vm::min(float a, float b){
		if(a>b) return b;
		return a;
	}
	float vm::angleBetween(sf::Vector2f v1, sf::Vector2f v2){

		return atan2(v1.x*v2.y-v1.y*v2.x, v1.x*v2.x+v1.y*v2.y);


	}
