#include <SFML/Graphics.hpp>
namespace vm {
	float length(const sf::Vector2f& source);
	sf::Vector2f normalize(const sf::Vector2f& source);
	sf::Vector2f normal(const sf::Vector2f& source);
	float scalarProduct(const sf::Vector2f& v1, const sf::Vector2f& v2);
	int sign(float v);
	void rotate(sf::Vector2f &v, const float &angle);
	sf::Vector2f intersect(sf::Vector3f line1, sf::Vector3f line2);
	bool between(sf::Vector2f a, sf::Vector2f v1, sf::Vector2f v2);
	bool between(float a, float v1, float v2);
	float min(float a, float b);
	float angleBetween(sf::Vector2f v1, sf::Vector2f v2);
}
