#include "game.h"


int play(niveau* niv){
	char buffer[1024];
	struct pollfd fds[1];
	int timeout_msecs=-1;
	fds[0].fd = 0;
	fds[0].events=POLLIN;
	while(true){
		int res = poll(fds,1,timeout_msecs);
		if (res>0){
			if (fds[0].revents & POLLIN){
				int nread = read(fds[0].fd, buffer, 1024);
				if(nread > 0){
					if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 68){
						//write(1, "GAUCHE\n", 8);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 65){
						//write(1, "HAUT\n", 6);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 67){
						//write(1, "DROITE\n", 8);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 66){
						//write(1, "BAS\n", 5);
					} else if(buffer[0] == 113){
						break;
					}
				}
			}
		}	
	}
	return 0;
}