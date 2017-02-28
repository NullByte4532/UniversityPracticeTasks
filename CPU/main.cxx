/*
 * main.cxx
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
void testStack(Stack* stack){			//simple test for a stack object
	int i;
	double j;
	for(i=0;i<=10;i++){
		checkError(stack->Push((double)i));
	}
	for(i=0;i<=10;i++){
		checkError(stack->Pop(&j));
		printf("%e\n", j);
	}
	for(i=0;i<=15;i++){
		checkError(stack->Push((double)i));
	}
	for(i=0;i<=15;i++){
		checkError(stack->Pop(&j));
		printf("%e\n", j);
	}
	checkError(stack->Push(5643.23));
	//checkError(stack->Pop((double*)0x00));
}
int main(int argc, char **argv)
{
	int a, i;
	Stack stack(30);
	testStack(&stack);
	stack.dump(stdout);
	for (i=2; i<6; i++) (&a)[i]=1234;
	stack.dump(stdout);
	testStack(&stack);
	stack.dump(stdout);
	
	return 0;
}
