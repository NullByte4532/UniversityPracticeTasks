/*
 * cpu.cxx
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
#include <cstdio>
#include "Stack.hxx"
#include "opcodes.hxx"
#include "cpu.hxx"
#include <malloc.h>


double Ram::readD(unsigned int addr){
	if(addr+sizeof(double)/sizeof(char)>=RAM_SIZE){
		fprintf(stderr,"ERROR: attempted to read double from %d when ram size is %d.", addr, RAM_SIZE);
		sysexit();
	}
	return ((double*)data_)[addr];	
}
int Ram::readI(unsigned int addr){
	if(addr+sizeof(int)/sizeof(char)>=RAM_SIZE){
		fprintf(stderr,"ERROR: attempted to read int from %d when ram size is %d.", addr, RAM_SIZE);
		sysexit();
	}
	return ((int*)data_)[addr];		
}
char Ram::readC(unsigned int addr){
	if(addr+1>=RAM_SIZE){
		fprintf(stderr,"ERROR: attempted to read char from %d when ram size is %d.", addr, RAM_SIZE);
		sysexit();
	}
	return ((char*)data_)[addr];		
}
void Ram::writeD(double data, unsigned int addr){
	if(addr+sizeof(double)/sizeof(char)>=RAM_SIZE){
		fprintf(stderr,"ERROR: attempted to write double to %d when ram size is %d.", addr, RAM_SIZE);
		sysexit();
	}
	((double*)data_)[addr]=data;	
}
void Ram::writeI(int data, unsigned int addr){
	if(addr+sizeof(int)/sizeof(char)>=RAM_SIZE){
		fprintf(stderr,"ERROR: attempted to write int to %d when ram size is %d.", addr, RAM_SIZE);
		sysexit();
	}
	((int*)data_)[addr]=data;	
}
Ram::Ram(unsigned int size){
	data_=calloc(size, sizeof(char));
}
Ram::~Ram(){
		free(data_);
}
void cpu::op_nop(){
return;	
}
int cpu::readregI(char code){
	int tmp;
	switch(code){
			case R_EAX:
				return eax.i;
			case R_EBX:
				return ebx.i;
			case R_ECX:
				return ecx.i;
			case R_EIP:
				return eip.i;
			case R_IO:
				scanf("%d", &tmp);
				return tmp;
			default:
				fprintf(stderr, "ERROR: Unknown register code %x\n", code);
				sysexit();
		
	}
}
double cpu::readregD(char code){
	double tmp;
	switch(code){
			case R_EAX:
				return eax.d;
			case R_EBX:
				return ebx.d;
			case R_ECX:
				return ecx.d;
			case R_EIP:
				return eip.d;
			case R_IO:
				scanf("%lf", &tmp);
				return tmp;
			default:
				fprintf(stderr, "ERROR: Unknown register code %x\n", code);
				sysexit();
				return 0;
		
	}
}
int cpu::rarg(void* ptr){
	char type;
	type=ram.readC((unsigned)eip.i++);
	switch(type){
			case RI_CODE:
				*((int*)ptr)=readregI(ram.readC((unsigned)eip.i++));
				return 1;
			case R_CODE:
				*((double*)ptr)=readregD(ram.readC((unsigned)eip.i++));
				return 2;
			case AI_CODE:
				*((int*)ptr)=ram.readI((unsigned int)ram.readI((unsigned)eip.i))
				eip.i=eip.i+sizeof(int)/sizeof(char);
				return 1;
			case A_CODE:
				*((double*)ptr)=ram.readD((unsigned int)ram.readI((unsigned)eip.i))
				eip.i=eip.i+sizeof(int)/sizeof(char);
				return 2;
			case NI_CODE:
				*((int*)ptr)=ram.readI((unsigned)eip.i)
				eip.i=eip.i+sizeof(int)/sizeof(char);
				return 1;
			case N_CODE:
				*((double*)ptr)=ram.readD((unsigned int)eip.i)
				eip.i=eip.i+sizeof(double)/sizeof(char);
				return 2;
		
	}
}
void cpu::op_push(){
	void* data;
	int ret;
	data=calloc(1, sizeof(double));
	ret=rarg(data);
	if (ret==1){
		stack.Push((double)*((int*)data));
	}else{
		stack.Push(*((double*)data));
	}
	
}
