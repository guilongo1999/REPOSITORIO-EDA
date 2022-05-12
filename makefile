CC = gcc
CCFLAGS = -Wall

all : fase1main

fase1main : fase1.o fase1main.o

fase1.o : fase1.c

fase1main.o : fase1main.c

clean :	@rm *.o
	@rm fase1main
