#include <stdio.h>
unsigned long long fact(int n);
int main(void) {
int current;
// change to zero since unsigned
printf("Enter a positive integer [to terminate enter zero] > ");
scanf("%d", &current);
while (current > 0) {
printf("The factorial of %d is %llu\n", current, fact(current));
printf("Enter a positive integer [to terminate enter non-positive] > ");
scanf("%d", &current);
}
return 0;
}
/* n is a positive integer. The function returns n factorial */
unsigned long long fact(int n) {
int lcv; /* loop control variable */
unsigned long long p; /* set to the product of the first lcv positive integers */
for (p=1, lcv=2; lcv <= n; lcv++)
{
p *= lcv;
// p = p*lcv;
}
return p;
};
