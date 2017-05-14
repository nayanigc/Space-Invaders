EXEC=main.exe
SOURCES=load.c game.c main.c menu.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-std=gnu99 -Wall -Werror -lm
  
.PHONY: default clean
 
default: $(EXEC)
 
load.o: load.c load.h
game.o: game.c game.h load.h menu.h
menu.o: menu.c menu.h load.h game.h
main.o: main.c game.h load.h menu.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
