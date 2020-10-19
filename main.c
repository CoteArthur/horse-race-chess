#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>

#define PIPES_LENGHT 24
#define PLAYERS_SIZE 4*sizeof(int)

void closePipes(int* pipes);
int play (int player, int curPlaying);

int main() {
	pid_t bufferPid;
    int pipes[PIPES_LENGHT],
		players[] = {0, 0, 0, 0},
		curPlaying = -1,
		i;

	for (i=0; i<PIPES_LENGHT; i+=2) pipe(pipes+i);

	//Player 0
	if ((bufferPid = fork()) == 0) {
		read(pipes[8], &curPlaying, sizeof(int));
		if (curPlaying == 0) {
			players[0] = play(players[0], 0);
			write(pipes[1], players, PLAYERS_SIZE);

			read(pipes[6], players, PLAYERS_SIZE); //TODO compare old and new players state
			write(pipes[11], players, PLAYERS_SIZE);
		} else {
			read(pipes[6], players, PLAYERS_SIZE);
			write(pipes[1], players, PLAYERS_SIZE);
		}
		closePipes(pipes);
		exit(0);
	} else if (bufferPid == -1) exit(-1);
	
	//Player 1
	if ((bufferPid = fork()) == 0) {
		read(pipes[12], &curPlaying, sizeof(int));
		if (curPlaying == 1) {
			players[1] = play(players[1], 1);
			write(pipes[3], players, PLAYERS_SIZE);

			read(pipes[0], players, PLAYERS_SIZE);
			write(pipes[15], players, PLAYERS_SIZE);
		} else {
			read(pipes[0], players, PLAYERS_SIZE);
			write(pipes[3], players, PLAYERS_SIZE);
		}
		closePipes(pipes);
		exit(0);
	} else if (bufferPid == -1) exit(-1);

	//Player 2
	if ((bufferPid = fork()) == 0) {
		read(pipes[16], &curPlaying, sizeof(int));
		if (curPlaying == 2) {
			players[2] = play(players[2], 2);
			write(pipes[5], players, PLAYERS_SIZE);

			read(pipes[2], players, PLAYERS_SIZE);
			write(pipes[19], players, PLAYERS_SIZE);
		} else {
			read(pipes[2], players, PLAYERS_SIZE);
			write(pipes[5], players, PLAYERS_SIZE);
		}
		closePipes(pipes);
		exit(0);
	} else if (bufferPid == -1) exit(-1);

	//Player 3
	if ((bufferPid = fork()) == 0) {
		read(pipes[20], &curPlaying, sizeof(int));
		if (curPlaying == 3) {
			players[3] = play(players[3], 3);
			write(pipes[7], players, PLAYERS_SIZE);

			read(pipes[4], players, PLAYERS_SIZE);
			write(pipes[23], players, PLAYERS_SIZE);
		} else {
			read(pipes[4], players, PLAYERS_SIZE);
			write(pipes[7], players, PLAYERS_SIZE);
		}
		closePipes(pipes);
		exit(0);
	} else if (bufferPid == -1) exit(-1);

	//Parent
	curPlaying = 0;
	for (i=9; i<=21; i+=4) write(pipes[i], &curPlaying, sizeof(int));
	read(pipes[10], players, PLAYERS_SIZE);
	printf("[%d, %d, %d, %d]\n\n", players[0], players[1], players[2], players[3]);

	closePipes(pipes);
	while (wait(NULL)!=-1);
	exit(0);
}

void closePipes(int* pipes) {
	for (int i=0; i<PIPES_LENGHT; i++) close(pipes[i]);
}

int play (int horseValue, int curPlaying) {
	srand(time(NULL) * getpid());
	int diceRoll = (rand() % 6) + 1;
	printf("Player %d rolled a : %d, ", curPlaying, diceRoll);

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
	printf("he's now at : %d\n", horseValue);
	
	return horseValue;
}