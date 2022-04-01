
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_LIMIT 200

/**
 * @brief clears io stream
 *
 * @return void
 */
int clear(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
    return 0;
}

void parseString(char *str, char **parsedArgs)
{
    char delim[4] = "\t \n";

    for (int i = 0; i < MAX_LIMIT; i++)
    {
        parsedArgs[i] = strsep(&str, delim);
        if (parsedArgs[i] == NULL)
            break;
        if (strlen(parsedArgs[i]) == 0)
            i--;
    }
}

void execute(char **parsedArgs)
{
    if (strcmp(parsedArgs[0], "cd") == 0)
    {
        if (parsedArgs[1])
            chdir(parsedArgs[1]);
        return;
    }

    int status;
    pid_t pid = fork();
    if (pid == 0)
    {
        // child
        execvp(parsedArgs[0], parsedArgs);
    }
    else
    {
        // parent
        waitpid(pid, &status, 0);
        // add status
    }
}

int main(void)
{
    char cwd[MAX_LIMIT];
    char input[MAX_LIMIT];
    char *parsedArgs[MAX_LIMIT];
    while (1)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s: ", cwd);
        }
        else
        {
            perror("getcwd() error\n");
        }
        if (!fgets(input, sizeof(input), stdin))
        {
            printf("Got EOF signal\n");
            break;
        }

        parseString(input, parsedArgs);

        for (int i = 0; i < MAX_LIMIT; i++)
        {
            if (!parsedArgs[i])
                break;
            printf("arg %d: %s\n", i, parsedArgs[i]);
        }
        execute(parsedArgs);
    }
}

/* if (getcwd(cwd, sizeof(cwd)) != NULL)
{
    printf("%s: ", cwd);
}
else
{
    perror("getcwd() error");
}
scanf("%s", input);

char *token = strtok_r(input, " ", &input);

printf("got input: %s\n", token);
token = strtok_r(NULL, " ", &input);
printf("got input: %s\n", token);
// clear(); */

// printf("input: %s", input);

/* char str[] = "test1 test2";

int i = 0;
char *token = strtok(str, ' /');
// char *array[3];

while (token != NULL)
{
    // array[i++] = token;
    token = strtok(NULL, ' /');
    printf("%s\n", token);

    // printf("%s\n", token);
} */

/*         for (i = 0; i < 2; ++i)
        {
            printf("%s\n", array[i]);
        } */

// fgets(input, sizeof(input), stdin);
/*
input[strcspn(input, "\n")] = 0;

// char str[] = "strtok needs to be called several times to split a string/alwdjmalw";

char input_cpy[MAX_LIMIT];
strcpy(input_cpy, input);

int init_size = strlen(input);
char delim[4] = "\t \n"; // NO TOUCHY!!!
char argument[MAX_LIMIT] = "";
char *ptr = strtok(input, delim);
strcpy(argument, ptr);
int counter = 0;
int next_delim = 0;

while (ptr != NULL)
{
    strcpy(arguments[counter], argument);
    printf("'%s'\n", ptr);
    ptr = strtok(NULL, delim);
    counter++;
}

for (int i = 0; i < counter; i++)
{
    next_delim = next_delim + strlen(arguments[i]);

    printf("Counter: %d, Argument: %s, index of delim: %d \n", i, arguments[i], next_delim);
}

printf("PATH: %s \n", arguments[0]);
printf("Arguments: %d \n", counter);
return 0;
*/