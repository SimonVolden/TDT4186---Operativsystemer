
#include "utilities.h"

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

/**
 * @brief checks if the given month and day are valid
 *
 * @param month
 * @param day
 * @return 1 if invalid, 0 if valid
 */
int check_month_and_day(int month, int day)
{
    if (month > 12 || month < 0)
    {
        return 1;
    }
    if (month == 2)
    {
        if (day > 29)
        {
            return 1;
        }
    }
    else if (month == (1 || 3 || 7 || 8 || 10 || 12))
    {
        if (day > 31)
        {
            return 1;
        }
    }
    else
    {
        if (day > 30)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief checks if the given time format is valid
 *
 * @param result
 * @param T
 * @return 1 if invalid, 0 if valid
 */
int check_format_time(time_t *result, const char *T)
{
    int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;

    if (sscanf(T, "%4d-%2d-%2d %2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) == 6)
    {
        struct tm breakdown = {0};
        breakdown.tm_year = year - 1900; /* years since 1900 */
        breakdown.tm_mon = month - 1;
        breakdown.tm_mday = day;
        breakdown.tm_hour = hour;
        breakdown.tm_min = min;
        breakdown.tm_sec = sec;

        if (check_month_and_day(month, day))
        {
            return 1;
        }

        if (hour > 24 || hour < 0)
        {
            return 1;
        }
        if (min > 59 || min < 0)
        {
            return 1;
        }
        if (sec > 59 || sec < 0)
        {
            return 1;
        }

        if ((*result = mktime(&breakdown)) == (time_t)-1)
        {
            return 1;
        }
        puts(ctime(result));
        return 0;
    }
    else
    {
        return 1;
    }
}