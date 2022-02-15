/**
 * @file alarm_clock.c
 * @author Simonevo
 * @author Siguhau
 * @author Andrhae
 * @brief Practical 1
 * @date 2022-02-15
 *
 * A unix-based alarm clock implemented in C.
 * Some utility functions are placed in the utilities.c file,
 * this is functions like clear input, check correct time format etc.
 * The test documentation task is written in the README.
 *
 */

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
    char ringtones[3][50] = {"ringtone.mp3", "ringtone2.mp3", "bikebell.mp3"};
    int ringtone_index;
    printf("Welcome to the alarm clock! It is currently %s \n", buffer);

    while (x)
    {

        printf("Please enter 's' (schedule), 'l' (list), 'c' (cancel), 'x' (exit) \n");

        ch = getchar();

        switch (ch)
        {
        case (115): // schedule alarm
            clear();

            printf("Answer was s \n");
            printf("Schedule alarm at which date and time? \n");

            fgets(T, MAX_LIMIT, stdin);

            if (check_format_time(result_p, T))
            {
                fprintf(stderr, "Could not convert time input to time_t\n");
                break;
            }

            printf("Select ringtone: {0:'ringtone.mp3', 1:'ringtone2.mp3', 2:'bikebell.mp3'} \n");

            scanf("%d", &ringtone_index);

            if (ringtone_index > 2 || ringtone_index < 0)
            {
                printf("Invalid ringtone, choosing default ringtone \n");
                ringtone_index = 0;
            }
            else
            {
                printf("You have chosen ringtone: %s \n", ringtones[ringtone_index]);
            }

            now = time(NULL);
            diff_t = (int)difftime(result, now);
            printf("Difference: %d\n", diff_t);

            if (diff_t < 0)
            {
                printf("Difference: %d is negative\n", diff_t);
                fprintf(stderr, "The input was not a valid time format\n");
                break;
            }
            else
            {

                int pid = fork();

                if (pid == 0)
                {
                    sleep(diff_t);
                    printf("alarm! \n");
                    // does not work on WSL, works on linux with mpg123 installed
                    execlp("mpg123", "-q", ringtones[ringtone_index], NULL);

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

        case (108): // list alarms
            printf("Answer was l \n");

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
                        alarms[i].pid = 0;
                    }
                }
            }
            else
            {
                // handle error
            }
            break;

        case (120): // exit
            printf("Exiting... \n");
            x = 0;

            for (int i = 0; i < NUMBER_ALARMS; i++)
            {
                if (alarms[i].pid != 0)
                {
                    printf("Deleting alarm %d with PID %d at: %s \n", i + 1, alarms[i].pid, alarms[i].timestring);
                    kill(alarms[i].pid, SIGKILL);
                    alarms[i].time = 1;
                    alarms[i].pid = 0;
                }
            }
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
