#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
    int players[4] = {-1, -1, -1, -1};
	int pipeP0_P1[2], pipeP1_P2[2], pipeP2_P3[2], pipeP3_P0[2];

	srand(time(NULL));

	pipe(pipeP3_P0);
	pipe(pipeP0_P1);

	switch(fork()){
		case -1:
			perror("fork");
			exit(-1);

		case 0:	//p0
			close(pipeP3_P0[0]);
			close(pipeP3_P0[1]);

			close(pipeP0_P1[0]);

			int diceRoll = (rand() % 6) + 1;
            printf("Player 0 rolled a : %d\n", diceRoll);

            if (players[0] == -1) {
                if(diceRoll == 6) {
                    players[0]++;
                }
            } else {
                players[0] += diceRoll;
            }

            printf("Player 0 is now at : %d\n\n", players[0]);

			write(pipeP0_P1[1], players, 4*sizeof(int));

			close(pipeP0_P1[1]);

			exit(0);
		default:
			pipe(pipeP1_P2);

			switch(fork()){
				case -1:
					perror("fork");
					exit(-1);

				case 0:	//p1
					close(pipeP3_P0[0]);
					close(pipeP3_P0[1]);

					close(pipeP0_P1[1]);

					close(pipeP1_P2[0]);

					read(pipeP0_P1[0], players, 4*sizeof(int));

					int diceRoll = (rand() % 6) + 1;
                    printf("Player 1 rolled a : %d\n", diceRoll);

                    if (players[1] == -1) {
                        if(diceRoll == 6) {
                            players[1]++;
                        }
                    } else {
                        players[1] += diceRoll;
                    }

                    printf("Player 0 is now at : %d\n\n", players[0]);

                    write(pipeP1_P2[1], players, 4*sizeof(int));

					close(pipeP0_P1[0]);
					close(pipeP1_P2[1]);

					exit(0);
				default:
					pipe(pipeP2_P3);

					switch(fork()){
						case -1:
							perror("fork");
							exit(-1);

						case 0:	//p2
							close(pipeP3_P0[0]);
							close(pipeP3_P0[1]);

							close(pipeP0_P1[0]);
							close(pipeP0_P1[1]);

							close(pipeP1_P2[1]);

							close(pipeP2_P3[0]);

							int dice = -1;
                            read(pipeP1_P2[0], &dice, sizeof(int));
                            printf("dice value in p2: %d\n", dice);
                            write(pipeP2_P3[1], &dice, sizeof(int));

                            close(pipeP1_P2[0]);
                            close(pipeP2_P3[1]);

							exit(0);
						default:
							switch(fork()){
								case -1:
									perror("fork");
									exit(-1);

								case 0:	//p3
									close(pipeP0_P1[0]);
									close(pipeP0_P1[1]);

									close(pipeP1_P2[0]);
									close(pipeP1_P2[1]);

									close(pipeP2_P3[1]);

									close(pipeP3_P0[0]);

									int dice = -1;
                                    read(pipeP2_P3[0], &dice, sizeof(int));
                                    printf("dice value in p3: %d\n", dice);
                                    write(pipeP3_P0[1], &dice, sizeof(int));

                                    close(pipeP2_P3[0]);
                                    close(pipeP3_P0[1]);

									exit(0);
								default:
									close(pipeP0_P1[0]);
									close(pipeP0_P1[1]);

									close(pipeP1_P2[0]);
									close(pipeP1_P2[1]);

									close(pipeP2_P3[0]);
									close(pipeP2_P3[1]);

									close(pipeP3_P0[0]);
									close(pipeP3_P0[1]);

									// attente mort des fils
									while (wait(NULL)!=-1);

									exit(0);
							}
						}
					}
			}
	}
