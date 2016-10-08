CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -g

all: player.o EAIT SCIENCE HABS HASS MABS stlucia

target: all

player.o: player.c player.h 
	$(CC) $(CFLAGS) player.c -c -o player.o

EAIT: eait.c player.o
	$(CC) $(CFLAGS) eait.c player.o -o EAIT

SCIENCE: science.c player.o
	$(CC) $(CFLAGS) science.c player.o -o SCIENCE

HABS: habs.c player.o
	$(CC) $(CFLAGS) habs.c player.o -o HABS

HASS: hass.c player.o
	$(CC) $(CFLAGS) hass.c player.o -o HASS

MABS: mabs.c player.o
	$(CC) $(CFLAGS) mabs.c player.o -o MABS

stlucia: stlucia.c stlucia.h
	$(CC) $(CFLAGS) stlucia.c -o stlucia
