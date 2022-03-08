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