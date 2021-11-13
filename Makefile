all:object linkage

object:
	g++ -c game.cpp

linkage:
	g++ -Wall -ansi -std=c++11 -g game.o -o game.out -lsfml-graphics -lsfml-system -lsfml-window
