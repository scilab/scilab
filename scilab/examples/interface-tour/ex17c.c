#if defined(__STDC__)
#define ARGS_ex17c double, double, double *
#else
#define ARGS_ex17c 
#endif 

typedef int (*funcex)(ARGS_ex17c);
extern int f1c(ARGS_ex17c);
extern int f2c(ARGS_ex17c);

/* function which calculates Z=ex17c(X,Y,f) : given x, y and f, 
   the array z is such that   z[i,j]=f(x[i],y[j]) */

int ex17c(x, nx, y, ny, z, f)
     double *x,*y,*z;
     int nx,ny;
     funcex f;
{
  int i,j;
  for ( i = 0 ; i < nx ; i++ ) 
    for ( j = 0 ; j < ny ; j++) 
	(*f)(x[i], y[j], &z[i+nx*j]);
  return 0;
}

/* Two examples of argument functions f. Scilab usage:
   Z=ex17c(X,Y,"f1c") 
   or
   Z=ex17c(X,Y,"f2c")      */

int f1c(x,y,z) 
     double x,y,*z;
{
  *z= x+y;
  return 0;
}

int f2c(x,y,z) 
     double x,y,*z;
{
  *z= x*x+y*y;
  return 0;
}

