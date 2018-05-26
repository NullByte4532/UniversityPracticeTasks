#include "vector.h"
#include <math.h>



float Vector::length() const {
	// можно просто sqrt(x * x + ... );
	return sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
}

Vector Vector::operator+(const Vector &v) const{
	return Vector(this->x+v.x, this->y+v.y, this->z+v.z);
};

Vector& Vector::operator+=(const Vector &v){
	this->x+=v.x;
	this->y+=v.y;
	this->z+=v.z;
	return *this;
};

Vector Vector::operator-(const Vector &v) const{
	return Vector(this->x-v.x, this->y-v.y, this->z-v.z);
};

Vector Vector::operator-() const{
	return Vector(-this->x, -this->y, -this->z);
}
		
Vector &Vector::operator-=(const Vector &v){
	this->x-=v.x;
	this->y-=v.y;
	this->z-=v.z;
	return *this;
};
		
float Vector::operator*(const Vector &v) const {
	return this->x*v.x + this->y*v.y + this->z*v.z;
};
		
Vector Vector::operator^(const Vector &v) const {
	return Vector(this->y*v.z-this->z*v.y, this->z*v.x-this->x*v.z, this->x*v.y-this->y*v.x);
};

Vector& Vector::operator*=(float k) {
	this->x*=k; 
	this->y*=k; 
	this->z*=k;
	return *this;
}

Vector& Vector::operator/=(float k) {
	this->x/=k;
	this->y/=k;
	this->z/=k;
	return *this;
}

bool Vector::is_null() const { 
	float epsilon = std::numeric_limits<float>::epsilon();
	
	// std::abs?
	// http://ru.cppreference.com/w/cpp/types/numeric_limits/epsilon
	return (this->x<epsilon && this->x>-epsilon && this->y<epsilon && this->y>-epsilon && this->z<epsilon && this->z>-epsilon);
}


Vector Vector::rotated(const Vector &axis, const float &angle) const {
	float cos_ = cos(angle);
	float sin_= sin(angle);
	if(axis.is_null() || this->is_null()) {
		std::cerr << "Error: Null vector supplied.\n";
		return Vector(0, 0, 0);
	}
	float new_x = (cos_ + (1-cos_)*axis.x*axis.x)*this->x
			 + ((1-cos_)*axis.x*axis.y - sin_*axis.z)*this->y
			 + ((1-cos_)*axis.x*axis.z + sin_*axis.y)*this->z;
	
	float new_y  = ((1-cos_)*axis.x*axis.y + sin_*axis.z)*this->x
			   + (cos_ + (1-cos_)*axis.y*axis.y)*y
			   + ((1-cos_)*axis.z*axis.y - sin_*axis.x)*this->z;

	float new_z = ((1-cos_)*axis.x*axis.z - sin_*axis.y)*this->x
			 + ((1-cos_)*axis.z*axis.y + sin_*axis.x)*this->y
			 + (cos_ + (1-cos_)*axis.z*axis.z)*this->z;

	return Vector(new_x, new_y, new_z);
}


Vector &Vector::rotate(const Vector &axis, const float &angle) {
	*this=rotated(axis, angle);
	return *this;
}
	
Vector &Vector::norm() {
	if(this->is_null()) {
		std::cerr << "Error: Null vector supplied.\n";
		this->x=0;
		this->y=0;
		this->z=0;
	} else {
		this->x=this->x/this->length();
		this->y=this->y/this->length();
		this->z=this->z/this->length();
	}
	return *this;
}

Vector Vector::normal() const {
	if(this->is_null()) {
		std::cerr << "Error: Null vector supplied.\n";
		return Vector(0, 0, 0);
	}
	if(this->x == 0 && this->y == 0) 
		return Vector(this->z, 0, 0);
	return Vector(-this->y, this->x, 0);
}

ostream &operator<<(ostream &stream, const Vector &v) {
	stream << v.x << " " << v.y << " " << v.z;
	return stream;
}

istream &operator>>(istream &stream, Vector &v) {
	stream >> v.x >> v.y >> v.z;
	return stream;
};
