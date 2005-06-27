/* copyright Enpc: Jean-Philippe Chancelier  */

#include "../stack-c.h" 
#include "../os_specific/Os_specific.h" /* isanan */ 

#ifdef WIN32 
/* vc++ pbs with nan and comparisons */ 
#define NAN_CHECK 
#endif 

#define less 59
#define great 60
#define equal 50 
/* less + equal */
#define less_equal 109 
/* less + great */
#define less_great 119 
/* great + equal */
#define great_equal 110 

static void   idcmp_equal(double x[],double y[],int *n,int res[]) ; 
static void   idcmp_lessgreat(double x[],double y[],int *n,int res[]) ;
static void   idcmp_less(double x[],double y[],int *n,int res[]) ;
static void   idcmp_great(double x[],double y[],int *n,int res[]) ;
static void   idcmp_lessequal(double x[],double y[],int *n,int res[]) ;
static void   idcmp_greatequal(double x[],double y[],int *n,int res[]) ;

int  C2F(idcmp)(double x[],double y[],int *n,int res[],int *op) 
{
  int i;
  switch (*op) {
  case equal :       idcmp_equal(x,y,n,res) ; break; 
  case less_great :  idcmp_lessgreat(x,y,n,res) ; break; 
  case less :        idcmp_less(x,y,n,res) ; break; 
  case great :       idcmp_great(x,y,n,res) ; break; 
  case less_equal :  idcmp_lessequal(x,y,n,res) ; break; 
  case great_equal : idcmp_greatequal(x,y,n,res) ; break; 
  default: 
    for (i=0; i < *n; i++) res[i]=0;
  }
  return 0; 
}

/* nan pbs with vc++ */ 

static void   idcmp_equal(double x[],double y[],int *n,int res[]) 
{
  int i; 
  for (i=0; i < *n; i++) 
    {
#ifdef NAN_CHECK 
      if ( C2F(isanan)(&x[i])== 1 || C2F(isanan)(&y[i])== 1 ) 
	res[i]= 0; 
      else 
	res[i]= x[i]== y[i]; 
#else 
    res[i]= x[i]== y[i]; 
#endif 
    }
}

static void   idcmp_lessgreat(double x[],double y[],int *n,int res[]) 
{
  int i; 
  for (i=0; i < *n; i++) 
    {
#ifdef NAN_CHECK 
      if ( C2F(isanan)(&x[i])== 1 || C2F(isanan)(&y[i])== 1 ) 
	res[i]= 1; 
      else 
	res[i]= x[i] != y[i]; 
#else 
      res[i]= x[i] != y[i]; 
#endif 
    }
}

static void   idcmp_less(double x[],double y[],int *n,int res[]) 
{
  int i; 
  for (i=0; i < *n; i++) 
    {
#ifdef NAN_CHECK 
      if ( C2F(isanan)(&x[i])== 1 || C2F(isanan)(&y[i])== 1 ) 
	res[i]= 0; 
      else 
	res[i]= x[i] <  y[i]; 
#else 
      res[i]= x[i] <  y[i]; 
#endif 
    }
}


static void   idcmp_great(double x[],double y[],int *n,int res[]) 
{
  int i; 
  for (i=0; i < *n; i++) 
    {
#ifdef NAN_CHECK 
      if ( C2F(isanan)(&x[i])== 1 || C2F(isanan)(&y[i])== 1 ) 
	res[i]= 0; 
      else 
	res[i]= x[i] >  y[i]; 
#else 
      res[i]= x[i] >  y[i]; 
#endif 
    }
}


static void   idcmp_lessequal(double x[],double y[],int *n,int res[]) 
{
  int i; 
  for (i=0; i < *n; i++) 
    {
#ifdef NAN_CHECK 
      if ( C2F(isanan)(&x[i])== 1 || C2F(isanan)(&y[i])== 1 ) 
	res[i]= 0; 
      else 
	res[i]= x[i] <=  y[i]; 
#else 
      res[i]= x[i] <=  y[i]; 
#endif 
    }
}


static void   idcmp_greatequal(double x[],double y[],int *n,int res[]) 
{
  int i; 
  for (i=0; i < *n; i++) 
    {
#ifdef NAN_CHECK 
      if ( C2F(isanan)(&x[i])== 1 || C2F(isanan)(&y[i])== 1 ) 
	res[i]= 0; 
      else 
	res[i]= x[i] >=  y[i]; 
#else 
      res[i]= x[i] >=  y[i]; 
#endif 
    }
}






