
#include <stdio.h>
#include <stdlib.h>
#define MAX_LIMIT 20

int main(void)
{
    char name[MAX_LIMIT];
    fgets(name, MAX_LIMIT, stdin);
    printf("%s", name);
    return 0;
}
