#include <stdio.h> //me: gives access to printf()
#include <stdlib.h> //me: functions involved in memory allocation like malloc()
#include <unistd.h> //me: access to NULL pointer
#include <string.h> //me: access to str functions like strlen()

#define PROMPT "lambda-shell$ " //me: everytime PROMPT is used it is replaced by "lambda-shell$" in prporcessing before compilation

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 1  // Set to 1 to turn on some debugging output, or 0 to turn off

/**
 * Parse the command line.
 *
 * YOU DON'T NEED TO MODIFY THIS!
 * (But you should study it to see how it works)
 *
 * Takes a string like "ls -la .." and breaks it down into an array of pointers
 * to strings like this:
 *
 *   args[0] ---> "ls"
 *   args[1] ---> "-la"
 *   args[2] ---> ".."
 *   args[3] ---> NULL (NULL is a pointer to address 0)
 *
 * @param str {char *} Pointer to the complete command line string.
 * @param args {char **} Pointer to an array of strings. This will hold the result.
 * @param args_count {int *} Pointer to an int that will hold the final args count.
 *
 * @returns A copy of args for convenience.
 */
char **parse_commandline(char *str, char **args, int *args_count)
{
    char *token;
    
    *args_count = 0; //me: pointer to string that is currently set to 0

    token = strtok(str, " \t\n\r"); // me: strtok() splits a string with delimenna \n is the new line character ,it moves the cursor to starting of next line \t is the horizontal tab character,it moves the cursor a tab width \r = carriage return

    while (token != NULL && *args_count < MAX_TOKENS - 1) {
        args[(*args_count)++] = token;
        token = strtok(NULL, " \t\n\r"); //me: token = split string
    }

    args[*args_count] = NULL; //me: NULL terminator in a string

    return args;
}

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS];

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Print a prompt
        printf("%s", PROMPT);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin);

        // Exit the shell on End-Of-File (CRTL-D)
        if (feof(stdin)) {
            break;
        }

        // Parse input into individual arguments
        parse_commandline(commandline, args, &args_count);

        if (args_count == 0) {
            // If the user entered no commands, do nothing
            continue;
        }

        // Exit the shell if args[0] is the built-in "exit" command
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args_count !=2) {
                printf("usage: cd dirname\n");
                continue;
            }

            if (chdir(args[1]) < 0) {
                fprintf(stderr, "failed to switch directory to %s\n", args[1]);
                continue;
            }

            continue;
        }
        #if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]
        for (int i = 0; args[i] != NULL; i++) {
            printf("%d: '%s'\n", i, args[i]);
        }

        #endif
        
        /* Add your code for implementing the shell's logic here */
        pid_t child_pid = fork();

        if (child_pid < 0) {
            fprintf(stderr, "Fork failed\n");
            continue;
        }

        if (child_pid == 0) {
            //in child process's context
            execvp(args[0], args);

            fprintf(stderr, "Exec failed\n");
            exit(1);
        } else {
            // in the parent's context
            wait(NULL);
        }
    }

    return 0;
}