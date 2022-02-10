#pragma once
#include <time.h>
#include <stdio.h>

int clear(void);
int check_month_and_day(int month, int day);
int check_format_time(time_t *result, const char *T);

struct tm *timeinfo;