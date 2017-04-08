#ifndef GAME_H
#define GAME_H 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <poll.h>
#include "load.h"


typedef struct {
 char** cases;
}plateau;

int play(niveau* niv);

#endif