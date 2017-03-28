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
#include "ram.hxx"
#include "FakeStack.hxx"
#include "instructions.cxx"

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
			case R_ESP:
				return esp.i;
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
			case R_ESP:
				return esp.d;
			case R_IO:
				scanf("%lf", &tmp);
				return tmp;
			default:
				fprintf(stderr, "ERROR: Unknown register code %x\n", code);
				sysexit();
				return 0;
		
	}
}



void cpu::writeregI(char code, int data){
	switch(code){
			case R_EAX:
				 eax.i=data;
				 return;
			case R_EBX:
				 ebx.i=data;
				 return;
			case R_ECX:
				 ecx.i=data;
				 return;
			case R_EIP:
				 eip.i=data;
				 return;
			case R_ESP:
				 esp.i=data;
				 return;
			case R_IO:
				printf("OUTPUT: %d\n", data);
				return;
			default:
				fprintf(stderr, "ERROR: Unknown register code %x\n", code);
				sysexit();
		
	}
}

void cpu::writeregD(char code, double data){
	switch(code){
			case R_EAX:
				 eax.d=data;
				 return;
			case R_EBX:
				 ebx.d=data;
				 return;
			case R_ECX:
				 ecx.d=data;
				 return;
			case R_EIP:
				 eip.d=data;
				 return;
			case R_ESP:
				 esp.d=data;
				 return;
			case R_IO:
				printf("OUTPUT: %lf\n", data);
				return;
			default:
				fprintf(stderr, "ERROR: Unknown register code %x\n", code);
				sysexit();
		
	}
}




int cpu::rarg(void* ptr){
	char type;
	type=ram.readC((unsigned)eip.i++);
	switch(type){
			case RA_CODE:
				*((double*)ptr)=ram.readD((unsigned int)readregI(ram.readC((unsigned)eip.i++)));
				return 2;
			case RAI_CODE:
				*((int*)ptr)=ram.readI((unsigned int)readregI(ram.readC((unsigned)eip.i++)));
				return 1;
			case RI_CODE:
				*((int*)ptr)=readregI(ram.readC((unsigned)eip.i++));
				return 1;
			case R_CODE:
				*((double*)ptr)=readregD(ram.readC((unsigned)eip.i++));
				return 2;
			case AI_CODE:
				*((int*)ptr)=ram.readI((unsigned int)ram.readI((unsigned)eip.i));
				eip.i=eip.i+(unsigned int) (sizeof(int)/sizeof(char));
				return 1;
			case A_CODE:
				*((double*)ptr)=ram.readD((unsigned int)ram.readI((unsigned)eip.i));
				eip.i=eip.i+(unsigned int) (sizeof(int)/sizeof(char));
				return 2;
			case NI_CODE:
				*((int*)ptr)=ram.readI((unsigned)eip.i);
				eip.i=eip.i+(unsigned int) (sizeof(int)/sizeof(char));
				return 1;
			case N_CODE:
				*((double*)ptr)=ram.readD((unsigned int)eip.i);
				eip.i=eip.i+(unsigned int) (sizeof(int)/sizeof(char));
				return 2;
			default:
				fprintf(stderr, "ERROR: Unknown argument type %x\n", type);
				sysexit();
		
	}
}

void cpu::wargI(int data){
	char type;
	type=ram.readC((unsigned)eip.i++);
	switch(type){
			case RA_CODE:
				ram.writeD((unsigned int)readregI(ram.readC((unsigned)eip.i++)), (double) data);
				return ;
			case RAI_CODE:
				ram.writeI((unsigned int)readregI(ram.readC((unsigned)eip.i++)), (int) data);
				return ;
			case RI_CODE:
				writeregI(ram.readC((unsigned)eip.i++), data);
				return ;
			case R_CODE:
				writeregD(ram.readC((unsigned)eip.i++), (double) data);
				return ;
			case AI_CODE:
				ram.writeI((unsigned int)ram.readI((unsigned)eip.i), data);
				eip.i=eip.i+(unsigned int) (sizeof(int)/sizeof(char));
				return ;
			case A_CODE:
				ram.writeD((unsigned int)ram.readI((unsigned)eip.i), (double) data);
				eip.i=eip.i+(unsigned int) (sizeof(int)/sizeof(char));
				return ;
			case NI_CODE:
			case N_CODE:
				fprintf(stderr, "ERROR: Attempted to write a constant");
				sysexit();
			default:
				fprintf(stderr, "ERROR: Unknown argument type %x\n", type);
				sysexit();
	}
}

void cpu::wargD(double data){
	char type;
	type=ram.readC((unsigned)eip.i++);

	switch(type){
			case RA_CODE:
				ram.writeD((unsigned int)readregI(ram.readC((unsigned)eip.i++)), (double) data);
				return ;
			case RAI_CODE:
				ram.writeI((unsigned int)readregI(ram.readC((unsigned)eip.i++)), (int) data);
				return ;
			case RI_CODE:
				writeregI(ram.readC((unsigned)eip.i++),(int) data);
				return ;
			case R_CODE:
				writeregD(ram.readC((unsigned)eip.i++), (double) data);
				return ;
			case AI_CODE:
				ram.writeI((unsigned int)ram.readI((unsigned)eip.i), (int)data);
				eip.i=eip.i+(unsigned int) (sizeof(int)/sizeof(char));
				return ;
			case A_CODE:
				ram.writeD((unsigned int)ram.readI((unsigned)eip.i), (double) data);
				eip.i=eip.i+(unsigned int) (sizeof(int)/sizeof(char));
				return ;
			case NI_CODE:
			case N_CODE:
				fprintf(stderr, "ERROR: Attempted to write a constant");
				return;
			default:
				fprintf(stderr, "ERROR: Unknown argument type %x\n", type);
				return;
	}
}


double cpu::rargD(){
	void* data;
	int ret;
	data=calloc(1, sizeof(double));
	ret=rarg(data);
	if (ret==1){
		return (double)*((int*)data);
	}else{
		return *((double*)data);
	}
	
}

int cpu::rargI(){
	void* data;
	int ret;
	data=calloc(1, sizeof(double));
	ret=rarg(data);
	if (ret==1){
		return *((int*)data);
	}else{
		return (int) *((double*)data);
	}
	
}

void cpu::clock(){
	char opcode;
	if(debug)printf("EIP: %d\n", eip.i);
	opcode=ram.readC((unsigned)eip.i++);
	if(debug)printf("Opcode: %x\n", opcode);
	switch(opcode){
		case OP_NOP:
			if(debug)printf("[OP]NOP\n");
			op_nop();
			break;
		case OP_PUSH:
			if(debug)printf("[OP]PUSH\n");
			op_push();
			break;
		case OP_POP:
			if(debug)printf("[OP]POP\n");
			op_pop();
			break;
		case OP_ADD:
			if(debug)printf("[OP]ADD\n");
			op_add();
			break;
		case OP_SUB:
			if(debug)printf("[OP]SUB\n");
			op_sub();
			break;
		case OP_MUL:
			if(debug)printf("[OP]MUL\n");
			op_mul();
			break;
		case OP_DIV:
			if(debug)printf("[OP]DIV\n");
			op_div();
			break;
		case OP_JMP:
			if(debug)printf("[OP]JMP\n");
			op_jmp();
			break;
		case OP_JEZ:
			if(debug)printf("[OP]JEZ\n");
			op_jez();
			break;
		case OP_JNZ:
			if(debug)printf("[OP]JNZ\n");
			op_jnz();
			break;
		case OP_JLZ:
			if(debug)printf("[OP]JLZ\n");
			op_jlz();
			break;
		case OP_JGZ:
			if(debug)printf("[OP]JGZ\n");
			op_jgz();
			break;
		case OP_CALL:
			if(debug)printf("[OP]CALL\n");
			op_call();
			break;
		case OP_RET:
			if(debug)printf("[OP]RET\n");
			op_ret();
			break;
		case OP_HLT:
			if(debug)printf("[OP]HLT\n");
			op_hlt();
			break;
		case OP_MOV:
			if(debug)printf("[OP]MOV\n");
			op_mov();
			break;
	}

	
}

void cpu::reset(){
	checkError(stack.Empty());
	checkError(callstack.Empty());
	eip.i=0;
	eax.d=0;
	ebx.d=0;
	ecx.d=0;
}

void cpu::load(FILE* fin){
	ram.load(fin);
}


cpu::cpu(int memsize, int stacksize):
	ram((unsigned)memsize),
	stack(&esp, &ram, stacksize),
	//stack((size_t)STACK_SIZE),
	callstack((size_t)CALL_STACK_SIZE)
{if(debug)printf("memsize=%d", memsize);}
void cpu::memdump(){
	int i;
	for(i=0; i<128; i++){
		
		printf("%x\n", ram.readC(i));
	}
	
}

