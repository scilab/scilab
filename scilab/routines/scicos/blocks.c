#include <stdio.h>
#include <math.h>

#include "../machine.h"
#include "../calelm/calelm.h"

#include "scicos.h"
 
/*------------------------------------------------
 *     Scicos block simulator 
 *     A set of elementary blocks 
 *------------------------------------------------*/

#define Abs(x) ( (x) > 0) ? (x) : -(x)
#define Min(x,y)	(((x)<(y))?(x):(y))
#define Max(x,y)	(((x)>(y))?(x):(y))

typedef  void (scicos0_block) __PARAMS((ARGS_scicos0));
extern scicos0_block F2C(absblk), F2C(andlog), F2C(bidon), F2C(gain);
extern scicos0_block F2C(cdummy), F2C(dband), F2C(cosblk);

/*------------------------------------------------
 *     Scicos block simulator 
 *     returns Absolute value of the input 
 *------------------------------------------------*/


void C2F(absblk)(flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
     integer *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
     double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{
  int i;
  if ( C2F(dbcos).idb == 1) 
    fprintf(stderr,"absblt t=%f, flag=%d\n", *t,*flag);
  for (i = 0 ; i <  *nu ; ++i ) y[i] = Abs(u[i]);
} 


/*------------------------------------------------
 *     Scicos block simulator 
 *     Logical and block
 *     if event input exists synchronuously, output is 1 else -1
 *------------------------------------------------*/

void C2F(andlog)(flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
     integer *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
     double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{
  if ( C2F(dbcos).idb == 1) 
    fprintf(stderr,"Andlog t=%f, flag=%d\n", *t,*flag);
  if ( *flag == 1)  y[0] = ( *nevprt != 3 ) ? -1.00 :  1.00; 
}


/*------------------------------------------------
 *     Scicos block simulator 
 *     does nothing 
 *------------------------------------------------*/

void C2F(bidon)(flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
     integer *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
     double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{
  if ( C2F(dbcos).idb == 1) 
    fprintf(stderr,"Bidon \n");
}



/*------------------------------------------------
 *     Scicos block simulator 
 *     input to output Gain
 *     rpar=gain matrix
 *------------------------------------------------*/

void C2F(gain)(flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
     integer *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
     double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{
  integer un=1;
  if ( C2F(dbcos).idb == 1) 
    fprintf(stderr,"Gain t=%f, flag=%d\n", *t,*flag);
  C2F(dmmul)(rpar,ny,u,nu,y,ny,ny,nu,&un);
}

/*------------------------------------------------
 *     Scicos block simulator 
 *     Dummy state space x'=0
 *------------------------------------------------*/

void C2F(cdummy)(flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
     integer *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
     double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{
  if ( C2F(dbcos).idb == 1) 
    fprintf(stderr,"Cdummy t=%f, flag=%d\n", *t,*flag);
  if ( *flag == 0 ) xd[0]=0.00;
}

/*------------------------------------------------
 *     Scicos block simulator 
 *     Dead Band, 
 *     if u(i)<0 ,y(i)=min(0,u+DB(i)/2) 
 *     else       y(i)=max(0,u-DB(i)/2) 
 *     DB(i)=rpar(i) 
 *------------------------------------------------*/

void C2F(dband)(flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
     integer *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
     double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{
  int i;
  
  for ( i=0 ; i < *nu ; i++ ) 
    {
      if ( u[i] < 0 ) 
	y[i] = Min(0.00,u[i]+rpar[i]/2.00);
      else  
	y[i] = Max(0.00,u[i]-rpar[i]/2.00);
    }
}


/*------------------------------------------------
 *     Scicos block simulator 
 *     
 *------------------------------------------------*/

void C2F(cosblk)(flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
     integer *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
     double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{

  int i;
  for ( i=0; i < *nu ; i++)     y[i]= cos(u[i]);
}
