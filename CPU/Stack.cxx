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


#include <cstdlib>
#include <cstdio>
#include "Stack.hxx"

Stack::Stack(int capacity):
magic_(564323),
capacity_(capacity),
size_(0)
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
void Stack::dump(FILE* f){
	unsigned char error;
	error=Broken();
	fprintf(f, "Stack object at <%p>:\n", this);
	fprintf(f, "\t valid:\t\t");
	if(error){
		fprintf(f, "[error %d]\n", error);
	}else{
		fprintf(f, "[ok]\n");
	}
	fprintf(f, "\t Data pointer:\t<%p>\n", data_);
	fprintf(f, "\t Capacity:\t%d\n", capacity_);
	fprintf(f, "\t Size:\t\t%d\n", size_);
	fprintf(f, "\t Magic:\t\t%d\n", magic_);
	fprintf(f, "\t Checksum:\t%02x\n", checksum_);
	fprintf(f, "\t Checksum2:\t%02x\n", checksum2_);
	
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

unsigned char Stack::Peek(double* location){
	if(!Broken()){
	if (capacity_>0){
		if(verifyPointer(location)){
			*location=data_[size_-1];
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
int Stack::verifyPointer(void* pointer){
	return (pointer!=NULL && !(((char*)&pointer)[sizeof(void*)/sizeof(char)]%2));//verify that pointer is not null
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
			fprintf(stderr,"Current stack size does not allow such operation.\n");
			sysexit();
			break;
	
		case 2:
			fprintf(stderr,"Null pointer supplied.\n");
			sysexit();
			break;
		case 3:
			fprintf(stderr,"Corrupted stack.\n");
			sysexit();
			break;
		default:
			fprintf(stderr,"Unknown error.\n");
			sysexit();
			break;
	
	}	
}


