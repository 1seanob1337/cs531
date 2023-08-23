/* add2.c -- Add two numbers and print them out together
along with their sum
AUTHOR:
DATE:
*/
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
    int first,second;
    if(argc<3) {
        printf("Enter two integers > ");
        scanf("%d %d", &first, &second);
    }
    else {
        printf("%s\t%s\n",argv[1],argv[2]);
        first=atoi(argv[1]);
        second=atoi(argv[2]);
    }

    printf("The two numbers are: %d %d\n", first, second);
    printf("Their sum is %d\n", first+second);
    return 0;
}