all: Main

o: BoardGenerator.o GameBoard.o Puzzle.o Position.o Main.o

BoardGenerator.o:
	g++ -c BoardGenerator.cpp

GameBoard.o:
	g++ -c GameBoard.cpp

Puzzle.o:
	g++ -c Puzzle.cpp

Position.o:
	g++ -c Position.cpp

Main.o:
	g++ -c Main.cpp

Main: o
	g++ BoardGenerator.o GameBoard.o Puzzle.o Position.o Main.o -o puzzle-app -lsfml-graphics -lsfml-window -lsfml-system

run: Main
	./puzzle-app

clean:
	rm *.o puzzle-app

