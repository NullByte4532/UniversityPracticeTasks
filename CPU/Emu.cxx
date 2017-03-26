/*
 * Emu.cxx
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
#include "cpu.hxx"


int main(int argc, char **argv)
{
	unsigned int i;
	FILE* fin;
	char tmpchar;
	int memsize, stacksize;
	if(argc!=2){
		printf("Usage: emu.run <input_file>\n");
		return 1;
	}
	
	fin=fopen(argv[1], "rb");
	fread(&tmpchar, sizeof(char), 1, fin);
	if (tmpchar!=HEADER_H){
		fprintf(stderr, "Error: Wrong header\n");
		return 1;
	}
	fread(&tmpchar, sizeof(char), 1, fin);
	if (tmpchar!=HEADER_L){
		fprintf(stderr, "Error: Wrong header\n");
		return 1;
	}
	fread(&memsize, sizeof(int), 1, fin);
	fread(&stacksize, sizeof(int), 1, fin);
	cpu my_cpu(memsize, stacksize);
	my_cpu.load(fin);
	if(debug)my_cpu.memdump();
	my_cpu.reset();
	while(1){
			
			my_cpu.clock();
			//if(debug)my_cpu.stack.dump(stdout);
	}
	 
	return 0;
}

