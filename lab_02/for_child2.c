#include <stdio.h>
#include <math.h>

#define OK 0
#define IO_ERROR 1
#define MAX_ANGLE 360
#define PI 3.1415
#define DEG_TO_RAD_MULT (PI/180)

int main(void)
{
    double a, b, c, alfa, s;
    int rc;

    printf("%d - Enter two sides of the triangle and the angle between them: ", getpid());
    rc = scanf("%lf %lf %lf", &a, &b, &alfa);
    if (rc != 3)
    {
        printf("ERROR: wrong input\n");
        return IO_ERROR;
    }
    if ((a < 0) || (b < 0))
    {
        printf("ERROR: side of a triangle can't be smaller than zero\n");
        return IO_ERROR;
    }
    if (alfa < 0)
    {
        printf("ERROR: angle of a triangle can't be smaller than zero\n");
        return IO_ERROR;
    }
    if (alfa > MAX_ANGLE)
    {
        printf("ERROR: angle of a triangle can't be greater than %d degrees\n", MAX_ANGLE);
        return IO_ERROR;
    }

    s = (a*b*sin(alfa*DEG_TO_RAD_MULT))/2;
    printf("Area of the triangle: %f\n", s);
    return OK;
}