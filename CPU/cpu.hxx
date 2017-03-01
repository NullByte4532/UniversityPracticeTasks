#ifndef __cpu_H_INCLUDED__ 
#define __cpu_H_INCLUDED__
#include <cstdlib>
#include <cstdio>
#include "Stack.hxx"
#include "opcodes.hxx"


#define RAM_SIZE 512
#define STACK_SIZE 32
union reg {
	int i;
	double d;
}

class Ram {
	public:
		Ram(unsigned int size);
		~Ram();
		void writeD(double data, unsigned int addr);
		void writeI(int data, unsigned int addr);
		double readD(unsigned int addr);
		int readI(unsigned int addr);
		char readC(unsigned int addr);
	private:
		void* data_;
	
}
class cpu {
	public:
		void clock();
		void load(FILE*);
		void reset();
	private:
		reg eax;
		reg ebx;
		reg ecx;
		reg eip;
		Ram ram(RAM_SIZE);
		Stack stack(STACK_SIZE);
		void op_nop();
		void op_push();
		void op_pop();
		void op_add();
		void op_sub();
		void op_mul();
		void op_div();
		void op_lshift();
		void op_rshift();
		void op_jmp();
		void op_jez();
		void op_jnz();
		void op_jlz();
		void op_jgz();
		void op_call();
		void op_ret();
		void op_pushi();
		void op_popi();
		void op_hlt();
		int rarg(void* ptr);
		void wargI(int data);
		void wargD(double data);
		int readregI(char code);
		double readregD(char code);
		void writeregI(char code, int data);
		void writeregD(char code, double data);
		
}
#endif 
