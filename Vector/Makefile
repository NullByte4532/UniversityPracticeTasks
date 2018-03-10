all: example.run

vector.o: vector.cpp
	g++ -c vector.cpp

example.o: example.cpp
	g++ -c example.cpp

example.run: vector.o example.o 
	g++ vector.o example.o -o example.run
test: example.run
	./example.run

clean:
	rm *.o *.run
