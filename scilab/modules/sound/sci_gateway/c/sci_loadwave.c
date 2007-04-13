#include "gw_sound.h"
#include "stack-c.h"
#include "sox.h" 
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,long int nams_len));  
/*-----------------------------------------------------------------------------------*/ 
/* FILENAME_MAX is set to 14 on hp */
#ifdef hppa 
#undef FILENAME_MAX
#define FILENAME_MAX 4096 
#endif 
/*-----------------------------------------------------------------------------------*/ 
static char filename[FILENAME_MAX];
static int out_n;
static long int lout;
/*-----------------------------------------------------------------------------------*/ 
/* SCILAB function : loadwave */
/*-----------------------------------------------------------------------------------*/ 
int sci_loadwave(char *fname,unsigned long fname_len)
{
  WavInfo Wi;
  int m1,n1,l1,m2=1,n2,nn2,l2,err=0,un=1,eight=8,l3,m4,n4,l4;
  Nbvars=0;
  CheckRhs(1,1);
  CheckLhs(1,3);
  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*** first call to get the size **/
  lout=FILENAME_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  C2F(loadwave)(filename,(double *) 0,&n2,0,&Wi,&err);
  if (err >  0)
	{
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  /* using channels */
  m2 =Wi.wChannels;
  nn2 = n2/m2;
  CreateVar(Rhs+2,"d",&m2,&nn2,&l2);
  CreateVar(Rhs+3,"d",&un,&eight,&l3);
  
  *stk(l3)   = Wi.wFormatTag;	/* data format */
  *stk(l3+1) = Wi.wChannels;	/* number of channels */
  *stk(l3+2) = Wi.wSamplesPerSecond; /* samples per second per channel */
  *stk(l3+3) = Wi.wAvgBytesPerSec; /* estimate of bytes per second needed */
  *stk(l3+4) = Wi.wBlockAlign;	/* byte alignment of a basic sample block */
  *stk(l3+5) = Wi.wBitsPerSample; /* bits per sample */
  *stk(l3+6) = Wi.data_length;	/* length of sound data in bytes */
  *stk(l3+7) = Wi.bytespersample; /* bytes per sample (per channel) */

  n4=1;m4=(int)strlen(Wi.wav_format);
  CreateVar(Rhs+4,"c",&m4,&n4,&l4);
  strncpy(cstk(l4),Wi.wav_format,strlen(Wi.wav_format));
  C2F(loadwave)(filename,stk(l2),&n2,1,&Wi,&err);
  if (err >  0)
	{
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  LhsVar(1)= Rhs+2;
  LhsVar(2)= Rhs+3;
  LhsVar(3)= Rhs+4;
  PutLhsVar();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
