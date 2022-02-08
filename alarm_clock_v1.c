/* localtime example */
/* #include <stdio.h>
#include <time.h>
int main(void)
{
    time_t now, alarm_time;
    struct tm *timeinfo;
    alarm_time = time(&now) + 3600;
    timeinfo = localtime(&alarm_time);
    printf("%ld\n", time(&now));
    printf("Current local time and date: %s", asctime(timeinfo));
    return 0;
}
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_LIMIT 20

int clear(void)
{
    while ((getchar()) != '\n')
        ;
    return 0;
}

int append_word(const char **words, size_t size, const char *word)
{
    size_t i = 0;
    while (i < size && words[i])
    {
        ++i;
    }
    if (i < size)
    {
        words[i] = word;
        return 1;
    }
    else
    {
        return 0;
    }
}

int main(void)
{
    // const char T[] = "2022-02-04 15:00:15";
    const char *alarms[100] = {};
    char T[MAX_LIMIT];
    struct tm *timeinfo;
    time_t result = 0;
    time_t now;
    double diff_t;
    now = time(NULL);

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
        case (115):

            printf("Answer was s \n");

            clear();
            printf("Schedule alarm at which date and time? ");

            // scanf("%19s", T);
            fgets(T, MAX_LIMIT, stdin);
            printf("%s \n", T);

            now = time(NULL);

            int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;

            if (sscanf(T, "%4d-%2d-%2d %2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) == 6)
            {
                struct tm breakdown = {0};
                breakdown.tm_year = year - 1900; /* years since 1900 */
                if (month > 12 || month < 0)
                {
                    fprintf(stderr, "Could not convert time input to time_t\n");
                    break;
                }
                breakdown.tm_mon = month - 1;

                if (month == 2)
                {
                    if (day > 29)
                    {
                        fprintf(stderr, "Could not convert time input to time_t\n");
                        break;
                    }
                }
                else if (month == (1 || 3 || 7 || 8 || 10 || 12))
                {
                    if (day > 31)
                    {
                        fprintf(stderr, "Could not convert time input to time_t\n");
                        break;
                    }
                }
                else
                {
                    if (day > 30)
                    {
                        fprintf(stderr, "Could not convert time input to time_t\n");
                        break;
                    }
                }

                breakdown.tm_mday = day;
                if (hour > 24 || hour < 0)
                {

                    fprintf(stderr, "Could not convert time input to time_t\n");
                    break;
                }
                breakdown.tm_hour = hour;
                if (min > 59 || min < 0)
                {
                    fprintf(stderr, "Could not convert time input to time_t\n");
                    break;
                }
                breakdown.tm_min = min;
                if (sec > 59 || sec < 0)
                {
                    fprintf(stderr, "Could not convert time input to time_t\n");
                    break;
                }
                breakdown.tm_sec = sec;

                if ((result = mktime(&breakdown)) == (time_t)-1)
                {
                    fprintf(stderr, "Could not convert time input to time_t\n");
                    break;
                }
                puts(ctime(&result));

                diff_t = difftime(result, now);

                if (diff_t < 0)
                {
                    printf("Difference: %f is negative\n", diff_t);
                    fprintf(stderr, "The input was not a valid time format\n");
                    break;
                }
                else
                {
                    printf("Difference: %f\n", diff_t);
                    append_word(alarms, 100, T);
                    /* number_of_alarms++;
                    printf("%s \n", alarms[number_of_alarms - 1]); */

                    clear();
                    break;
                    /* size_t n = sizeof(alarms) / sizeof(alarms[0]);
                    printf("%lu", n);
                    return EXIT_SUCCESS; */
                }
            }

            else
            {
                fprintf(stderr, "The input was not a valid time format\n");
                clear();
                break;
            }
            clear();

            break;

        case (108):
            // printf("Answer was l \n");

            number_of_alarms = 0;
            while (number_of_alarms < 100 && alarms[number_of_alarms])
            {
                printf("%s \n", alarms[number_of_alarms]);
                ++number_of_alarms;
            }

            clear();
            break;
        case (99):
            printf("Answer was c \n");

            clear();
            break;
        case (120):
            printf("Exiting... \n");

            clear();
            x = 0;
            break;
        default:

            clear();
            printf("Error!");
        }
    }
}