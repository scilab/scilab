#include <stdio.h>
#include "../machine.h"
int worldsize(type)
char type[4];
{
  char c;
  int wsz;

  c=type[0];
  if (c=='u') c=type[1];
  switch ( c )
    {
    case 'l' : wsz=sizeof(long);
      break;
    case 's' : wsz=sizeof(short);
      break;
    case 'c' : wsz=sizeof(char);
      break;
    case 'd' : wsz=sizeof(double);
      break;
    case 'f' : wsz=sizeof(float); 
      break;
    }
  return wsz;
}
void 
readc(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	       ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
     /*
     ipar[1]   = lfil : file name length
     ipar[2:4] = fmt  : numbers type ascii code
     ipar[5]   = is it a time record
     ipar[6]   = n : buffer length in number of records
     ipar[7]   = maxvoie : record size
     ipar[8]   = swap
     ipar[9]   = first : first record to read
     ipar[10:9+lfil] = character codes for file name
     ipar[10+lfil:9+lfil++ny+ievt] = reading mask
     */
integer *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar;
integer insz[],*nin,outsz[],*nout;
double x[],xd[],z[],tvec[],rpar[];
double *inptr[],*outptr[],*t;

{
  char str[100],type[4];
  int job = 1,three=3;
  FILE *fd;
  int n, k, ievt, kmax, no, lfil, m, i, irep, nm, ierr;
  double *buffer,*record;
  int *mask;
  long offset;
  

  --ipar;
  --z;
  fd=(FILE *)(long)z[3];
  buffer = (z+4);
  mask = ipar+11+ipar[1];
    
  /*
    k    : record counter within the buffer
    kmax :  number of records in the buffer
  */

  if (*flag==1) {
    n    = ipar[6];
    ievt = ipar[5];
    k    = z[1];
    /* copy current record to output */
    record=buffer+(k-1)*ipar[7]-1;

    for (i=0;i<outsz[0];i++)
      *(outptr[0]+i)=record[mask[ievt+i]];

    if (*nevprt>0) {
      /*     discrete state */
      kmax = z[2];
      if (k>=kmax&&kmax==n) {
	/*     read a new buffer */
	m=ipar[6]*ipar[7];
	F2C(cvstr)(&three,&(ipar[2]),type,&job);
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
  else if (*flag==3) {
    ievt = ipar[5];
    n    = ipar[6];
    k    = z[1];
    kmax = z[2];
    if (k > kmax && kmax < n) {
      if(ievt) 
	tvec[0] = *t-1.0;
      else
	tvec[0] = *t*(1.0+0.0000000001);
    }
    else {
      record=buffer+(k-1)*ipar[7]-1;
      if(ievt) tvec[0] = record[mask[0]];
    }
  }
  else if (*flag==4) {
    F2C(cvstr)(&(ipar[1]),&(ipar[10]),str,&job);
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
      F2C(cvstr)(&three,&(ipar[2]),type,&job);
      for (i=2;i>=0;i--)
	  if (type[i]!=' ') { type[i+1]='\0';break;}
      offset=(ipar[9]-1)*ipar[7]*worldsize(type);
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
    F2C(cvstr)(&three,&(ipar[2]),type,&job);
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
    fclose(fd);
    z[3] = 0.0;
  }
  return;
}

