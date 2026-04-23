#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for(int i = 0; i < N; i++){
		if (strcmp(cmd, allowed[i]) == 0){
			return 1;
		}
	}	


	return 0;
}

int main() {

    // TODO
    // Add variables as needed
    pid_t pid;
    int status;
    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c
	if (strcmp(line,"exit")==0) return 0;
	if (strcmp(line,"help")==0){
		printf("The allowed commands are:\n");
		printf("1: cp\n");
    		printf("2: touch\n");
    		printf("3: mkdir\n");
    		printf("4: ls\n");
    		printf("5: pwd\n");
    		printf("6: cat\n");
    		printf("7: grep\n");
    		printf("8: chmod\n");
    		printf("9: diff\n");
    		printf("10: cd\n");
    		printf("11: exit\n");
    		printf("12: help\n");
		continue;
	}

	char *p = strtok(line, " ");
	char *argv[21];

	int i = 0;
	while(p != NULL){
		argv[i++] = p;
		p = strtok(NULL, " ");
	}
	argv[i] = NULL;

	//for (int j = 0; j < i; j++) {
        //	printf("Token %d: %s\n", j, argv[j]);
    	//}

	if(i == 0){
		continue;
	}

	if(!isAllowed(argv[0])) {
		printf("NOT ALLOWED!\n");
		continue;
	}

	if(strcmp(argv[0],"cd") == 0){
		if(i > 2){
			printf("-rsh: cd: too many arguments\n");
		}
		else if(i == 2){
			chdir(argv[1]);
		}
		continue;
	}

    	posix_spawnattr_t attr;

    	// Initialize spawn attributes
    	posix_spawnattr_init(&attr);

    	// Set flags if needed, for example, to specify the scheduling policy
    	//posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

    	// Spawn a new process
    	if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
        	perror("spawn failed");
        	exit(EXIT_FAILURE);
    	}

    	// Wait for the spawned process to terminate
    	if (waitpid(pid, &status, 0) == -1) {
        	perror("waitpid failed");
	        exit(EXIT_FAILURE);
    	}

    	// Destroy spawn attributes
    	posix_spawnattr_destroy(&attr);
    	}
   	 return 0;
}
