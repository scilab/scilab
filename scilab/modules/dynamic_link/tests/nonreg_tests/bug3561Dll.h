// bug3561Dll.h
#pragma once
#pragma managed(push, off)

#define DllExport   __declspec( dllexport )

extern "C" {
    // Returns a + b
    DllExport void Add(double *a, double *b, double *c);

    // Returns a - b
    DllExport void Subtract(double *a, double *b, double *c);

    // Returns a * b
    DllExport void Multiply(double *a, double *b, double *c);

    // Returns a / b
    // Throws DivideByZeroException if b is 0
    DllExport void Divide(double *a, double *b, double *c);

    DllExport void WriteString(char *c);
}


#pragma managed(pop)