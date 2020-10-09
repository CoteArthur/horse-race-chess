#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h> 
#include <unistd.h> 

int main() {
    int p0, p1, p2, p3;
	int pipeP0_P1[2], pipeP1_P2[2], pipeP2_P3[2], pipeP3_P0[2];

	pipe(pipeP3_P0);
	pipe(pipeP0_P1);

	switch(fork()){
		case -1:
			perror("fork");
			exit(-1);

		case 0:	//p0
			// in
			close(0);
			dup(pipeP3_P0[0]);
			close(pipeP3_P0[0]);
			close(pipeP3_P0[1]);

			// out
			close(1);
			dup(pipeP0_P1[1]);
			close(pipeP0_P1[0]);
			close(pipeP0_P1[1]);

			//write
			
		default:
			pipe(pipeP1_P2);
			switch(fork()){
				case -1:
					perror("fork");
					exit(-1);

				case 0:	//p1
					close(pipeP3_P0[0]);
					close(pipeP3_P0[1]);

					// in
					close(0);
					dup(pipeP0_P1[0]);
					close(pipeP0_P1[0]);
					close(pipeP0_P1[1]);

					// out
					close(1);
					dup(pipeP1_P2[1]);
					close(pipeP1_P2[0]);
					close(pipeP1_P2[1]);

					//write
					
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

							// in
							close(0);
							dup(pipeP1_P2[0]);
							close(pipeP1_P2[0]);
							close(pipeP1_P2[1]);

							// out
							close(1);
							dup(pipeP2_P3[1]);
							close(pipeP2_P3[0]);
							close(pipeP2_P3[1]);

							//write
							
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

									// in
									close(0);
									dup(pipeP2_P3[0]);
									close(pipeP2_P3[0]);
									close(pipeP2_P3[1]);

									// out
									close(1);
									dup(pipeP3_P0[1]);
									close(pipeP3_P0[0]);
									close(pipeP3_P0[1]);

									//write
									
								default:

									// fermeture descripteurs inutilisés
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
							}
						}
					}
			}
	}
}
