#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../machine.h"

extern void sciprint __PARAMS((char *fmt,...));
void mput2  __PARAMS((FILE *fa, integer swap, double *res, integer n, char *type, integer *ierr));

void 
writeau(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	       ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
integer *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar,insz[],*nin,outsz[],*nout;
double x[],xd[],z[],tvec[],rpar[];
double *inptr[],*outptr[],*t;
     /*
     ipar[1]   = lfil : file name length
     ipar[2:4] = fmt  : numbers type ascii code
     ipar[5]   = n : buffer length in number of records
     ipar[6]   = swap
     ipar[7:6+lfil] = character codes for file name
     */
{
  FILE *fd;
  int n, k,/* m,*/ i, ierr;
  double *buffer,*record;
/*  long offset;*/
  int SCALE  = 32768;
  int BIAS   =   132;
  int CLIP   = 32635;
  int OFFSET =   335;
  double y;
  int sig;
  int e;
  double f;
  

  --ipar;
  --z;
  fd=(FILE *)(long)z[2];
  buffer = (z+3);
  ierr=0;
  /*
    k    : record counter within the buffer
  */

  if (*flag==2&&*nevprt>0) 
    { /* add a new record to the buffer */
      n    = ipar[5];
      k    = (int)z[1];
      /* copy current record to output 
	 printf("%i\n",k);*/
      record=buffer+(k-1)*(*nin); 

      for (i=0;i<*nin;i++)
	{
	  y= *inptr[i];
	  y=SCALE*y;
	  if (y<0.0)
	    {
	      y=-y;
	      sig=-1;
	    }
	  else
	    sig=1;
	  if(y>CLIP)
	    y=CLIP;
	  y=y+BIAS;
	  f=frexp(y,&e);
	  y=64*sig-16*e- (int) (32*f)+OFFSET;
	  record[i] = y;
	}
      if (k<n) 
	z[1] = z[1]+1.0;
      else {
	mput2(fd,ipar[6],buffer,ipar[5]*(*nin),"uc",&ierr); 
	if(ierr!=0) {
	  *flag = -3;
	  return;
	}
	z[1] = 1.0;
	
      }
      
    }
  else if (*flag==4) {
    fd = fopen("/dev/audio","wb");
    if (!fd ) {
      sciprint("Could not open /dev/audio!\n");
      *flag = -3;
      return;
    }
    z[2]=(double)(long)fd;
    z[1] = 1.0;
  }
  else if (*flag==5) {
    if(z[2]==0) return;
    k    =(int) z[1];
    if (k>1) {/* flush rest of buffer */
      mput2(fd,ipar[6],buffer,(k-1)*(*nin),"uc",&ierr);
      if(ierr!=0) {
	*flag = -3;
	return;
      }
    }
    fclose(fd);
    z[2] = 0.0;
  }
  return;
}

