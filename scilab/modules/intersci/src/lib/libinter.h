#ifndef LIBINTER_SCI
#define LIBINTER_SCI

#include "machine.h"
#include "stack-c.h"

void C2F(out) __PARAMS ((char *str,unsigned int));
void C2F(ccharf) __PARAMS (( int *n, char **ip, int *op));
void C2F(cdoublef) __PARAMS (( int *n, double *ip[], double *op));
void C2F(cintf) __PARAMS (( int *n, int *ip[], double *op));
void C2F(cfloatf) __PARAMS (( int *n, float *ip[], double *op));
void C2F(cboolf) __PARAMS (( int *n, int *ip[], int *op));
void Cout __PARAMS ((char *str));
void C2F(dbl2cdbl) __PARAMS ((int *n,double *ip[], double *op));
void C2F(int2cint) __PARAMS (( int *n, integer *ip[], integer *op));
int C2F(csparsef)  __PARAMS ((SciSparse **x,int *mnel,int *icol,double *R,double *I));
SciSparse *NewSparse __PARAMS((int *,int *,int *,int *));
void FreeSparse(SciSparse *x);
void FreeRhsSVar(char **ptrStr);

#endif
