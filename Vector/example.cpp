#include "vector.h"
#include <math.h>
using std::endl;
int main() {
	Vector v1(45, 32, 12);
	Vector v(1, 9, 70);
	Vector axis(0, 1, 0);
	float angle = M_PI/2;
	std::cout << "v1.length = " << v1.length() << endl;
	Vector v2(v1);
	std::cout << "Enter coordinates for new vector"<< endl;;
	std::cin >> v;
	std::cout << "v1 + v2= " << v1 + v2 << endl;
	std::cout << "v1 - v2= " << v1 - v2 << endl;
	std::cout << "-v= " << -v << endl;
	std::cout << "(axis * v1)= " << (axis * v1) << endl;
	std::cout << "(v1 /= 5)= " << (v1 /= 5) << endl;
	std::cout << "(v *= 5)= " << (v *= 5) << endl;
	std::cout << "(v2 += v1)= " << (v2 += v1) << endl;
	std::cout << "(v2 -= v1)= " << (v2 -= v1) << endl;
	std::cout << "v2.norm= " << v2.norm() << endl;
	std::cout << "v2.normal= " << v2.normal() << endl;
	std::cout << "v.rotated(axis, angle)= " << v.rotated(axis, angle) << endl; 
	std::cout << "v.rotate(axis, angle)= " << v.rotate(axis, angle) << endl;
	std::cout << "(v ^ v.normal())= " << (v ^ v.normal()) << endl;


}
