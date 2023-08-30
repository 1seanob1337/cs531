#include <stdio.h>
#include <string.h>
int main()
{
    char strings[20][30];
    int counter;
    for (counter = 0; counter < 10; counter++)
    {
        printf("Enter a string:");
        fgets(strings[counter], 25, stdin);
        int len=strlen(strings[counter]);
        if(strings[counter][len-1] =='\n')
        {
            strings[counter][len-1] ='\0';
        }
    }
    for (counter = 0; counter < 10; counter++)
    {
        printf("%d: %s\n", counter, strings[counter]);
    }
    return 0;
}