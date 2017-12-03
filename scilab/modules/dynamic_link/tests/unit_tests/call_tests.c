#include <stdio.h>

#define M_PI 3.14159265358979323846

//call_lhs_rhs

void call_0_0(void)
{
    printf("call_0_0\n");
}

void call_i1_0(int* o1)
{
    printf("call_i1_0\n");
    *o1 = 42;
}

void call_d1_0(double* o1)
{
    printf("call_d1_0\n");
    *o1 = M_PI;
}

void call_0_i1(int* i1)
{
    printf("call_0_i1: %d\n", *i1);
}

void call_0_d1(double* d1)
{
    printf("call_0_d1: %f\n", *d1);
}

void call_i1_i1(int* io1)
{
    printf("call_i1_i1: %d\n", *io1);
    *io1 *= 2;
}

void call_d1_d1(double* io1)
{
    printf("call_i1_i1: %f\n", *io1);
    *io1 *= 2;
}

void call_i1_i1_bis(int* i1, int* o2)
{
    printf("call_i1_i1: %d\n", *i1);
    *o2 = *i1 * 2;
}

void call_d1_d1_bis(double* i1, double* o2)
{
    printf("call_i1_i1: %d\n", *i1);
    *o2 = *i1 * 2;
}

void call_i1_d1(int* i1, double* o2)
{
    printf("call_i1_i1: %d\n", *i1);
    *o2 = *i1 * M_PI;
}

void call_0_s1(char* s1)
{
    printf("call_0_s1: %s\n", s1);
}

void call_s1_0(char* s1)
{
    int size = 10;
    int i = 0;
    for (i = 0; i < size; ++i)
    {
        s1[i] = 'a' + i;
    }

    s1[size] = '\0';
    
    printf("call_s1_0: %s\n", s1);
}

void call_s1_s1(char* s1)
{
    printf("call_s1_s1: %s -> ", s1);
    int i = 0;
    for(i = 0; i < 10; ++i)
    {
        s1[i] += 1;
    }

    printf("%s\n", s1);
}

