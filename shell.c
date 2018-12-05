#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include "shell_functions.c"


/*======== main ==========
Inputs: N/A
Returns: N/A, but it runs the Reed_Shell
====================*/
int main () {

	initialize();
	int go = 1;
	char *user = getenv("USER");

	while(go) {	
	
		welcome(user);
		
		char input[250];
		fgets(input, 250, stdin);
		
		for(int i = strlen(input) - 1; i < strlen(input) + 1; i++) {
			input[i] = 0;
		}
		
		char ***commands = semicolons(input);
		
		for(int i = 0; i < 21; i++) {
			char **args = commands[i];

			if(!strcmp(args[0], "cd")) {
				cd(args[1]);
			}
			else if (!strcmp(args[0], "exit")) {
				go = 0;
			}
			if(go) {
				
				struct redir *redir_info = find_redir(commands, i);
				
				int process = fork();
				if(!process) {
					if(redir_info -> arg == -1) {
						execvp(args[0], args);
					}
					else {
						execreedvp(args, redir_info);
					}
					exit(0);
				}
				else if(process) {
					int s = 0;
					int child = wait(&s);
				}
			}
			if(commands[i + 1] == NULL) {
				i = 21;
			}
		}
	}

	leave();

	return 0;

}
