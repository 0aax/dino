all: dino

dino: dino.o position.o
	g++ position.o dino.o -o dino -lncurses

dino.o: position.h dino.cpp dino.h
	g++ -c dino.cpp

position.o: position.cpp position.h
	g++ -c position.cpp

clean:
	rm  dino

install:
	cp dino /usr/local/bin/dino

uninstall:
	rm /usr/local/bin/dino
