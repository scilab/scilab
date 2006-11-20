#include <stdio.h>
#include <math.h> 
#include <string.h> 
#include "../machine.h"
extern  int C2F(cvstr)  __PARAMS((integer *,integer *,char *,integer *,unsigned long int));
extern void sciprint __PARAMS((char *fmt,...));
extern void mget2 __PARAMS((FILE *fa, integer swap, double *res, integer n, char *type, integer *ierr));
void 
readau(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	       ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
     /*
     ipar[1]   = lfil : file name length
     ipar[2:4] = fmt  : numbers type ascii code
     ipar[5]   = void
     ipar[6]   = n : buffer length in number of records
     ipar[7]   = maxvoie : record size
     ipar[8]   = swap
     ipar[9]   = first : first record to read
     ipar[10:9+lfil] = character codes for file name
     */
integer *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar;
integer insz[],*nin,outsz[],*nout;
double x[],xd[],z[],tvec[],rpar[];
double *inptr[],*outptr[],*t;

{
  char str[100],type[4];
  int job = 1,three=3;
  FILE *fd;
  int n, k, kmax, /*no, lfil,*/ m, i, irep,/* nm,*/ ierr;
  double *buffer,*record;
  long offset;
  double y;
  /*  div_t divt;*/int quot, rem;
  double SCALE=0.000030517578125;
  /*  int ETAB[8]={0, 132, 396, 924, 1980, 4092, 8316, 16764}; */
  int ETAB[8];
  int mu;
  int sig;
  int e;
  int f;
/*  double ff;*/

  ETAB[0]=0; ETAB[1]=132; ETAB[2]= 396; ETAB[3]=924; ETAB[4]=1980;
  ETAB[5]=4092; ETAB[6]=8316; ETAB[7]=16764;

  --ipar;
  --z;
  fd=(FILE *)(long)z[3];
  buffer = (z+4);
    
  /*
    k    : record counter within the buffer
    kmax :  number of records in the buffer
  */

  if (*flag==1) {
    n    = ipar[6];
    k    = (int)z[1];
    /* copy current record to output */
    record=buffer+(k-1)*ipar[7];

    for (i=0;i<*nout;i++)
      {
	mu=(int) record[i];

	mu=255-mu;
	if(mu>127)
	  sig=1;
	else
	  sig=0;
	/* comment out for SUNOS SS 8/10/99 
	divt=div(mu,16);
	e=divt.quot-8*sig+1;
	f=divt.rem;
	*/
	quot=mu/16;rem=mu-16*quot;
	e=quot-8*sig+1;
	f=rem;

	y=ldexp((double)(f),(e+2));
	/* ff=(double)(e+2);
	   y=((double) f) * pow(two, ff); */

	e=ETAB[e-1];

	y=SCALE*(1-2*sig)*(e+y);

	*outptr[i]=y;
      }
    if (*nevprt>0) {
      /*     discrete state */
      kmax =(int) z[2];
      if (k>=kmax&&kmax==n) {
	/*     read a new buffer */
	m=ipar[6]*ipar[7];
	F2C(cvstr)(&three,&(ipar[2]),type,&job,(unsigned long)strlen(type));
	for (i=2;i>=0;i--)
	  if (type[i]!=' ') { type[i+1]='\0';break;}
	ierr=0;
	mget2(fd,ipar[8],buffer,m,type,&ierr);
	if (ierr>0) {
	  sciprint("Read error!\n");
	  fclose(fd);
	  z[3] = 0.0;
	  *flag = -1;
	  return;
	}
	else if (ierr<0) { /* EOF reached */
	  kmax=-(ierr+1)/ipar[7];
	}
	else
	  kmax=ipar[6];

	z[1] = 1.0;
	z[2] = kmax;
      }
      else if (k<kmax) 
	z[1] = z[1]+1.0;
    }
  }
  else if (*flag==4) {
    F2C(cvstr)(&(ipar[1]),&(ipar[10]),str,&job,(unsigned long)strlen(str));
    str[ipar[1]] = '\0';
    fd = fopen(str,"rb");
    if (!fd ) {
      sciprint("Could not open the file!\n");
      *flag = -1;
      return;
    }
    z[3]=(long)fd;
    /* skip first records */
    if (ipar[9]>1) {
      F2C(cvstr)(&three,&(ipar[2]),type,&job,(unsigned long)strlen(type));
      for (i=2;i>=0;i--)
	  if (type[i]!=' ') { type[i+1]='\0';break;}
      offset=(ipar[9]-1)*ipar[7]*sizeof(char);
      irep = fseek(fd,offset,0) ;
      if ( irep != 0 ) 
	{
	  sciprint("Read error\r\n");
	  *flag = -1;
	  fclose(fd);
	  z[3] = 0.0;
	  return;
	}
    }
    /* read first buffer */
    m=ipar[6]*ipar[7];
    F2C(cvstr)(&three,&(ipar[2]),type,&job,(unsigned long)strlen(type));
    for (i=2;i>=0;i--)
	  if (type[i]!=' ') { type[i+1]='\0';break;}
    mget2(fd,ipar[8],buffer,m,type,&ierr);
    if (ierr>0) {
      sciprint("Read error!\n");
      *flag = -1;
      fclose(fd);
      z[3] = 0.0;
      return;
    }
    else if (ierr<0) { /* EOF reached */
      kmax=-(ierr+1)/ipar[7];
    }
    else
      kmax=ipar[6];

    z[1] = 1.0;
    z[2] = kmax;
  }
  else if (*flag==5) {
    if(z[3]==0) return;
    fclose(fd);
    z[3] = 0.0;
  }
  return;
}

