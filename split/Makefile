test: example.run
	./test.sh
example.run: split.o example.o
	gcc split.o example.o -o example.run
example.o: example.c split.h
	gcc -c example.c
split.o: split.c split.h
	gcc -c split.c
