#include <cstdlib>
#include <cstdio>
#include "Stack.hxx"
#include "opcodes.hxx"
#include "cpu.hxx"
#include <malloc.h>
#include "ram.hxx"
#include "FakeStack.hxx"
#include "reg.hxx"
unsigned char FakeStack::Empty(){
	esp->i=ram->size_-sizeof(double)/sizeof(char)-1;
	return 0;
}
unsigned char FakeStack::Push(double val){
	esp->i=esp->i-sizeof(double)/sizeof(char);
	if (esp->i>ram->size_-maxsize)
		ram->writeD((unsigned)esp->i, val);
	else{
		if(debug){
		printf("ram->size_=%u\nmaxsize=%d\nesp->i=%d\n",ram->size_, maxsize, esp->i);	
			
		}
		 checkError(1);}
	return 0;
}
unsigned char FakeStack::Pop(double* val){
	if (esp->i<=ram->size_-sizeof(double)/sizeof(char))
		*val=ram->readD((unsigned)esp->i);
	else checkError(1);
	esp->i=esp->i+sizeof(double)/sizeof(char);
	return 0;
}
unsigned char FakeStack::Peek(double* val){
	if (esp->i<=ram->size_-sizeof(double)/sizeof(char))
		*val=ram->readD((unsigned)esp->i);
	else checkError(1);
	return 0;
}
FakeStack::FakeStack(reg* espP, Ram* ramP, int stacksize):
esp(espP),
ram(ramP),
maxsize(stacksize)
{}
