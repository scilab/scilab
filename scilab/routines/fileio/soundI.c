#include <string.h>

#include "../stack-c.h"
#include "sox.h" 
#include "stdio.h" 

extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,
			        long int nams_len));  

/* FILENAME_MAX is set to 14 on hp */
#ifdef hppa 
#undef FILENAME_MAX
#define FILENAME_MAX 4096 
#endif 


/******************************************
 * SCILAB function : savewave, 
 ******************************************/

char filename[FILENAME_MAX];
int out_n;
long int lin,lout;

int intssavewave(char *fname)
{
  int m1,n1,l1,m2,n2,mn2,l2,m3,n3,l3,l4,err,rate=22050;
  int un=1;
  Nbvars=0;
  CheckRhs(2,3);
  CheckLhs(1,1);
  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*  checking variable res */
  GetRhsVar(2,"d",&m2,&n2,&l2);
  mn2=m2*n2;
  /* Checking variable rate */
  if ( Rhs == 3 )
    {
      GetRhsVar(3,"d",&m3,&n3,&l3);
      rate =(int)(*stk(l3));
    }
  CreateVar(Rhs+1, "d", &un,&un, &l4);
  lout=FILENAME_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  C2F(savewave)(filename,stk(l2),&rate,&mn2,&err);
  if (err >  0) {
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  *stk(l4) = *stk(l2);
  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : loadwave, 
 ******************************************/

int intsloadwave(char *fname)
{
  WavInfo Wi;
  int m1,n1,l1,m2=1,n2,l2,err=0,un=1,eight=8,l3,m4,n4,l4;
  Nbvars=0;
  CheckRhs(1,1);
  CheckLhs(1,3);
  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*** first call to get the size **/
  lout=FILENAME_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  C2F(loadwave)(filename,(double *) 0,&n2,0,&Wi,&err);
  if (err >  0) {
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  CreateVar(2,"d",&m2,&n2,&l2);
  CreateVar(3,"d",&un,&eight,&l3);
  
  *stk(l3)   = Wi.wFormatTag;	/* data format */
  *stk(l3+1) = Wi.wChannels;	/* number of channels */
  *stk(l3+2) = Wi.wSamplesPerSecond; /* samples per second per channel */
  *stk(l3+3) = Wi.wAvgBytesPerSec; /* estimate of bytes per second needed */
  *stk(l3+4) = Wi.wBlockAlign;	/* byte alignment of a basic sample block */
  *stk(l3+5) = Wi.wBitsPerSample; /* bits per sample */
  *stk(l3+6) = Wi.data_length;	/* length of sound data in bytes */
  *stk(l3+7) = Wi.bytespersample; /* bytes per sample (per channel) */

  n4=1;m4=strlen(Wi.wav_format);
  CreateVar(4,"c",&m4,&n4,&l4);
  strncpy(cstk(l4),Wi.wav_format,strlen(Wi.wav_format));
  C2F(loadwave)(filename,stk(l2),&n2,1,&Wi,&err);
  if (err >  0) {
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  LhsVar(1)= 2;
  LhsVar(2)= 3;
  LhsVar(3)= 4;
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : mopen, 
 ******************************************/

int intsmopen(char *fname)
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,l4,l5,err;
  int swap = 1,un=1;
  char *status;

  Nbvars=0;
  CheckRhs(1,3);
  CheckLhs(1,2);
  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  if ( Rhs >= 2) 
    {
      GetRhsVar(2,"c",&m2,&n2,&l2);
      status = cstk(l2);
    }
  else 
    {
      status = "rb";
    }
  if ( Rhs >= 3) 
    {
      GetRhsVar(3,"i",&m3,&n3,&l3);
      swap = *istk(l3);
    } 
  CreateVar(Rhs+1, "i", &un,&un, &l4);
  CreateVar(Rhs+2, "d", &un,&un, &l5);
  lout=FILENAME_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  C2F(mopen)(istk(l4),filename,status,&swap,stk(l5),&err);
  if (err >  0) {
    if ( Lhs == 1) 
      {
	if ( err == 1) {
	  Error(66);/* no more logical units */
	  return 0;
	}
	else if ( err == 2) {
	  sciprint("%s:  Could not open the file!\r\n",fname);
	  Error(999);
	  return 0;
	}
	else {
	  Error(112);/* Not enough memory to  open the file*/
	  return 0;
	}
      }
    else
      {
	*stk(l5) = - err;
      }
  }
  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : mputstr, 
 ******************************************/

int intsmputstr(char *fname)
{
  int m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3,err;
  int fd = -1;
  Nbvars=0;
  CheckRhs(1,2);
  CheckLhs(1,1);
  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  if ( Rhs >= 2) 
    {
      GetRhsVar(2,"i",&m2,&n2,&l2);
      fd = *istk(l2);
    }
  CreateVar(Rhs+1, "d", &m3,&n3, &l3);
  C2F(mputstr)(&fd,cstk(l1),stk(l3),&err);
  if (err >  0) {
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : mclose, 
 ******************************************/

int intsmclose(char *fname)
{
  int m1,n1,l1,un=1,l2;
  int fd = -1;
  Nbvars=0;
  CheckRhs(0,1);
  CheckLhs(1,1);
  /*  checking variable file */
  if ( Rhs >= 1) 
    {
      GetRhsVar(1,"i",&m1,&n1,&l1);
      fd = *istk(l1);
    }
  CreateVar(Rhs+1, "d", &un,&un, &l2);
  C2F(mclose)(&fd,stk(l2));
  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}
/******************************************
 * SCILAB function : mput, 
 ******************************************/

int intsmput(char *fname)
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,err;
  char *type;
  int fd=-1;
  Nbvars=0;
  CheckRhs(1,3);
  CheckLhs(1,1);
  /*  checking variable res */
  GetRhsVar(1,"d",&m1,&n1,&l1);
  n1=m1*n1;
  if ( Rhs >= 2) 
    {
      GetRhsVar(2,"c",&m2,&n2,&l2);
      type = cstk(l2);
    }
  else 
    {
      type = "l";
    }
  if ( Rhs >= 3) 
    {
      GetRhsVar(3,"i",&m3,&n3,&l3);
      fd = *istk(l3);
    }
  C2F(mput)(&fd,stk(l1),&n1,type,&err);
  if (err >  0) {
    /* sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  LhsVar(1)= 0;
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : mget, 
 ******************************************/

int intsmget(char *fname)
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,l4,err;
  char *type;
  int fd=-1;
  int n=1,un=1;
  Nbvars=0;
  CheckRhs(1,3);
  CheckLhs(1,1);
  if ( Rhs >= 1) 
    {
      GetRhsVar(1,"i",&m1,&n1,&l1);
      n  = *istk(l1);
    }
  if ( Rhs >= 2) 
    {
      GetRhsVar(2,"c",&m2,&n2,&l2);
      type = cstk(l2);
    }
  else 
    {
      type = "l";
    }
  if ( Rhs >= 3) 
    {
      GetRhsVar(3,"i",&m3,&n3,&l3);
      fd = *istk(l3);
    }
  CreateVar(Rhs+1,"d",&un,&n,&l4);
  LhsVar(1)= Rhs+1;
  C2F(mget)(&fd,stk(l4),&n,type,&err);
  if (err >  0) 
    {
      /*      sciprint("%s: Internal Error \r\n",fname);*/
      Error(10000);
      return 0;
    }
  else if ( err < 0) 
    {
      int n5,l5,i;
      /** n contains now the effectively read data **/
      n5 = -err -1;
      if ( n5 < n ) 
	{
	  CreateVar(Rhs+2,"d",&un,&n5,&l5);
	  for ( i=0; i < n5 ; i++) 
	    *stk(l5+i) = *stk(l4+i);
	  LhsVar(1)= Rhs+2;
	}
    }
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : mgetstr, 
 ******************************************/

int intsmgetstr(char *fname)
{
  int m1,n1,l1,m2,n2,l2,l3,err;
  int fd=-1;
  int n=1,un=1;
  Nbvars=0;
  CheckRhs(1,3);
  CheckLhs(1,1);
  if ( Rhs >= 1) 
    {
      GetRhsVar(1,"i",&m1,&n1,&l1);
      n  = *istk(l1);
    }
  if ( Rhs >= 2) 
    {
      GetRhsVar(2,"i",&m2,&n2,&l2);
      fd = *istk(l2);
    }
  CreateVar(Rhs+1,"c",&n,&un,&l3);
  C2F(mgetstr1)(&fd,cstk(l3),&n,&err);
  LhsVar(1)=Rhs+1;
  if (err >  0) 
    {
      /*      sciprint("%s: Internal Error \r\n",fname);*/
      Error(10000);
      return 0;
    }
  else if ( err < 0) 
    {
      int n5,l5;
      /** n contains now the effectively read data **/
      n5 = -err -1;
      if ( n5 < n ) 
	{
	  CreateVar(Rhs+2,"c",&un,&n5,&l5);
	  strcpy(cstk(l5),cstk(l3));
	  LhsVar(1)= Rhs+2;
	}
    }
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : meof, 
 ******************************************/

int intsmeof(char *fname)
{
  int m1,n1,l1,un=1,lr;
  int fd=-1;
  Nbvars=0;
  CheckRhs(0,1);
  CheckLhs(1,1);
  if ( Rhs >= 1) 
    {
      GetRhsVar(1,"i",&m1,&n1,&l1);
      fd  = *istk(l1);
    }
  CreateVar(Rhs+1,"d",&un,&un,&lr);
  C2F(meof)(&fd,stk(lr));
  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : mseek, 
 ******************************************/

int intsmseek(char *fname)
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,err;
  int fd=-1;
  char *flag;
  Nbvars=0;
  CheckRhs(1,3);
  CheckLhs(1,1);
  GetRhsVar(1,"i",&m1,&n1,&l1);
  if ( Rhs >= 2) 
    {
      GetRhsVar(2,"i",&m2,&n2,&l2);
      fd = *istk(l2);
    }
  if ( Rhs >= 3)
    {
      GetRhsVar(3,"c",&m3,&n3,&l3);
      flag = cstk(l3);
    }
  else
    {
      flag = "set";
    }
  C2F(mseek)(&fd,istk(l1),flag,&err);
  if (err >  0) {
    /* sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  LhsVar(1)=0;
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : mtell, 
 ******************************************/

int intsmtell(char *fname)
{
  int m1,n1,l1,un=1,l2,err;
  int fd = -1;
  Nbvars=0;
  CheckRhs(0,1);
  CheckLhs(1,1);
  /*  checking variable file */
  if ( Rhs >= 1) 
    {
      GetRhsVar(1,"i",&m1,&n1,&l1);
      fd = *istk(l1);
    }
  CreateVar(Rhs+1, "d", &un,&un, &l2);
  C2F(mtell)(&fd,stk(l2),&err);
  if (err >  0) {
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}

/******************************************
 * SCILAB function : mclearerr, 
 ******************************************/

int intsmclearerr(char *fname)
{
  int m1,n1,l1;
  int fd = -1;
  Nbvars=0;
  CheckRhs(0,1);
  CheckLhs(1,1);
  /*  checking variable file */
  if ( Rhs >= 1) 
    {
      GetRhsVar(1,"i",&m1,&n1,&l1);
      fd = *istk(l1);
    }
  C2F(mclearerr)(&fd);
  LhsVar(1)= 0;
  PutLhsVar();
  return 0;
}

extern int int_objprintf __PARAMS((char *));
extern int int_objfprintf __PARAMS((char *));
extern int int_objsprintf __PARAMS((char *));
extern int int_objscanf __PARAMS((char *));
extern int int_objfscanf __PARAMS((char *));
extern int int_objsscanf __PARAMS((char *));
extern int int_objfscanfMat __PARAMS((char *));
extern int int_objnumTokens __PARAMS((char *));
extern int int_objfprintfMat __PARAMS((char *));




/**********************
 *  interface function 
 ********************/
static TabF Tab[]={
 { intssavewave, "savewave"},
 { intsloadwave, "loadwave"},
 { intsmopen, "mopen"},
 { intsmputstr, "mputstr"},
 { intsmclose, "mclose"},
 { intsmput, "mput"},
 { intsmget, "mget"},
 { intsmgetstr, "mgetstr"},
 { intsmeof, "meof"},
 { intsmseek, "mseek"},
 { intsmtell, "mtell"},
 { intsmclearerr, "mclearerr"},
 {int_objprintf,"mprintf"},
 {int_objfprintf,"mfprintf"},
 {int_objsprintf,"msprintf"},
 {int_objscanf,"mscanf"},
 {int_objfscanf,"mfscanf"},
 {int_objsscanf,"msscanf"},
 {int_objfscanfMat,"fscanfMat"},
 {int_objfprintfMat,"fprintfMat"},
 {int_objnumTokens,"NumTokens"},
};

int C2F(soundi)(void)
{
 Rhs=Max(0,Rhs);
 (*(Tab[Fin-1].f))(Tab[Fin-1].name);
 return 0;
}

