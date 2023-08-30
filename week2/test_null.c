#include <stdio.h>
#include <string.h>
int main()
{
    char c_string[] = "Hello\0";
    printf("%x\n",c_string[0]);
}