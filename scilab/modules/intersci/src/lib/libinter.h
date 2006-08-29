#ifndef LIBINTER_SCI
#define LIBINTER_SCI

extern void C2F(erro)  __PARAMS ((    char *str, unsigned int));
extern void C2F(out) __PARAMS ((    char *str,unsigned int));
void C2F(cchar) __PARAMS ((     int *n,     char **ip,     int *op));
void C2F(ccharf) __PARAMS ((     int *n,     char **ip,     int *op));
void C2F(cdouble) __PARAMS ((     int *n,     double *ip[],     double *op));
void C2F(cdoublef) __PARAMS ((     int *n,     double *ip[],     double *op));
void C2F(cint) __PARAMS ((     int *n,     int *ip[],     double *op));
void C2F(cintf) __PARAMS ((     int *n,     int *ip[],     double *op));
void C2F(cfloat) __PARAMS ((     int *n,     float *ip[],     double *op));
void C2F(cfloatf) __PARAMS ((     int *n,     float *ip[],     double *op));
void C2F(cbool) __PARAMS ((     int *n,     int *ip[],     int *op));
void C2F(cboolf) __PARAMS ((     int *n,     int *ip[],     int *op));
void cerro    __PARAMS ((    char *str));
void Cout    __PARAMS ((   char *str));
void C2F(cstringf) __PARAMS (( char ***ip,int *sciptr,int *m, int*n,int *max,int *ierr));
int C2F(stringc) __PARAMS ((int *sciptr, char ***cptr,     int *ierr));
void C2F(dbl2cdbl) __PARAMS ((int *n,double *ip[],     double *op));
void C2F(freeptr)   __PARAMS (( double *ip[]));
void C2F(int2cint) __PARAMS ((     int *n,     integer *ip[],     integer *op));
/* int C2F(csparsef) __PARAMS ((Sparse **x,     int *mnel,*icol,     double *xr,double *xi));*/

#endif 
