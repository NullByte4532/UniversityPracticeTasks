#ifndef __Stack_H_INCLUDED__ 
#define __Stack_H_INCLUDED__
#include <cstdio>
class Stack {
	public:
		void dump(FILE* f);					//Dump Stack data to file 'f'
		unsigned char Broken();				//Check integrity. Returns unsigned char.
		unsigned char Push(double value);		//Push 'value' to the top of the stack
		unsigned char Pop(double* location);	//Pop an element from the top of the stack and write it to 'location'
		unsigned char Peek(double* location);	//Copy an element from the top of the stack and write it to 'location'
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
		int magic_;
		double* data_;
		int capacity_;
		int size_;
		unsigned char checksum_;
		unsigned char checksum2_;
		//======================
		int verifyPointer(void* p);	//returns 1 if pointer is correct.
		unsigned char calculateChecksum2();
		unsigned char calculateChecksum();
		int parity(int* data, unsigned int n);
		int parity(double** data, unsigned int n);
		void update();
};
void sysexit();
void checkError(int errcode);
#endif 
