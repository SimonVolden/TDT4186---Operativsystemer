/**
 * @file alarm_clock.c
 * @author Simonevo
 * @author Siguhau
 * @author Andrhae
 * @brief Practical 3
 * @date 2022-04-05
 *
 * A unix-based shell implemented in C.
 *
 */
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

/**
 * @brief Linked list where the background tasks is stored
 */
struct node
{
    int pid;
    char cmd_line[MAX_LIMIT];
    struct node *previous;
    struct node *next;
};

/* Global variables for accessing the linked list of background tasks */
struct node *head = NULL;
struct node *tail = NULL;

/**
 * @brief Checks the status of the exited task and prints it.
 * @param status int from waitpid
 * @param input the task that has been executed.
 */
void check_status(int status, char *input)
{
    /* removes newline from input string */
    input[strcspn(input, "\n")] = 0;

    if (WIFEXITED(status))
    {
        int es = WEXITSTATUS(status);
        printf("Exit status [%s] = %d\n", input, es);
    }
}

/**
 * @brief Adds a node to the linked list
 *
 * @param pid the pid of the process node
 * @param cmd_line the command line of the process node
 */
void add_node(int pid, char *cmd_line)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));

    /* update values */
    new_node->pid = pid;
    strcpy(new_node->cmd_line, cmd_line);

    /* update previous pointer */
    new_node->previous = tail;
    if (tail != NULL)
        tail->next = new_node;
    tail = new_node;

    /* update next pointer */
    new_node->next = NULL;
    if (head == NULL)
    {
        head = new_node;
    }
}

/**
 * @brief Prints all nodes in the linked list,
 * is called by the prompt "jobs"
 */
void print_nodes()
{
    struct node *ptr = head;
    while (ptr != NULL)
    {
        printf("[pid %d] %s\n", ptr->pid, ptr->cmd_line);
        ptr = ptr->next;
    }
}

/**
 *@brief Removes given node from linked list and rebinds the
 * previous and next node together.
 * If next or previous is NULL, head/tail is updated.
 * Frees the node
 */

void remove_node(struct node *remove)
{
    /* checks if first node */
    if (remove->previous != NULL)
    {
        remove->previous->next = remove->next;
    }
    else
    {
        head = remove->next;
    }

    /* checks if last node */
    if (remove->next != NULL)
    {
        remove->next->previous = remove->previous;
    }
    else
    {
        tail = remove->previous;
    }
    free(remove);
}

/**
 * @brief check if processes in list are complete.
 *
 * Goes through all the background processes in the linked list,
 * and checks if the process has terminated.
 * If it terminated, print it's exit status and remove the node.
 */
void check_nodes()
{
    struct node *ptr = head;
    while (ptr != NULL)
    {
        int status;

        /* checks if processes are complete */
        if (waitpid(ptr->pid, &status, WNOHANG))
        {
            check_status(status, ptr->cmd_line);

            remove_node(ptr);
        }
        ptr = ptr->next;
    }
}

/**
 * @brief Parses the given input and adds them to parsedArgs
 *
 * @param str input to parse
 * @param parsedArgs pointer to list of parsed arguments
 */
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

/**
 * @brief Checks if the given input should be executed as a background task
 * @param input string input from the user.
 */
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

/**
 * @brief Executes the command from user
 * @param parsedArgs list of every argument from the user input.
 * @param cwd current working directory.
 * @param input the command given from user.
 */
void execute(char **parsedArgs, char *input)
{
    int background = check_if_background_task(input);
    int has_redirect = 0;

    /* internal commands */
    if (strcmp(parsedArgs[0], "cd") == 0)
    {
        if (parsedArgs[1])
            chdir(parsedArgs[1]);
        return;
    }
    if (strcmp(parsedArgs[0], "jobs") == 0)
    {
        print_nodes();
        return;
    }

    /* I/O redirection */
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
        /* child process */
        if (has_redirect == 1)
        {
            execl("/bin/sh", "/bin/sh", "-c", input, (char *)0);
        }
        else
        {
            execvp(parsedArgs[0], parsedArgs);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        /* parent process */
        if (background)
        {
            add_node(pid, input);
        }
        else
        {
            waitpid(pid, &status, 0);
            check_status(status, input);
        }
    }
}

/**
 * @brief runs the shell in a loop. Gets the input from user and
 * calls the execute function. If background task is finished,
 * it is shown before the next prompt is given.
 */
int main(void)
{
    char cwd[MAX_LIMIT];
    char input[MAX_LIMIT];
    char inputString[MAX_LIMIT];
    char *parsedArgs[MAX_LIMIT];

    while (1)
    {
        /* checks background processes */
        check_nodes();

        /* gets current working directory and asks for input */
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("\033[1;31m%s: \033[0m", cwd);
            // printf("%s: ", cwd);
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

        /* parses input into arguments */
        parseString(input, parsedArgs);
        for (int i = 0; i < MAX_LIMIT; i++)
        {
            if (!parsedArgs[i])
                break;
        }

        /* executes the command */
        execute(parsedArgs, inputString);
    }
}
