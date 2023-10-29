#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define SHELL_PROMPT "myshell> "

const char* command_help[] = {
	"help - Get list of commands and uses",
	"pwd - Get present working directory",
	"cd - Change the current directory",
	"ls - List files and directories in the current directory",
	"mkdir - Create a new directory",
	"touch - Create new file in current directory",
	NULL

};

void showHelp(char* tokens[]){
	if (tokens[1] == NULL) {
		// Display list of available commands
		for (int i = 0; command_help[i] != NULL; i++) {
			printf("%s\n", command_help[i]);
		
		}
	
	}
}

// Function to handle built-in commands
int handle_builtint_commands(char* tokens[]){
	
	if (strcmp(tokens[0], "exit") == 0) {
		exit(0); // Exit the shell 
	
	}else if (strcmp(tokens[0], "cd") == 0) {
		if (tokens[1] == NULL) {
			// No directory specified; go to user's home directory
			chdir(getenv("HOME"));
		
		}else {
			// Change to the specified directory
			if(chdir(tokens[1]) != 0){
				perror("cd");
			}
		}
		return 1; // Command was handled 
	
	}else if (strcmp(tokens[0], "help") == 0) {
		
		printf("---------- Helpful Commands ----------\n");
		showHelp(tokens);
		return 1;
	
	}
	
	return 0; // Command was not handled
}

int main(){
	
	char command[MAX_COMMAND_LENGTH];
	while (1) {

		printf(SHELL_PROMPT);
		if (!fgets(command, sizeof(command), stdin)) {

			break;
		
		}

		// Tokenize user input
		char* tokens[MAX_COMMAND_LENGTH];
		char* token = strtok(command, " \n");
		int i = 0;

		while (token != NULL) {

			tokens[i] = token;
			i++;
			token = strtok(NULL, " \n");
		
		}

		tokens[i] = NULL;

		// Check if the command is a built-in command
		if (handle_builtint_commands(tokens)) {

			continue; // skip command execution
		
		}

		// Execute external commands
		pid_t child_pid = fork();
		if (child_pid == 0) {
			// Executed in the child process
			execvp(tokens[0], tokens);
			perror("Command execution error");
			exit(1);
		
		}else if (child_pid < 0) {
			perror("Fork error");
			exit(1);
		
		}else {
			
			// Executed in the parent process
			int status;
			waitpid(child_pid, &status, 0);
		}
	
	}
	return 0;
}
