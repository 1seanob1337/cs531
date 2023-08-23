/*
hello.c -- The most famous program of them all ..
*/
#include <stdio.h>
int main(int argc, char** argv) {
printf("Hello");
printf("%s\n",argv[1]);
return 0;
}