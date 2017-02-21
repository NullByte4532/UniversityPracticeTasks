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
		unsigned char Broken();				//Check integrity. Returns unsigned char.
		unsigned char Push(double value);		//Push 'value' to the top of the stack
		unsigned char Pop(double* location);	//Pop an element from the top of the stack and write it to 'location'
		unsigned char Empty();				//Empty stack (does not clear nor reset memory)
		int getSize();				//Returns current stack size
		int getCapacity();			//Returns stack capacity
		Stack(int capacity);		//Constructor. Allocates memory. 'capacity' is the maximum number of elements
		~Stack();					//Destructor. Frees memory.
/*
 * All functions (except those with get- prefix and Broken)
 * return an error code.
 * Possible return codes:
 * 0- Completed successfully
 * 1- Current stack size does not allow such operation
 * 2- Null pointer supplied
 * 3- Corrupted stack
 */
 
 /* Broken returns an unsigned char where (lsb first):
  * bit 0- invalid data pointer;
  * bit 1- capacity<size;
  * bit 2- capacity<=0;
  * bit 3- wrong checksum2;
  * bit 4- wrong checksum;
  * bit 5- wrong magic;
  */
	private:
		double* data_;
		int capacity_;
		int size_;
		int verifyPointer(void* p);	//returns 1 if pointer is correct.
		int magic_;
		unsigned char checksum_;
		unsigned char checksum2_;
		unsigned char calculateChecksum2();
		unsigned char calculateChecksum();
		int parity(int* data, unsigned int n);
		int parity(double** data, unsigned int n);
		void update();
};
Stack::Stack(int capacity):
capacity_(capacity),
size_(0),
magic_(564323)
{
	data_ = (double*)calloc(capacity, sizeof(double));
	update();
}
unsigned char Stack::Push(double value){
	if(!Broken()){
	if (size_<capacity_){
		data_[size_++]=value;
		update();
		return 0;
	}
	else{
		return 1;
	}	
}
return 3;
}
unsigned char Stack::Pop(double* location){
	if(!Broken()){
	if (capacity_>0){
		if(verifyPointer(location)){
			*location=data_[--size_];
			update();
			return 0;
		}else{
			return 2;
		}
	}
	else{
		return 1;
	}
}
return 3;
}
unsigned char Stack::Empty(){
	if(!Broken()){
	size_=0;
	update();
	return 0;
}
	return 3;
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
int Stack::verifyPointer(void* p){
	int f;
	f=(p==NULL); //verify that pointer is not null
	return !f;
}
int Stack::parity(int* data, unsigned int n){
	return *data%n;
}
int Stack::parity(double** data, unsigned int n){
	unsigned int i, tmp=0;
	for (i=0; i<=sizeof(double*); i++)
	tmp+=((char*)data)[i]%n;
	return tmp%n;
	
}
unsigned char Stack::calculateChecksum2(){
	unsigned char tmp=0;
	tmp+=parity(&data_, 4);
	tmp+=parity(&capacity_, 4)<<2;
	tmp+=parity(&size_, 4)<<4;
	return tmp;
}
unsigned char Stack::calculateChecksum(){
	unsigned int tmp=0;
	tmp+= parity(&data_, 256);
	tmp+= parity(&capacity_, 256);
	tmp+= parity(&size_, 256);
	return (unsigned char) tmp % 256;
	
}
unsigned char Stack::Broken(){
	char f=0;
	f=(f+!verifyPointer(data_)+(capacity_<size_)*2+(capacity_<=0)*4);
	f=(f+(checksum2_!=calculateChecksum2())*8+(checksum_!=calculateChecksum())*16+(magic_!=564323)*32);
	return f;
}
void Stack::update(){
	checksum_=calculateChecksum();
	checksum2_=calculateChecksum2();
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
		case 3:
			cerr<<"Corrupted stack."<<endl;
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
	checkError(stack->Push(5643.23));
	//checkError(stack->Pop((double*)0x00));
}
int main(int argc, char **argv)
{
	int a, i;
	Stack stack(30);
	testStack(&stack);
	cout << "CHECK " << (int)stack.Broken() << endl;
	for (i=2; i<8; i++) (&a)[i]=1234;
	cout << "CHECK " << (int)stack.Broken() << endl;
	testStack(&stack);
	
	return 0;
}

