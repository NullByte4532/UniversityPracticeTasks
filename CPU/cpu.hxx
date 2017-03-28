#ifndef __cpu_H_INCLUDED__ 
#define __cpu_H_INCLUDED__
#include <cstdlib>
#include <cstdio>
#include "Stack.hxx"
#include "opcodes.hxx"
#include "ram.hxx"
#include "FakeStack.hxx"
#include "reg.hxx"





class cpu {
	public:
		void clock();
		void load(FILE*);
		void reset();
		cpu(int memsize, int stacksize);
		void memdump();
		//Stack stack;
	private:
		reg eax;
		reg ebx;
		reg ecx;
		reg eip;
		reg esp;
		Stack callstack;
		FakeStack stack;
		Ram ram;
		void op_nop();
		void op_push();
		void op_pop();
		void op_add();
		void op_sub();
		void op_mul();
		void op_div();
		void op_jmp();
		void op_jez();
		void op_jnz();
		void op_jlz();
		void op_jgz();
		void op_call();
		void op_ret();
		void op_hlt();
		void op_mov();
		int rarg(void* ptr);
		void wargI(int data);
		void wargD(double data);
		int readregI(char code);
		double readregD(char code);
		void writeregI(char code, int data);
		void writeregD(char code, double data);
		double rargD();
		int rargI();
		
};

#endif 
