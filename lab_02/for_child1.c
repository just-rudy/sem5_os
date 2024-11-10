#include <stdio.h>

#define OK 0
#define IO_ERROR 1

int main(void)
{
    double num1, num2, avg;
    int rc = 0;
    printf("%d - Enter two numbers: ", getpid());
    rc = scanf("%lf %lf", &num1, &num2);
    if (rc != 2)
    {
        printf("ERROR: wrong input\n");
        return IO_ERROR;
    }
    avg = (num1 + num2) / 2;
    printf("Average of this two nums: %lf\n", avg);
    return OK;
}