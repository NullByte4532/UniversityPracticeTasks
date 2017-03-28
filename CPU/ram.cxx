#include "ram.hxx"
#include "opcodes.hxx"
#include <cstdlib>
#include <cstdio>
#include "Stack.hxx"
double Ram::readD(unsigned int addr){
	if(debug)printf("Reading double from %d\n", addr); 
	if(addr+sizeof(double)/sizeof(char)>=size_){
		fprintf(stderr,"ERROR: attempted to read double from %d when ram size is %d.", addr, size_);
		sysexit();
	}
	return ((double*)&(((char*)data_)[addr]))[0];	
}
int Ram::readI(unsigned int addr){
	if(debug)printf("Reading int from %d\n", addr); 
	if(addr+sizeof(int)/sizeof(char)>=size_){
		fprintf(stderr,"ERROR: attempted to read int from %d when ram size is %d.", addr, size_);
		sysexit();
	}
	return ((int*)&(((char*)data_)[addr]))[0];		
}
char Ram::readC(unsigned int addr){
	if(debug)printf("Reading char from %d\n", addr); 
	if(addr+1>=size_){
		fprintf(stderr,"ERROR: attempted to read char from %d when ram size is %d.", addr, size_);
		sysexit();
	}
	return ((char*)data_)[addr];		
}
void Ram::writeD(unsigned int addr, double data){
	
	if(debug)printf("Writing double %lf to %d\n", data, addr); 
	if(addr+sizeof(double)/sizeof(char)>=size_){
		fprintf(stderr,"ERROR: attempted to write double to %d when ram size is %d.", addr, size_);
		sysexit();
	}
	((double*)&(((char*)data_)[addr]))[0]=data;	
}
void Ram::writeI(unsigned int addr, int data){
	if(debug)printf("Writing int %d to %d\n", data, addr);
	if(addr+sizeof(int)/sizeof(char)>=size_){
		fprintf(stderr,"ERROR: attempted to write int to %d when ram size is %d.", addr, size_);
		sysexit();
	}
	((int*)&(((char*)data_)[addr]))[0]=data;	
}
Ram::Ram(unsigned int size):
size_(size)
{
	data_=calloc(size, sizeof(char));
}
Ram::~Ram(){
		free(data_);
}
void Ram::load(FILE* fin){
	fread(data_, sizeof(char), size_, fin);
}
