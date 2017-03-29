/*
 * Asm.cxx
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
#include <cstring>
#include <malloc.h>
#include "opcodes.hxx"
#include "HashTable.hxx"
#include "Stack.hxx"
int calcArgSize(FILE* fin){
	char ch;
	char* arg;
	arg=(char*)calloc(ARG_MAX_LENGTH, sizeof(char)); 
	int i=0;
	ch=fgetc(fin);
	int f=0;
    while (ch && !isspace(ch) && ch!=0xffffffff) {
		if (i>=ARG_MAX_LENGTH){
			fprintf(stderr, "ERROR: Too long argument starting with %s\n", arg); return 0;
		}
		arg[i++]=ch;
		if(isdigit(ch)){
			if(f==2){
				f=3;
			}else if(f==0){ 
				f=1;
			}
		}else if(ch=='.'){
			if(f==2||f==3){
				f=4;
			}else if(f==1){ 
				f=2;
			}else{
				f=4;
			}
		}else{
			f=4;
		}
		ch=fgetc(fin);
		
	}
	if (arg[0]=='h'&&strlen(arg)<2){
		fprintf(stderr, "ERROR: Too short argument starting with %s\n", arg); return 0;
	}
	if 		(f==3)				{return sizeof(double)/sizeof(char)+1;}
	if 		(f==1)				{return sizeof(int)/sizeof(char)+1;}
	else if	(!strcmp(arg, "EAX")){return 2;}
	else if	(!strcmp(arg, "EBX")){return 2;}
	else if	(!strcmp(arg, "ECX")){return 2;}
	else if	(!strcmp(arg, "EIP")){return 2;}
	else if	(!strcmp(arg, "EIO")){return 2;}
	else if	(!strcmp(arg, "AX")){return 2;}
	else if	(!strcmp(arg, "BX")){return 2;}
	else if	(!strcmp(arg, "CX")){return 2;}
	else if	(!strcmp(arg, "IP")){return 2;}
	else if	(!strcmp(arg, "IO")){return 2;}
	else if	(arg[0]==ADDR_PREFIX){
				 if	(!strcmp(&arg[1], "AX")){return 2;}
			else if	(!strcmp(&arg[1], "BX")){return 2;}
			else if	(!strcmp(&arg[1], "CX")){return 2;}
			else if	(!strcmp(&arg[1], "IP")){return 2;}
			else if	(!strcmp(&arg[1], "IO")){return 2;}
			else if	(!strcmp(&arg[1], "SP")){return 2;}
			else{return 1+sizeof(int)/sizeof(char);}
		}
	else if	(arg[0]==ADDR_I_PREFIX){
				 if	(!strcmp(&arg[1], "AX")){return 2;}
			else if	(!strcmp(&arg[1], "BX")){return 2;}
			else if	(!strcmp(&arg[1], "CX")){return 2;}
			else if	(!strcmp(&arg[1], "IP")){return 2;}
			else if	(!strcmp(&arg[1], "IO")){return 2;}
			else if	(!strcmp(&arg[1], "SP")){return 2;}
			else{return 1+sizeof(int)/sizeof(char);}
		}
	else if	(arg[0]==LABEL_REF_PREFIX){return sizeof(int)/sizeof(char)+1;}
	else if (!strcmp(arg, "")){fprintf(stderr, "ERROR: Missing argument\n"); return 0;}
	else                      {fprintf(stderr, "ERROR: Unknown argument %s\n", arg); return 0;}
}

int process_labels(FILE* fin, HashTable* htable){
int curAddr=0;
int f=0, c=0;

while (1){
	char ch;
	char* op;
	op=(char*)calloc(OP_MAX_LENGTH, sizeof(char)); 
	int i=0;
	ch=fgetc(fin);
    while (ch && !isspace(ch) && ch!=0xffffffff) {
		if (i>=OP_MAX_LENGTH){
			fprintf(stderr, "ERROR: Too long operation name starting with %s\n", op); return -1;
		}
		op[i++]=ch;
		ch=fgetc(fin);
	}
	if (!f){
	if		(!strcmp(op, "NOP")){curAddr+=1;}
	else if	(!strcmp(op, "PUSH")){curAddr+=1+calcArgSize(fin);}
	else if	(!strcmp(op, "POP")){curAddr+=1+calcArgSize(fin);}
	else if	(!strcmp(op, "ADD")){curAddr+=1;}
	else if	(!strcmp(op, "SUB")){curAddr+=1;}
	else if	(!strcmp(op, "MUL")){curAddr+=1;}
	else if	(!strcmp(op, "DIV")){curAddr+=1;}
	else if	(!strcmp(op, "MOV")){curAddr+=1+calcArgSize(fin)+calcArgSize(fin);}
	else if	(!strcmp(op, "JMP")){curAddr+=1+calcArgSize(fin);}
	else if	(!strcmp(op, "JEZ")){curAddr+=1+calcArgSize(fin);}
	else if	(!strcmp(op, "JNZ")){curAddr+=1+calcArgSize(fin);}
	else if	(!strcmp(op, "JLZ")){curAddr+=1+calcArgSize(fin);}
	else if	(!strcmp(op, "JGZ")){curAddr+=1+calcArgSize(fin);}
	else if	(!strcmp(op, "CALL")){curAddr+=1+calcArgSize(fin);}
	else if	(!strcmp(op, "RET")){curAddr+=1;}
	else if	(!strcmp(op, "HLT")){curAddr+=1;}
	else if	(op[0]==LABEL_DEF_PREFIX){htable->add(&op[1], curAddr);}
	else if (!strcmp(op, ".data")){ f=1;}
	else if	(!strcmp(op, "")){fprintf(stdout, "done.\n"); return curAddr;}
	else					 {fprintf(stderr, "ERROR: Unknown operation %s\n", op); sysexit();}
}else{
	if(c==0){
		if	(!strcmp(op, "")){fprintf(stdout, "done.\n"); return curAddr;}
		htable->add(op, curAddr);
		if(debug) printf("HASHED %s AS %d\n", op, curAddr);
		c=1;
	}else{
		sscanf(op, "%d", &c);
		curAddr+=c;
		c=0;
	}
	
}
	
}

	return curAddr;
}
void write_file(FILE* fout, char c){
	fwrite(&c, sizeof(char), 1, fout);
}
int process_arg(FILE* fin, FILE* fout, HashTable* htable){
	char ch;
	char* arg;
	arg=(char*)calloc(ARG_MAX_LENGTH, sizeof(char)); 
	int i=0;
	ch=fgetc(fin);
	int val;
	double dval;
	int state=0;
    while (ch && !isspace(ch) && ch!=0xffffffff) {
		if (i>=ARG_MAX_LENGTH){
			fprintf(stderr, "ERROR: Too long argument starting with %s\n", arg); return 0;
		}
		arg[i++]=ch;
		if(isdigit(ch)){
			if(state==2){
				state=3;
			}else if(state==0){ 
				state=1;
			}
		}else if(ch=='.'){
			if(state==2||state==3){
				state=4;
			}else if(state==1){ 
				state=2;
			}else{
				state=4;
			}
		}else{
			state=4;
		}
		ch=fgetc(fin);
		
	}
	if (arg[0]=='h'&&strlen(arg)<2){
		fprintf(stderr, "ERROR: Too short argument starting with %s\n", arg); return 0;
	}
	if 		(state==3)		   	 {write_file(fout, N_CODE); sscanf(arg,"%lf", &dval); fwrite(&dval, sizeof(double), 1, fout); return 1;}
	if 		(state==1)			 {write_file(fout, NI_CODE); sscanf(arg,"%d", &val); fwrite(&val, sizeof(int), 1, fout); return 1;}
	else if	(!strcmp(arg, "EAX")){write_file(fout, R_CODE); write_file(fout, R_EAX); return 1;}
	else if	(!strcmp(arg, "EBX")){write_file(fout, R_CODE); write_file(fout, R_EBX); return 1;}
	else if	(!strcmp(arg, "ECX")){write_file(fout, R_CODE); write_file(fout, R_ECX); return 1;}
	else if	(!strcmp(arg, "EIP")){write_file(fout, R_CODE); write_file(fout, R_EIP); return 1;}
	else if	(!strcmp(arg, "EIO")){write_file(fout, R_CODE); write_file(fout, R_IO); return 1;}
	else if	(!strcmp(arg, "ESP")){write_file(fout, R_CODE); write_file(fout, R_ESP); return 1;}
	else if	(!strcmp(arg, "AX")){write_file(fout, RI_CODE); write_file(fout, R_EAX); return 1;}
	else if	(!strcmp(arg, "BX")){write_file(fout, RI_CODE); write_file(fout, R_EBX); return 1;}
	else if	(!strcmp(arg, "CX")){write_file(fout, RI_CODE); write_file(fout, R_ECX); return 1;}
	else if	(!strcmp(arg, "IP")){write_file(fout, RI_CODE); write_file(fout, R_EIP); return 1;}
	else if	(!strcmp(arg, "IO")){write_file(fout, RI_CODE); write_file(fout, R_IO); return 1;}
	else if	(!strcmp(arg, "SP")){write_file(fout, RI_CODE); write_file(fout, R_ESP); return 1;}
	else if	(arg[0]==ADDR_PREFIX){
				 if	(!strcmp(&arg[1], "AX")){write_file(fout, RA_CODE); write_file(fout, R_EAX); return 1;}
			else if	(!strcmp(&arg[1], "BX")){write_file(fout, RA_CODE); write_file(fout, R_EBX); return 1;}
			else if	(!strcmp(&arg[1], "CX")){write_file(fout, RA_CODE); write_file(fout, R_ECX); return 1;}
			else if	(!strcmp(&arg[1], "IP")){write_file(fout, RA_CODE); write_file(fout, R_EIP); return 1;}
			else if	(!strcmp(&arg[1], "IO")){write_file(fout, RA_CODE); write_file(fout, R_IO); return 1;}
			else if	(!strcmp(&arg[1], "SP")){write_file(fout, RA_CODE); write_file(fout, R_ESP); return 1;}
			else if (arg[1]==LABEL_REF_PREFIX){write_file(fout, A_CODE); val=htable->get(&arg[2]);fwrite(&val, sizeof(int), 1, fout);if(debug) printf("!\n"); return 1;}
			else{write_file(fout, A_CODE);sscanf(&arg[1],"%d", &val); fwrite(&val, sizeof(int), 1, fout); return 1;}
		}
	else if	(arg[0]==ADDR_I_PREFIX){
			 if	(!strcmp(&arg[1], "AX")){write_file(fout, RAI_CODE); write_file(fout, R_EAX); return 1;}
			else if	(!strcmp(&arg[1], "BX")){write_file(fout, RAI_CODE); write_file(fout, R_EBX); return 1;}
			else if	(!strcmp(&arg[1], "CX")){write_file(fout, RAI_CODE); write_file(fout, R_ECX); return 1;}
			else if	(!strcmp(&arg[1], "IP")){write_file(fout, RAI_CODE); write_file(fout, R_EIP); return 1;}
			else if	(!strcmp(&arg[1], "IO")){write_file(fout, RAI_CODE); write_file(fout, R_IO); return 1;}
			else if	(!strcmp(&arg[1], "SP")){write_file(fout, RAI_CODE); write_file(fout, R_ESP); return 1;}
			else if (arg[1]==LABEL_REF_PREFIX){write_file(fout, AI_CODE); val=htable->get(&arg[2]);fwrite(&val, sizeof(int), 1, fout);if(debug) printf("!\n"); return 1;}
			else{write_file(fout, AI_CODE);sscanf(&arg[1],"%d", &val); fwrite(&val, sizeof(int), 1, fout); return 1;}
		
		}
	else if	(arg[0]==LABEL_REF_PREFIX){write_file(fout, NI_CODE);val=htable->get(&arg[1]);fwrite(&val, sizeof(int), 1, fout); return 1;}
	else if (!strcmp(arg, "")){fprintf(stderr, "ERROR: Missing argument\n"); return 0;}
	else                      {fprintf(stderr, "ERROR: Unknown argument %s\n", arg); return 0;}
	
}
int process_op(FILE* fin, FILE* fout, HashTable* htable){
	char ch;
	char* op;
	op=(char*)calloc(OP_MAX_LENGTH, sizeof(char)); 
	int i=0;
	ch=fgetc(fin);
    while (ch && !isspace(ch) && ch!=0xffffffff) {
		if (i>=OP_MAX_LENGTH){
			fprintf(stderr, "ERROR: Too long operation name starting with %s\n", op); return 0;
		}
		op[i++]=ch;
		ch=fgetc(fin);
	}
	if		(!strcmp(op, "NOP")){write_file(fout,  OP_NOP); return 1;}
	else if	(!strcmp(op, "PUSH")){write_file(fout,  OP_PUSH); return process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "POP")){write_file(fout,  OP_POP); return process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "ADD")){write_file(fout,  OP_ADD); return 1;}
	else if	(!strcmp(op, "SUB")){write_file(fout,  OP_SUB); return 1;}
	else if	(!strcmp(op, "MUL")){write_file(fout,  OP_MUL); return 1;}
	else if	(!strcmp(op, "DIV")){write_file(fout,  OP_DIV); return 1;}
	else if	(!strcmp(op, "MOV")){write_file(fout,  OP_MOV); return process_arg(fin, fout, htable)&&process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "JMP")){write_file(fout,  OP_JMP); return process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "JEZ")){write_file(fout,  OP_JEZ); return process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "JNZ")){write_file(fout,  OP_JNZ); return process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "JLZ")){write_file(fout,  OP_JLZ); return process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "JGZ")){write_file(fout,  OP_JGZ); return process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "CALL")){write_file(fout,  OP_CALL); return process_arg(fin, fout, htable);}
	else if	(!strcmp(op, "RET")){write_file(fout,  OP_RET); return 1;}
	else if	(!strcmp(op, "HLT")){write_file(fout,  OP_HLT); return 1;}
	else if	(op[0]==':'){return 1;}
	else if	(!strcmp(op, "")||!strcmp(op, ".data")){fprintf(stdout, "done.\n"); return 0;}
	else					 {fprintf(stderr, "ERROR: Unknown operation %s\n", op); return 0;}
	
	
	
}

int main(int argc, char **argv)
{
	char* fin_name;
	char* fout_name;
	FILE* fin;
	FILE* fout;
	HashTable htable(32);
	int size, stack_size;
	fout_name="out.bin";
	switch(argc-1){
		case 1:
				fin_name=argv[1];
				break;
		case 2:	fin_name=argv[1];
				fout_name=argv[2];
				break;
		default:
			fprintf(stderr,"Usage: asm.run <input_file> [output_file]\n");
			return 1;
	}
	fin=fopen(fin_name, "r+");
	fout=fopen(fout_name, "wb");
	write_file(fout,HEADER_H);
	write_file(fout,HEADER_L);
	stack_size=DEF_STACK_SIZE;
	size=process_labels(fin, &htable)+stack_size;
	fwrite(&size, sizeof(int), 1, fout);
	fwrite(&stack_size, sizeof(int), 1, fout);
	rewind(fin);
	while(process_op(fin, fout, &htable)){}
	return 0;
}

