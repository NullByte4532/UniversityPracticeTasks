all: game.run


game.run: build/engine.o build/kinda_sorta_final.o build/VectorMath.o build/game.o build/menu.o build/game.o build/levels.o build/objects.o
	g++ build/engine.o build/kinda_sorta_final.o build/VectorMath.o build/menu.o build/game.o build/levels.o build/objects.o -o game.run -lsfml-graphics -lsfml-window -lsfml-system -std=c++11

build/engine.o: src/engine.cpp src/engine.hpp src/VectorMath.hpp 
	g++ -c src/engine.cpp -o build/engine.o -std=c++11
build/objects.o: src/objects.cpp src/engine.hpp src/VectorMath.hpp src/objects.hpp
	g++ -c src/objects.cpp -o build/objects.o -std=c++11
build/game.o: src/game.cpp src/engine.hpp src/VectorMath.hpp src/levels.hpp src/menu.hpp
	g++ -c src/game.cpp -o build/game.o -std=c++11
build/VectorMath.o: src/VectorMath.hpp src/VectorMath.cpp
	g++ -c src/VectorMath.cpp -o build/VectorMath.o -std=c++11
build/kinda_sorta_final.o: src/kinda_sorta_final.cpp src/VectorMath.hpp src/engine.hpp
	g++ -c src/kinda_sorta_final.cpp -o build/kinda_sorta_final.o -std=c++11
build/menu.o: src/menu.cpp src/VectorMath.hpp src/engine.hpp src/menu.hpp
	g++ -c src/menu.cpp -o build/menu.o -std=c++11
build/levels.o: src/levels.cpp src/levels.hpp src/engine.hpp
	g++ -c src/levels.cpp -o build/levels.o -std=c++11
clean: 
	rm build/* game.run
