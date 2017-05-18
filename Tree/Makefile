COMPILER=gcc
all: Tree.o example.o
		$(COMPILER) Tree.o example.o -o example.run
Tree.o: Tree.c
		$(COMPILER) -c Tree.c
example.o: example.c
		$(COMPILER) -c example.c
clean:
		rm -f *.o *.run *.gch
