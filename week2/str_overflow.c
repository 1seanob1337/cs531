#include <stdio.h>
#include <string.h>
int main(void)
{
    char str[10];
    strcpy(str, "hello world");
    for (int i = 0; str[i] != '\0'; i++)
    {
        printf("%c is stored in memory location %p \n", str[i], &str[i]);
    }
    return 0;
}