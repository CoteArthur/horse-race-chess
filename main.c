#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>

#define PIPES_LENGHT 24
#define INT_SIZE sizeof(int)

void closePipes(int *);
int play (int, int);

int main() {
	pid_t bufferPid;
    int pipes[PIPES_LENGHT],
		players[] = {0, 0, 0, 0},
		curPlaying = 0,
		token = 0,
		i;

	for (i=0; i<PIPES_LENGHT; i+=2) pipe(pipes+i);

	while (1) {
		//Player 0
		if ((bufferPid = fork()) == 0) {
			if (curPlaying == 0) {
				players[0] = play(players[0], 0);
				token++;
				write(pipes[1], &token, INT_SIZE);
				read(pipes[6], &token, INT_SIZE);
				if (!token) exit(-1);
				write(pipes[11], players, INT_SIZE);
			} else {
				read(pipes[6], &token, INT_SIZE);
				write(pipes[1], &token, INT_SIZE);
			}
			closePipes(pipes);
			exit(0);
		} else if (bufferPid == -1) exit(-1);
		
		//Player 1
		if ((bufferPid = fork()) == 0) {
			if (curPlaying == 1) {
				players[1] = play(players[1], 1);
				token++;
				write(pipes[3], &token, INT_SIZE);
				read(pipes[0], &token, INT_SIZE);
				if (!token) exit(-1);
				write(pipes[15], players+1, INT_SIZE);
			} else {
				read(pipes[0], &token, INT_SIZE);
				write(pipes[3], &token, INT_SIZE);
			}
			closePipes(pipes);
			exit(0);
		} else if (bufferPid == -1) exit(-1);

		//Player 2
		if ((bufferPid = fork()) == 0) {
			if (curPlaying == 2) {
				players[2] = play(players[2], 2);
				token++;
				write(pipes[5], &token, INT_SIZE);
				read(pipes[2], &token, INT_SIZE);
				if (!token) exit(-1);
				write(pipes[19], players+2, INT_SIZE);
			} else {
				read(pipes[2], &token, INT_SIZE);
				write(pipes[5], &token, INT_SIZE);
			}
			closePipes(pipes);
			exit(0);
		} else if (bufferPid == -1) exit(-1);

		//Player 3
		if ((bufferPid = fork()) == 0) {
			if (curPlaying == 3) {
				players[3] = play(players[3], 3);
				token++;
				write(pipes[7], &token, INT_SIZE);
				read(pipes[4], &token, INT_SIZE);
				if (!token) exit(-1);
				write(pipes[23], players+3, INT_SIZE);
			} else {
				read(pipes[4], &token, INT_SIZE);
				write(pipes[7], &token, INT_SIZE);
			}
			closePipes(pipes);
			exit(0);
		} else if (bufferPid == -1) exit(-1);

		//Parent
		switch (curPlaying)
		{
			case 0:
				read(pipes[10], players+0, INT_SIZE);
				break;
			case 1:
				read(pipes[14], players+1, INT_SIZE);
				break;
			case 2:
				read(pipes[18], players+2, INT_SIZE);
				break;
			case 3:
				read(pipes[22], players+3, INT_SIZE);
				break;
			default:
				break;
		}
		
		printf("Player 0: %d\nPlayer 1: %d\nPlayer 2: %d\nPlayer 3: %d\n\n", players[0], players[1], players[2], players[3]);

		if (players[curPlaying] == 62) {
			printf("Player %d won\n", curPlaying);
			break;
		}

		curPlaying == 3 ? curPlaying = 0 : curPlaying++;

		while (wait(NULL)!=-1);
		usleep(500000);
	}
	
	closePipes(pipes);
	exit(0);
}

void closePipes(int* pipes) {
	for (int i=0; i<PIPES_LENGHT; i++) close(pipes[i]);
}

int play (int horseValue, int curPlaying) {
	srand(time(NULL) * getpid());
	int diceRoll = (rand() % 6) + 1;
	system("clear");
	printf("Player %d rolled a : %d\n\n", curPlaying, diceRoll);
	switch (horseValue)
	{
		case 0:
		case 60:
		case 61:
			if (diceRoll == 6) horseValue++;
			break;
		case 55:
			if (diceRoll == 1) horseValue++;
			break;
		case 56:
			if (diceRoll == 2) horseValue++;
			break;
		case 57:
			if (diceRoll == 3) horseValue++;
			break;
		case 58:
			if (diceRoll == 4) horseValue++;
			break;
		case 59:
			if (diceRoll == 5) horseValue++;
			break;
		default:
			horseValue += diceRoll;
			break;
	}
	return horseValue;
}