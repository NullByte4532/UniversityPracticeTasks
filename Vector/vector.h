#pragma once 

#include <iostream>
#include <cstdlib>
#include <limits>


using std::istream;
using std::ostream;


class Vector {
	
	private:
		
		float x;
		float y;
		float z;
	
	public:
		Vector(float x, float y, float z): x(x), y(y), z(z) {};
		Vector();
		// зачем писать this->x, если можно просто x?
		float get_x() const { return this->x; }
		float get_y() const { return this->y; }
		float get_z() const { return this->z; }
		Vector operator+(const Vector& v) const;
		Vector &operator+=(const Vector& v);
		Vector operator-(const Vector& v) const;
		Vector operator-() const;
		Vector &operator-=(const Vector& v);
		float operator*(const Vector& v) const;
		Vector operator^(const Vector& v) const;
		Vector &operator*=(float k); 
		Vector &operator/=(float k); 
		Vector normal() const;
		float length() const;				
		Vector &norm(); 
		Vector &rotate(const Vector &axis, const float &angle);
		Vector rotated(const Vector &axis, const float &angle) const;
		friend ostream &operator<<(ostream &stream, const Vector &v);
		friend istream &operator>>(istream &stream, Vector &v);
		bool is_null() const;
};
