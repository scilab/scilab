#include <stdio.h>
#include <string.h>

#include "../machine.h"

extern  int C2F(cvstr)  __PARAMS((integer *,integer *,char *,integer *,unsigned long int));
extern void sciprint __PARAMS((char *fmt,...));
void mput2  __PARAMS((FILE *fa, integer swap, double *res, integer n, char *type, integer *ierr));

void 
writec(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
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
  char str[100],type[4];
  int job = 1,three=3;
  FILE *fd;
  int n, k,/* m,*/ i, ierr;
  double *buffer,*record;
/*  long offset;*/
  

  --ipar;
  --z;
  fd=(FILE *)(long)z[2];
  buffer = (z+3);
  ierr=0;
  /*
    k    : record counter within the buffer
  */

  if (*flag==2&&*nevprt>0) { /* add a new record to the buffer */
    n    = ipar[5];
    k    = (int)z[1];
    /* copy current record to output */
    record=buffer+(k-1)*(insz[0]);

    for (i=0;i<insz[0];i++)
      record[i] = *(inptr[0]+i);
    if (k<n) 
      z[1] = z[1]+1.0;
    else {/* buffer is full write it to the file */
      F2C(cvstr)(&three,&(ipar[2]),type,&job,(unsigned long)strlen(type));
      for (i=2;i>=0;i--)
	if (type[i]!=' ') { type[i+1]='\0';break;}
      mput2(fd,ipar[6],buffer,ipar[5]*insz[0],type,&ierr);
      if(ierr!=0) {
	*flag = -3;
	return;
      }
      z[1] = 1.0;
    }
  }
  else if (*flag==4) {
    F2C(cvstr)(&(ipar[1]),&(ipar[7]),str,&job,(unsigned long)strlen(str));
    str[ipar[1]] = '\0';
    fd = fopen(str,"wb");
    if (!fd ) {
      sciprint("Could not open the file!\n");
      *flag = -3;
      return;
    }
    z[2]=(long)fd;
    z[1] = 1.0;
  }
  else if (*flag==5) {
    if(z[2]==0) return;
    k    =(int) z[1];
    if (k>=1) {/* flush rest of buffer */
      F2C(cvstr)(&three,&(ipar[2]),type,&job,(unsigned long)strlen(type));
      for (i=2;i>=0;i--)
	if (type[i]!=' ') { type[i+1]='\0';break;}
      mput2(fd,ipar[6],buffer,(k-1)*insz[0],type,&ierr);
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

