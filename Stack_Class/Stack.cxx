/*
 * Stack.cxx
 * 
 * Copyright 2017 NullByte4532 <nullbyte4532@nullbyte4532-Lenovo-G510>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <cstdlib>
using namespace std;
class Stack {
	public:
		int Push(double value);		//Push 'value' to the top of the stack
		int Pop(double* location);	//Pop an element from the top of the stack and write it to 'location'
		int Empty();				//Empty stack (does not clear nor reset memory)
		int getSize();				//Returns current stack size
		int getCapacity();			//Returns stack capacity
		Stack(int capacity);		//Constructor. Allocates memory. 'capacity' is the maximum number of elements
		~Stack();					//Destructor. Frees memory.
/*
 * All functions (except those with get- prefix)
 * return an error code.
 * Possible return codes:
 * 0- Completed successfully
 * 1- Current stack size does not allow such operation
 * 2- Null pointer supplied
 */
	private:
		double* data_;
		int capacity_;
		int size_;
};
Stack::Stack(int capacity):
data_(new double[capacity]),
capacity_(capacity),
size_(0)
{
	//data_ = (double*)calloc(capacity, sizeof(double));
}
int Stack::Push(double value){
	if (size_<capacity_){
		data_[size_++]=value;
		return 0;
	}
	else{
		return 1;
	}	
}
int Stack::Pop(double* location){
	if (capacity_>0){
		if(location){
			*location=data_[--size_];
			return 0;
		}else{
			return 2;
		}
	}
	else{
		return 1;
	}
}
int Stack::Empty(){
	size_=0;
	return 0;
}
int Stack::getSize(){
	return size_;
}
int Stack::getCapacity(){
	return capacity_;
}
Stack::~Stack(){
	free(data_);
	
}

//#################################################################################################################################


void sysexit(){					//makes sys_exit syscall with exit code of 1
__asm__	(	"movl $1, %eax;"	//sys_exit
			"movl $1, %ebx;"	//1
			"int $128;");		//syscall
}
void checkError(int errcode){	//interprets error codes
	switch(errcode){
		case 0:
			break;
		case 1:
			cerr<<"Current stack size does not allow such operation."<<endl;
			sysexit();
			break;
	
		case 2:
			cerr<<"Null pointer supplied."<<endl;
			sysexit();
			break;
	
		default:
			cerr<<"Unknown error."<<endl;
			sysexit();
			break;
	
	}	
}
void testStack(Stack* stack){			//simple test for a stack object
	int i;
	double j;
	for(i=0;i<=10;i++){
		checkError(stack->Push((double)i));
	}
	for(i=0;i<=10;i++){
		checkError(stack->Pop(&j));
		cout << j << endl;
	}
	for(i=0;i<=15;i++){
		checkError(stack->Push((double)i));
	}
	for(i=0;i<=15;i++){
		checkError(stack->Pop(&j));
		cout << j << endl;
	}
	checkError(stack->Push(4532.12));
	checkError(stack->Pop((double*)0x00));
}
int main(int argc, char **argv)
{
	Stack stack(30);
	testStack(&stack);
	return 0;
}

