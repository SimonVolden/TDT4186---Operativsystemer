#include <stdio.h>

// /**
//  * @brief clears io stream
//  *
//  * @return void
//  */
// int clear(void)
// {
//     while ((getchar()) != '\n')
//         ;
//     return 0;
// }

/**
 * @brief checks if the given month and day are valid
 *
 * @param month
 * @param day
 * @return 1 if invalid, 0 if valid
 */
int check_month_and_day(int month, int day)
{
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