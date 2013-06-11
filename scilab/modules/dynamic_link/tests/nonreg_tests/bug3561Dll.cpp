// MathFuncsDll.cpp

#pragma managed(push, off)
#include <iostream>
//#include <math.h>
//#include "string.h"


#include "bug3561Dll.h"



DllExport void Add(double *a, double *b, double *c)
{
    *c = *a + *b;
}

DllExport void Subtract(double *a, double *b, double *c)
{

    *c = *a - *b;
}

DllExport void Multiply(double *a, double *b, double *c)
{

    *c = *a * *b;
}

DllExport void Divide(double *a, double *b, double *c)
{
    if (b == 0)
    {
        std::cout << "invalid_argument b cannot be zero!";
    }
    else
    {

        *c = *a / *b;
    }
}

DllExport void WriteString(char * c)
{

    c[0] = '1';
    c[1] = '2';
    c[2] = '3';
    c[3] = '4';
    c[4] = '5';
    c[5] = 0;
}
