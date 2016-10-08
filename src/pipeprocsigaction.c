#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
//
//(isdigit()/isallnum())
//le père balance la série de caractères au fils et le fils compte quand le père envoie sigint

int signalIntercepte = 0;

void sighandler (int signal) {
	printf("signal intercepté\n");
	signalIntercepte = 1;
}

int main() {
	int fd[2];
	pid_t pid;
	char buf[255];
	int status;
	struct	sigaction act;
	act.sa_handler = &sighandler;

	if (pipe(fd) == -1) {
		fprintf(stderr, "error creating pipes");
		exit(1);
	}

	pid = fork();
	if (pid == 0) {
		close(fd[1]); /* close output side of pipe */
		int compteur = 0;
		for(int i = 0;(read(fd[0],&buf, sizeof(buf)))>0;++i) {
			if (isalnum(buf[i]) && !isdigit(buf[i])) {
				compteur++;
			}
		}
		//Le programme fait une itération en trop et le compteur ne fonctionne pas
		printf("nombre de caractères : %d", compteur);
	} 
	else {
		close(fd[0]); /* close input side of pipe */
		if (sigaction(SIGINT, &act, NULL) < 0) {
			perror ("sigaction");
			return 1;
		}
		for(;signalIntercepte == 0;) {
			scanf("%c",&buf);
			//isalnum sert à savoir si le caractère est alphanumérique
			//isdigit sert à savoir si le caractère est un chiffre
			write(fd[1],&buf, sizeof(buf));
		}
		waitpid(pid, &status, WEXITED);
	
	}
 
	return EXIT_SUCCESS;
}
