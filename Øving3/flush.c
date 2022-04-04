
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LIMIT 200

struct pid_list
{
    int pid;
    char cmd_line[MAX_LIMIT];
    struct pid_list *last;
    struct pid_list *next;
};

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

void check_status(int status, char input[MAX_LIMIT])
{
    /* removes newline from input string */
    input[strcspn(input, "\n")] = 0;

    if (WIFEXITED(status))
    {
        int es = WEXITSTATUS(status);
        printf("Exit status [%s] = %d\n", input, es);
    }
}

int check_if_background_task(char input[MAX_LIMIT])
{
    /* removes newline from input string */
    input[strcspn(input, "\n")] = 0;
    int length = strlen(input) - 1;

    /* checks for '&' and removes it from string */
    int check = input[length] == '&';
    if ((length > 0) && (input[length] == '&'))
    {
        input[length] = '\0';
        length--;
    }

    /* removes trailing whitespaces after removing '&' */
    while (length > -1)
    {
        if (input[length] == ' ' || input[length] == '\t')
            length--;
        else
            break;
        input[length + 1] = '\0';
    }

    return check;
}

void execute(char **parsedArgs, char cwd[], char input[MAX_LIMIT])
{
    int background = check_if_background_task(input);
    int has_redirect = 0;
    if (strcmp(parsedArgs[0], "cd") == 0)
    {
        if (parsedArgs[1])
            chdir(parsedArgs[1]);
        return;
    }

    for (int i = 0; i < MAX_LIMIT; i++)
    {
        if (!parsedArgs[i])
            break;

        printf("%s\n", parsedArgs[i]);
        if (strcmp(parsedArgs[i], "> "))
        {
            has_redirect = 1;
            break;
        }
        if (strcmp(parsedArgs[i], "< "))
        {
            has_redirect = 1;
            break;
        }
    }

    int status;
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
    }

    if (pid == 0)
    {
        // child
        if (has_redirect == 1)
        {
            execl("/bin/sh", "/bin/sh", "-c", input, 0);
        }
        else
        {
            execvp(parsedArgs[0], parsedArgs);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        // parent
        /* TODO: Check if background task, add to list */
        if (background)
        {
            printf("ENDS WITH &\n");
        }
        // else
        // printf("Does not end with &\n");
        waitpid(pid, &status, 0);
        check_status(status, input);
    }
    /*
    /home/user/shelldev: /bin/echo test
    test
    Exit status [/bin/echo test] = 0
    */
}

int main(void)
{
    char cwd[MAX_LIMIT];
    char input[MAX_LIMIT];
    char inputString[MAX_LIMIT];
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
        strcpy(inputString, input); // Make a copy of input string

        parseString(input, parsedArgs);

        for (int i = 0; i < MAX_LIMIT; i++)
        {
            if (!parsedArgs[i])
                break;
        }
        execute(parsedArgs, cwd, inputString);
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