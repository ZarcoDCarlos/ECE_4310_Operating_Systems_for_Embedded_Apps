#include <stdio.h> 
#include <sys/types.h> //libraries for fork
#include <unistd.h> //fork
#include <unistd.h> // for exec: execute a file
#include <sys/wait.h> // for wait: function waits for a specific process and reports back the status when it is completed
#include <string.h>
#include <stdlib.h> // for exit stuff

#define Max_Command_Size 256 // 256 character max for commands
#define Max_Argument_Size 5 //no more than 5 arguments
#define Max_Para 10 // max number of parameters


int main () {

    char* command[Max_Para];   //command array: each element of the array is a pointer to a string
    while (1) {

        //1. read input using fgets
        printf("\nzarcodcarl.shell>> ");
        int max_characters = 50;
        char input[50]; //max of 50 characters can be read from the user

        // 1. Read input from the user
        fgets(input, max_characters, stdin); // 1st arg: the character array with text from the user, 2nd arg is the max size to be read, 
                                             // 3rd arg. stdin (standard input stream): it is a pointer to the stdin, allows you to read input 

        //printf("\nYou typed the following: %s", input);


        //2. token 
                                             // token = a pointer to the string of the input from the user
        char* token = strtok(input, " \n");  // 1st arg. string to separate into tokens, 2nd arg. Delimiters are space and newline

        int arg_count = 0; //counts # of arguments after the command

        //Initialize the array because the command array will be in a known safe state before storing the tokens 
        for (int i = 0; i < Max_Para; i++ ) {
            command[i] = NULL; //NULL = '\0'
        }

        while (token != NULL && arg_count <  Max_Para) {    // if the next token is not null,and we haven't reached the cap on arguments  then  
            command[arg_count] = token; // store the current token into the command array
                                         // get the next token
            token = strtok(NULL, " \n"); // arg 1. string to be tokenized (if NULL it continues where left ogg), arg 2. delimiter
            arg_count++;
            
        }  

        // Ensure the last element of the command array is NULL for execvp
        command[arg_count] = NULL;

        // Test if I can print out the command, and the remaining arguments
        // printf("Command: %s\n", command[0]);
        // printf("Arguments: ");
        // for (int i = 1; i <= arg_count; i++) {
        //     printf("%s ", command[i]);
        // }
        // printf("\n"); 


        // Custom command handling
        if (strcmp(command[0], "exit") == 0) { // compares the string stored in command[0] with the string "exit". command[0]: A pointer to a constant character
            // Custom "exit" command
            printf("Exiting the shell...\n");
            break; // Exit the shell loop
        } else if (strcmp(command[0], "help") == 0) {
            // Custom "help" command
            printf("This assignment took me forever ngl\n");
            printf("Carlos Zarco\nECE 4310 Custom Shell\n");
            printf("Custom shell commands:\n");
            printf(" - exit: Exit the shell\n");
            printf(" - help: Display this help message\n");
        } else {
            //3. Create a Child process and execute
            
            int pid_from_fork = -1; // initialize pid variable to -1 (failed case)
            pid_from_fork = fork(); //fork happens

            int wait_pid = 0;

            //according to manpages results tab, 3 results are possible -1 for failure, 0, for success and random number
            switch(pid_from_fork) {
            case(0):    // 0 is returned in the child, fork succeeded
                //if fork succeeds execute the command
                if (execvp(command[0], command) == -1) { //first argument is the command, second arg is the rest of the arguments from the user
                    perror("execvp"); // prints an error message and indicates the nature of the error
                                        //EXIT_FAILURE is a constant typically used to define a nonzero value
                    exit(EXIT_FAILURE); // when the program tries to terminate, a 0 indicates success and anything else if a fail
                }
                break;

            case (-1): // // -1 returned in the parent (inside the parent), no child created and the call for fork failed
                printf("\nThe fork has failed.");
                perror("fork");
                exit(EXIT_FAILURE);
                break;    
            default:
                //suspend the parent process until the child process is finished
                int state = 0;  //where the termination stauts of the chilld process is stored
                                            // wait for the child process associated with the pid_from_fork
                wait_pid = wait(&state);    // returns process ID of terminated child, -1 returned if error occurs
                break;    
            }
        }       

    }     
}