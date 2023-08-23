#include <stdio.h>
int main (void) {
int a = 10;
do {
printf("value of a: %d\n", a);
a = a + 1;
} while( a < 20 );

return 0;
}