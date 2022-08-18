all: compile link

compile:
	g++ -c -Isrc/include main.cpp

link:
	g++ main.o -o run/app -lsfml-graphics -lsfml-window -lsfml-system
