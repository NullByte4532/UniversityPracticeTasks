#ifndef __cpu_H_INCLUDED__ 
#define __cpu_H_INCLUDED__
#include <cstdlib>
#include <cstdio>
#include "Stack.hxx"
#include "opcodes.hxx"

#define RAM_SIZE 512
#define STACK_SIZE 80
#define CALL_STACK_SIZE 16
#define debug 0
union reg {
	int i;
	double d;
};

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
		
	private:
		void* data_;
	
};
class FakeStack{
	public:
		unsigned char Push(double value);		//Push 'value' to the top of the stack
		unsigned char Pop(double* location);	//Pop an element from the top of the stack and write it to 'location'
		unsigned char Peek(double* location);	//Copy an element from the top of the stack and write it to 'location'
		unsigned char Empty();
		FakeStack(reg* esp, Ram* ram, int stacksize);
	private:
		reg* esp;
		Ram* ram;
		int maxsize;
		
};
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
