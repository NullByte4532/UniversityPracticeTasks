#ifndef __fakestack_H_INCLUDED__ 
#define __fakestack_H_INCLUDED__
#include <cstdlib>
#include <cstdio>
#include "reg.hxx"
#include "ram.hxx"
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
#endif
