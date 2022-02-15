#include "alarm_clock.h"

int main(void)
{
    char T[MAX_LIMIT];
    struct tm *timeinfo;
    time_t result = 0;
    time_t *result_p = &result;
    time_t now;
    int diff_t;
    now = time(NULL);
    struct Alarm
    {
        time_t time;
        int pid;
        char timestring[MAX_LIMIT];
    };
    struct Alarm alarms[NUMBER_ALARMS] = {0};

    char buffer[80];
    timeinfo = localtime(&now);
    strftime(buffer, 80, "%F %H:%M:%S", timeinfo);

    char answer;
    int x = 1;
    int ch = 0;
    int number_of_alarms = 0;

    printf("Welcome to the alarm clock! It is currently %s \n", buffer);

    while (x)
    {

        printf("Please enter 's' (schedule), 'l' (list), 'c' (cancel), 'x' (exit) \n");
        // scanf("%c", &answer);

        ch = getchar();

        switch (ch)
        {
        case (115): // schedule alarm
            clear();

            printf("Answer was s \n");
            printf("Schedule alarm at which date and time? ");

            // scanf("%19s", T);
            fgets(T, MAX_LIMIT, stdin);
            printf("%s \n", T);

            now = time(NULL);

            if (check_format_time(result_p, T))
            {
                fprintf(stderr, "Could not convert time input to time_t\n");
                break;
            }

            diff_t = (int)difftime(result, now);

            if (diff_t < 0)
            {
                printf("Difference: %d is negative\n", diff_t);
                fprintf(stderr, "The input was not a valid time format\n");
                break;
            }
            else
            {
                printf("Difference: %d\n", diff_t);

                int pid = fork();

                if (pid == 0)
                {
                    sleep(diff_t);
                    printf("alarm! \n");
                    exec();
                    // clear();
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    struct Alarm alarm;
                    alarm.pid = pid;
                    alarm.time = result;
                    strcpy(alarm.timestring, T);
                    alarms[number_of_alarms] = alarm;
                    number_of_alarms++;
                }
                break;
            }
            break;

        case (108):
            printf("Answer was l \n"); // list alarms

            for (int i = 0; i < NUMBER_ALARMS; i++)
            {
                if (alarms[i].time > time(NULL))
                {
                    printf("Alarm %d with PID %d at: %s \n", i + 1, alarms[i].pid, alarms[i].timestring);
                }
            }
            break;

        case (99): // cancel
            printf("Answer was c \n");
            // clear();
            printf("Alarms: \n");
            for (int i = 0; i < NUMBER_ALARMS; i++)
            {
                if (alarms[i].time > time(NULL))
                {
                    printf("Alarm %d with PID %d at: %s \n", i + 1, alarms[i].pid, alarms[i].timestring);
                }
            }

            int pid_to_cancel;
            printf("Give PID of alarm you want to cancel. \n");
            if (scanf("%d", &pid_to_cancel))
            {
                for (int i = 0; i < NUMBER_ALARMS; i++)
                {
                    if (alarms[i].pid == pid_to_cancel)
                    {
                        printf("Deleting alarm %d with PID %d at: %s \n", i + 1, alarms[i].pid, alarms[i].timestring);
                        kill(pid_to_cancel, SIGKILL);
                        alarms[i].time = 1;
                    }
                }
            }
            else
            {
                // handle error
            }

            // printf("%d \n", pid_to_cancel);

            break;
        case (120): // exit
            printf("Exiting... \n");
            x = 0;
            break;
        default: // error, incorrect input
            printf("Error! \n");
            break;
        } // switch
        clear();

        // kills existing zombies
        waitpid(-1, NULL, WNOHANG);
    } // while loop
} // main
