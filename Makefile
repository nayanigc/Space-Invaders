EXEC=main.exe
SOURCES=load.c game.c main.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-std=gnu99 -Wall -Werror
  
.PHONY: default clean
 
default: $(EXEC)
 
load.o: load.c load.h
game.o: game.c game.h
main.o: main.c load.c
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~