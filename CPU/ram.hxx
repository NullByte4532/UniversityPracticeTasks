#ifndef __ram_H_INCLUDED__ 
#define __ram_H_INCLUDED__
#include <cstdlib>
#include <cstdio>
class Ram {
	public:
		Ram(unsigned int size);
		~Ram();
		void writeD(unsigned int addr, double data);
		void writeI(unsigned int addr, int data);
		double readD(unsigned int addr);
		int readI(unsigned int addr);
		char readC(unsigned int addr);
		void load(FILE* fin);
		unsigned int size_;
	private:
		void* data_;
	
};
#endif
