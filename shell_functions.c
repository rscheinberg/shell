#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include "shell_functions.h"

/*======== cd ==========
Inputs: @path to the directory the process is being changed to
Returns: N/A, but it changes the directory and prints an error if needed
====================*/
void cd (char *path) {
	
	int status = chdir(path);
	if(status) printf("%s\n", strerror(errno));
	
}

/*======== remove_spaces ==========
Inputs: @line being modified
Returns: The modified line with the spaces at either end of the string removed
====================*/
char * remove_spaces(char * line) {
	
	int i = 0;
	while(line[i] == ' ') {
		line++;
		i++;
	}
	
	i = strlen(line) - 1;
	while(line[i] == ' ') {
		line[i] = '\0';
		i--;
	}
	
	return line;
	
}

/*======== parse_args ==========
Inputs: @line being parsed, and the number of @argc spaces in that line
Returns: The properly parsed line in char ** form
====================*/
char ** parse_args(int argc, char * line) {
    
	char **arr = calloc(argc + 1, sizeof(char *));
    
	char *new_line = calloc(strlen(line),sizeof(char));
	strcpy(new_line, line);
	new_line = remove_spaces(new_line);
    
	for (int i = 0 ; new_line; i++) {
		arr[i] = strsep(&new_line," ");
	}
	
	arr[argc] = NULL;
    
	return arr;
}

/*======== leave ==========
Inputs: N/A
Returns: N/A, but prints a friendly goodbye message
====================*/
void leave () {
	
	printf("Closing...\n");
	sleep(2);
	printf("Thank you for using Reed_Shell! Have a good day.\n");
}

void initialize () {
	
	printf("\nWelcome to Reed_Shell!\n");
	printf("Loading...\n\n");
	sleep(2);
	
}

/*======== charfind ==========
Inputs: the char @c being searched in string (char array) @s
Returns: The number of times @c appears in @s
====================*/
int charfind(char s[], char c) { 

	int n = 0; 
  
	for (int i = 0; i < strlen(s); i++) 
  
	if (s[i] == c) {
		n++;
	} 
  
	return n;

}

/*======== semicolons ==========
Inputs: The @input being separated by semicolons
Returns: An array of arrays of strings, separated by semicolons, with each element being in a form similar to args from the command line
====================*/
char ***semicolons (char *input) {
	
	char ***commands = calloc(21, sizeof(char **));
	int cur = -1;
	int parse = 0;
	
	for(int i = 0; i < strlen(input); i++) {
		if(input[i] == ';') {
			char cmnd[i - cur];
			cmnd[i - cur - 1] = '\0';
			for(int j = cur + 1; j < i; j++) {
				cmnd[j - cur - 1] = input[j];
			}
			commands[parse] = parse_args(charfind(cmnd, ' ') + 1, cmnd);
			cur = i;
			parse++;
		}
	}
	
	cur++;
	char final[strlen(input) - cur + 1];
	final[strlen(input) - cur] = '\0';
	for(int i = 0; cur < strlen(input); cur++) {
		final[i] = input[cur];
		i++;
	}
	commands[parse] = parse_args(charfind(final, ' ') + 1, final);
	parse++;
	
	for( ; parse < 21; parse++) {
		commands[parse] = NULL;
	}
	
	return commands;
}

void welcome(char * user) {
	
	printf("%s@Reed_Shell: Enter Command -> ", user);

}

/*======== find_redir ==========
Inputs: The parsed @commands (separated by semicolon), and @i, the element in commands that is currently being used by the program
Returns: A redir struct containing information about the location and type of the redirection symbols (< and >)
====================*/
struct redir * find_redir(char *** commands, int i) {
	
	int redirect_arg = -1;
	int redirect_position = -1;
	char * redirect_type = NULL;
	int skip = 0;
	for(int j = 0; j < 21 && commands[i][j] != NULL; j++) {
		for(int k = 0; k < strlen(commands[i][j]); k++) {
			if(commands[i][j][k] == '<') {
				redirect_type = "<";
				redirect_arg = j;
				redirect_position = k;
				skip = 1;
			}
			else if(commands[i][j][k] == '&') {
				if(k + 1 < strlen(commands[i][j]) && commands[i][j][k + 1] == '>') {
					if(k + 2 < strlen(commands[i][j]) && commands[i][j][k + 2] == '>') {
						redirect_type = "&>>";
					}
					else {
						redirect_type = "&>";
					}
				}
				redirect_arg = j;
				redirect_position = k;		
				skip = 1;			
			}
			else if(commands[i][j][k] == '2') {
				if(k + 1 < strlen(commands[i][j]) && commands[i][j][k + 1] == '>') {
					if(k + 2 < strlen(commands[i][j]) && commands[i][j][k + 2] == '>') {
						redirect_type = "2>>";
					}
					else {
						redirect_type = "2>";
					}
				}
				redirect_arg = j;
				redirect_position = k;	
				skip = 1;					
			}
			else if(commands[i][j][k] == '>') {
				if(k + 1 < strlen(commands[i][j]) && commands[i][j][k + 1] == '>') {
					redirect_type = ">>";
				}
				else {
					redirect_type = ">";
				}
				redirect_arg = j;
				redirect_position = k;	
				skip = 1;					
			}
			if(skip == 1) {
				goto END_LOOP;
			}
		}	
	}
	END_LOOP: ;
		
	struct redir *ret = malloc(sizeof(struct redir));
	ret -> arg = redirect_arg;
	ret -> position = redirect_position;
	ret -> type = redirect_type;
	
	return ret;
	
}

/*======== redir ==========
Inputs: The @args of the current command, the redirect @type, and the @file that is being accessed with the redirection
Returns: N/A, but it does exec the program for you depending on the type of redirection used
====================*/
void redir(char ** args, char * type, char * file) {

	int fd;
	if(strcmp(type, ">") == 0) {
		remove(file);
		fd = open(file, O_CREAT | O_WRONLY, 0666);
		dup2(fd, STDOUT_FILENO);
		execvp(args[0], args);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if(strcmp(type, ">>") == 0) {
		fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0666);
		dup2(fd, STDOUT_FILENO);
		execvp(args[0], args);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if(strcmp(type, "2>")) {
		remove(file);
		fd = open(file, O_CREAT | O_WRONLY, 0666);
		dup2(fd, STDERR_FILENO);
		execvp(args[0], args);
		dup2(fd, STDERR_FILENO);
		close(fd);
	}
	else if(strcmp(type, "2>>")) {
		fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0666);
		dup2(fd, STDERR_FILENO);
		execvp(args[0], args);
		dup2(fd, STDERR_FILENO);
		close(fd);
	}
	else if(strcmp(type, "&>")) {
		remove(file);
		fd = open(file, O_CREAT | O_WRONLY, 0666);
		char *original = strerror(errno);
		execvp(args[0], args);
		if(strcmp(original, strerror(errno))) {
			write(fd, strerror(errno), strlen(strerror(errno)));
		}
		close(fd);
	}
	else if(strcmp(type, "&>>")) {
		fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0666);
		char *original = strerror(errno);
		execvp(args[0], args);
		if(strcmp(original, strerror(errno))) {
			write(fd, strerror(errno), strlen(strerror(errno)));
		}
		close(fd);
	}
	else if(strcmp(type, "<") == 0) {
		fd = open(file, O_CREAT | O_RDONLY);
		dup2(fd, STDIN_FILENO);
		execvp(args[0], args);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	
}

/*======== execreedvp ==========
Inputs: The @args of the current command being used, and a redir struct, @redir_info , containing redirect information about that command
Returns: N/A, but it does run the redirection using redir()
====================*/
void execreedvp(char ** args, struct redir *redir_info) {
	
	int length = 0;
	while (args[length] != NULL) {
		length++;
	}
	char **args_cpy = malloc(length * (sizeof(*args_cpy) + 2));
	for(int j = 0; j < length; j++) {
  	  args_cpy[j] = strdup(args[j]);
	}
	
	char *file = args[(redir_info -> arg) + 1];
	if(redir_info -> arg == -1) {
		execvp(args_cpy[0], args_cpy);
	}
	else {
		if(redir_info -> position != 0) {
			goto END;
		}
		for(int i = redir_info -> arg; i < length; i++) {
			args_cpy[i] = NULL;
		}
		redir(args_cpy, redir_info -> type, file);
	}
	
	END: ;
}