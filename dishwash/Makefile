all: processor.run input.run output.run
processor.run: processor.o split.o
		gcc processor.o split.o -o processor.run
processor.o: processor.c
		gcc -c processor.c
input.run: input.o split.o
		gcc input.o split.o -o input.run
input.o: input.c
		gcc -c input.c
output.run: output.c
		gcc output.c -o output.run
split.o: ../split/split.c
		gcc -c ../split/split.c
clean:
		rm *.run *.o
