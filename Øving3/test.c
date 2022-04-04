#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256 // Maximum input length + 2 (2 because of ending "\n" and \0)

// We made a separate run-method since its used in both shell-scripting and normal shell-prompting.
int run(char input[MAX_INPUT_LEN])
{
    char delims[] = " \t\n"; // Tokens split by space or tab character
    int pid;
    char *params[64];
    char *command = strtok(input, delims); // Fetching the command from input
    printf("\033[0;33mCommand:\033[0m %s \t \033[0;33mParameters:\033[0m ", command);
    char *param = strtok(NULL, delims); // Fetching paramaters from input
    int i = 0;

    while (param != NULL)
    { // Iterating through paramaters
        if (!strcmp(param, "<") || !strcmp(param, ">"))
        { // End of parameters, break
            break;
        }
        printf("%s, ", param);
        params[i] = param;
        i++;
        param = strtok(NULL, delims);
    }
    printf("\n");

    // Redirections
    char *in_file = NULL;
    char *out_file = NULL;
    int has_redirected_input = 0;
    int has_redirected_output = 0;
    if (param != NULL && strcmp(param, "<") == 0)
    { // Setting redirection variables for input
        has_redirected_input = 1;
        in_file = strtok(NULL, delims);
        param = strtok(NULL, delims);
        printf("\033[0;33mRedirection-file-input: \033[0m%s \n", in_file);
    }
    if (param != NULL && strcmp(param, ">") == 0)
    { // Setting redirection variables for output
        has_redirected_output = 1;
        out_file = strtok(NULL, delims);
        param = strtok(NULL, delims);
        printf("\033[0;33mRedirection-file-output: \033[0m%s \n", out_file);
    }
    if (param != NULL && strcmp(param, "<") == 0)
    { // Setting redirecting variables, not actually doing redirecting here
        if (has_redirected_input)
        {
            printf("Attempted to redirect input twice, exiting\n");
            exit(1);
        }
        in_file = strtok(NULL, delims);
        param = strtok(NULL, delims);
        printf("\033[0;33mRedirection-file-input: \033[0m%s \n", in_file);
    }

    // TASK D - Internal Commands
    if (strcmp(command, "cd") == 0 || strcmp(command, "CD") == 0)
    { // internal command to change working directore
        if (chdir(params[0]) != 0)
        {
            printf("Error CD-ing, %d\n", errno);
        }
    }
    else if (strcmp(command, "exit") == 0 || strcmp(command, "EXIT") == 0)
    { // internal command to exit shell
        printf("Exiting...\n");
        exit(EXIT_SUCCESS);
    }
    // TASK B and C
    else
    {                 // external command -- creating child prosess for execution
        pid = fork(); // Creating child prosess
        if (pid > 0)  // Parent process
        {
            wait(NULL);
        }
        else if (pid == 0) // Child process
        {
            char *argv[i + 2];
            argv[0] = command; // first element of argv is always the command so that commandoes that dont have parameters work
            for (int c = 0; c < i; c++)
            { // making list argv out of parameters
                argv[c + 1] = params[c];
            }
            argv[i + 1] = NULL; // last element must be NULL

            if (in_file != NULL)
            {                                        // redirect if inputfile is given
                int input = open(in_file, O_RDONLY); // reading in_file with open
                if (input == -1)
                { // if return-value of open is -1 an error occured
                    printf("Error: InputFile can not be read or doesn't exist\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    dup2(input, 0); // setting standard input to in_file (this is where the redirecting happens)
                    close(input);   // closing input-file
                }
            }
            if (out_file != NULL)
            { // redirect output if outputfile is given
                // printf("INNE HER\n");
                int output = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR); // FLAGS FOUND HERE: http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
                printf("ERROR: %d", errno);
                if (output == -1)
                { // if return-value of open is -1 an error occured
                    printf("ERROR: Outfile cannot be opened - Showing Result in shell instead\n");
                }
                else
                {
                    dup2(output, 1); // setting standard output to out-file (this is where the redirecting happens)
                    close(output);   // closing output-file
                }
            }
            if (execvp(command, argv) < 0)
            { // executing command with given parameters (returns negative number if error has occured)
                printf("An error has occured. Error type: %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }
    }
}
// main holds input in argv to support shell scripting from ubuntu
int main(int argc, char *argv[])
{

    char *input = malloc(MAX_INPUT_LEN);

    if (input == NULL)
    {
        printf("No memory\n");
        return 1;
    }
    char cwd[PATH_MAX];

    if (argc > 1)
    { // Execute shell script
        FILE *stream;
        stream = fopen(argv[1], "r"); // Opening file with fopen, only for reading
        char *line = NULL;            // Initializing line-variable
        size_t lenght = 0;
        ssize_t readline;
        if (stream == NULL)
        {
            printf("Error: shell script can not be read or doesn't exist\n");
            exit(EXIT_FAILURE);
        }

        while ((readline = getline(&line, &lenght, stream)) != -1)
        {              // Reading line for line from input-stream
            run(line); // Executing line with the shells run-method
        }
        free(line);     // Deallocating memory
        fclose(stream); // Closing input-stream
    }
    else
    { // Run shell as usual
        while (1)
        {
            getcwd(cwd, sizeof(cwd));                    // Updating CWD
            printf("\033[1;31mwish:<%s>$ \033[0m", cwd); // Our shell prompt prints the current path of the shell in red.
            fgets(input, MAX_INPUT_LEN, stdin);          // Fgets returns 0 on error TODO: error handling
            run(input);                                  // Call the run-method with current input
        }
    }
}
