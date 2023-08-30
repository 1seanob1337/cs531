#include <stdio.h>
#include <string.h>
int main(void)
{
    int str[12]={12,1,2,3,4,5,6,7,8,9,10,0};
    int *str_ptr;
    // strcpy(str, "hello world");
    str_ptr = str; // note: an array name serves as a "static" pointer
    while (*str_ptr)
    {
        printf("%i has ascii (Hex) value: %x and is stored in mem location %p \n",
               *str_ptr, *str_ptr, str_ptr);
        str_ptr++;
    }
    return 0;
}