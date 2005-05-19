/*********************************************************************
 * This Software is ( Copyright ENPC 1998 )                          *
 * Jean-Philippe Chancelier Enpc/Cergrene                            *
 * See also the copyright below for do_printf                        *
 * interface and implementation of xxprintf and xxscanf functions    *
 * for scilab                                                        *
 *                                                                   * 
 * Modified May 2000 by S. Steer for vectorization of                *
 * *printf and *scanf                                                *
 *********************************************************************/

#include <math.h>
#include <stdio.h>
#ifdef __STDC__
#include <stdlib.h>
#else 
#include <malloc.h>
#endif
#include <string.h>

#include <ctype.h>  /* isdigit */
#include "../graphics/Math.h"
#include "../stack-c.h"
#include "../sun/Sun.h"

extern char * SciGetLine __PARAMS((char *));
extern FILE *GetFile __PARAMS((int *));
extern int C2F(xscion) __PARAMS((int *));

#ifdef __STDC__ 
extern int  sciprint2(int iv,char *fmt,...) ;
#else 
/*VARARGS0*/
extern int sciprint2() ;
#endif 


/* extern int sciprint2 __PARAMS((int i,char *fmt, ...));*/

static int do_printf __PARAMS((char *fname,FILE * fp, char *format,int n_args,
			      int arg_cnt,int lcount,char **strv));

/*if maxscan is increased don't forget to chage the (*printer)(......) 
  in do_scanf procedure */
#define MAXSCAN 30

typedef union {
  char * c;
  long unsigned int lui;
  short unsigned int sui;
  unsigned int ui;
  long int li;
  short int si;
  int i;
  double lf;
  float f;
} rec_entry;
typedef union {
  double d;
  char * s;
} entry;
typedef enum {SF_C,SF_S,SF_LUI,SF_SUI,SF_UI,SF_LI,SF_SI,SF_I,SF_LF,SF_F} sfdir;
static int Store_Scan __PARAMS((int *nrow,int *ncol,sfdir *type_s,sfdir *type,
			       int *retval, int*retval_s, rec_entry *buf, 
			       entry **data,int rowcount,int n));
static void Free_Scan  __PARAMS((int rowcount,int ncol,sfdir *type_s,entry **data));
static int Sci_Store __PARAMS((int nrow,int ncol,entry* data,sfdir *type,int retval));

int SciStrtoStr __PARAMS((int *Scistring,int *nstring,int *ptrstrings,char **strh));

static int do_scanf __PARAMS((char *fname,  FILE *fp, char *format,int *nargs, 
			     char *strv,int *retval,rec_entry *buf,sfdir *type));
#define RET_END -2
#define RET_BUG -1 
#define FAIL 0
#define OK 1
#define MEM_LACK -3
#define MISMATCH -4
#define NOT_ENOUGH_ARGS -5

static int GetString __PARAMS((char *fname,int *first,int *arg,int narg,
			      int *ir,int ic,char **sval) );
static int GetScalarInt __PARAMS((char *fname,int *first,int *arg,int narg,
				 int *ir,int ic,int *ival));
static int GetScalarDouble  __PARAMS((char *fname,int *first,int *arg,int narg,
				     int *ir,int ic,double *dval));
static int StringConvert __PARAMS((char *str));

int NumTokens __PARAMS((char *str));

/*********************************************************************
 * Scilab printf function OK 
 *********************************************************************/

int int_objprintf(char *fname,unsigned long fname_len)
{
  static int l1, m1, n1, lcount, rval, k, mx, mk, nk;
  char *ptrFormat=NULL;
  int i=0;
  int NumberPercent=0;

  Nbvars = 0;
  CheckRhs(1,1000);
  CheckLhs(0,1);
  if ( Rhs < 1 ) 
    { 
      Scierror(999,"Error:\tRhs must be > 0\r\n");
      return 0;
    }

  for (k=2;k<=Rhs;k++) {
    if (VarType(k) !=1 && VarType(k) !=10) {OverLoad(k);return 0;}
  }

  GetRhsVar(1,"c",&m1,&n1,&l1);
  ptrFormat=cstk(l1
);
  for(i=0;i<(int)strlen(ptrFormat);i++)
  {
    if (ptrFormat[i]=='%') {
      NumberPercent++;
      if (ptrFormat[i+1]=='%') {NumberPercent--;i++;}
    }
  }

  if (NumberPercent<Rhs-1)
  {
	Scierror(999,"mfprintf: Invalid format.\r\n");
	return 0;
  }

  mx=0;
  if (Rhs>=2) {
    GetMatrixdims(2,&mx,&nk);
    for (k=3;k<=Rhs;k++) {
      GetMatrixdims(k,&mk,&nk);
      mx = Min(mx,mk);
    }
  }
  lcount = 1;
  if (Rhs == 1) 
    rval=do_printf("printf",stdout,cstk(l1),Rhs,1,lcount,(char **)0);
  else
    while (1) 
      {
	if ((rval = do_printf("printf",stdout,cstk(l1),Rhs,1,lcount,(char **)0)) < 0) break;
	lcount++;
	if (lcount>mx) break;
      }
  if (rval == RET_BUG) return 0;
  LhsVar(1)=0; /** No return value **/
  PutLhsVar();
  return 0;
}  

/*********************************************************************
 * Scilab fprintf function OK 
 *********************************************************************/

int int_objfprintf(char *fname,unsigned long fname_len)
{
  FILE *f;
  static int l1, m1, n1,l2,m2,n2,lcount,rval, mx, mk, nk, k;
  char *ptrFormat=NULL;
  int i=0;
  int NumberPercent=0;

  Nbvars = 0;
  CheckRhs(1,1000);
  CheckLhs(0,1);
  if ( Rhs < 2 ) 
    { 
      Scierror(999,"Error:\tRhs must be >= 2\r\n");
      return 0;
    }
  for (k=3;k<=Rhs;k++) {
    if (VarType(k) !=1 && VarType(k) !=10) {OverLoad(k);return 0;}
  }
  GetRhsVar(1,"i",&m1,&n1,&l1); /* file id */
  GetRhsVar(2,"c",&m2,&n2,&l2); /* format */
  ptrFormat=cstk(l2);

  if ((f= GetFile(istk(l1))) == (FILE *)0)
    {
      Scierror(999,"mfprintf:\t wrong file descriptor %d\r\n",*istk(l1));
      return 0;
    }

  for(i=0;i<(int)strlen(ptrFormat);i++)
  {
    if (ptrFormat[i]=='%') {
      NumberPercent++;
      if (ptrFormat[i+1]=='%') {NumberPercent--;i++;}
    }
  }

  if (NumberPercent<Rhs-2)
  {
	Scierror(999,"mfprintf: Invalid format.\r\n");
	return 0;
  }

  mx=0;
  if (Rhs>=3) {
    GetMatrixdims(3,&mx,&nk);
    for (k=4;k<=Rhs;k++) {
      GetMatrixdims(k,&mk,&nk);
      mx = Min(mx,mk);
    }
  }
  lcount=1;
  if (Rhs == 2) 
    rval=do_printf("fprintf",f,cstk(l2),Rhs,2,lcount,(char **)0);
  else
    while (1) 
      {
	if ((rval=do_printf("fprintf",f,cstk(l2),Rhs,2,lcount,(char **)0)) < 0) break;
	lcount++;
	if (lcount>mx) break;
      }
  if (rval == RET_BUG) return 0;
  LhsVar(1)=0; /** No return value **/
  PutLhsVar();
  return 0;
}  

/*********************************************************************
 * Scilab sprintf function OK 
 *********************************************************************/

int int_objsprintf(char *fname,unsigned long fname_len)
{
  unsigned long lstr;
  static int l1, m1, n1,n2,lcount,rval,blk=200;
  static int k;
  char ** strs;
  char *str,*str1;
  int n,nmax,cat_to_last,ll;
  char *ptrFormat=NULL;
  int i=0;
  int NumberPercent=0;

  Nbvars = 0;
  CheckRhs(1,1000);
  CheckLhs(0,1);
  if ( Rhs < 1 ) 
    { 
      Scierror(999,"Error:\tRhs must be > 0\r\n");
      return 0;
    }
  for (k=2;k<=Rhs;k++) {
    if (VarType(k) !=1 && VarType(k) !=10) {OverLoad(k);return 0;}
  }
  GetRhsVar(1,"c",&m1,&n1,&l1);
  ptrFormat=cstk(l1);

  for(i=0;i<(int)strlen(ptrFormat);i++)
  {
    if (ptrFormat[i]=='%') {
      NumberPercent++;
      if (ptrFormat[i+1]=='%') {NumberPercent--;i++;}
    }
  }

  if (NumberPercent<Rhs-1)
  {
	Scierror(999,"sprintf: Invalid format.\r\n");
	return 0;
  }

  n=0; /* output line counter */
  nmax=0;
  strs=NULL;
  lcount=1;
  cat_to_last=0;

  while (1) 
    {
      if ((rval=do_printf("sprintf",(FILE *) 0,cstk(l1),Rhs,1,lcount,
			  (char **) &lstr)) < 0) break; 
      lcount++;
      str=(char *) lstr;
      str1=str;
      while (*str != '\0') {
	if (strncmp(str,"\\n",2) ==0) {
	  k=(int)(str-str1);
	  if (! cat_to_last) { /*add a new line */
	    if (n==nmax) {
	      nmax+=blk;
	      if ( (strs = (char **) realloc(strs,nmax*sizeof(char **))) == NULL) goto mem;
	    }
	    if ((strs[n]=malloc((k+1))) == NULL) goto mem;
	    strncpy(strs[n],str1, k);
	    strs[n][k]='\0';
	    n++;
	  }
	  else { /* cat to previous line */
	    ll=strlen(strs[n-1]);
	    if ((strs[n-1]=realloc(strs[n-1],(k+1+ll))) == NULL) goto mem;
	    strncpy(&(strs[n-1][ll]),str1, k);
	    strs[n-1][k+ll]='\0';
	  }
	  k=0;
	  str+=2;
	  str1=str;
	  cat_to_last=0;
	}
	else
	  str++;
      }
      k=(int)(str-str1);
      if (k>0) {
	if ((! cat_to_last) || (n == 0)) { /*add a new line */
	  if (n==nmax) {
	    nmax+=blk;
	    if ( (strs = (char **) realloc(strs,nmax*sizeof(char **))) == NULL) goto mem;
	  }
	  if ((strs[n]=malloc((k+1))) == NULL) goto mem;
	  strncpy(strs[n],str1, k);
	  strs[n][k]='\0';
	  n++;
	  
	}
	else { /* cat to previous line */
	    ll=strlen(strs[n-1]);
	    if ((strs[n-1]=realloc(strs[n-1],(k+1+ll))) == NULL) goto mem;
	    strncpy(&(strs[n-1][ll]),str1, k);
	    strs[n-1][k+ll]='\0';
	}
      }
      if (strncmp(str-2,"\\n",2) !=0) cat_to_last=1;
      if (Rhs == 1) break;

    }
  if (rval == RET_BUG) return 0;
  /** Create a Scilab String : lstr must not be freed **/
  n2=1;
  CreateVarFromPtr(Rhs+1, "S", &n, &n2, strs);
  for (k=0;k<n;k++) free(strs[k]);
  free(strs);
  LhsVar(1)=Rhs+1;
  PutLhsVar();    
  return 0;
 mem:
  Scierror(999,"sprintf: cannot allocate cannot allocate more memory \r\n");
  return 0;
}  

/*********************************************************************
 * Scilab scanf function
 *********************************************************************/
#define MAXSTR 512


int int_objscanf(char *fname,unsigned long fname_len)
{
  static char String[MAXSTR];
  static int l1, m1, n1, len= MAXSTR-1,iarg,maxrow,nrow,rowcount,ncol;
  int args,retval,retval_s,lline,status,iflag,err,n_count;
  int interrupt=0;
  entry *data;
  rec_entry buf[MAXSCAN];
  sfdir  type[MAXSCAN],type_s[MAXSCAN];

  Nbvars = 0;
  CheckRhs(1,2);
  if (Rhs==2) {
    GetRhsVar(1,"i",&m1,&n1,&l1);
    if (m1*n1 != 1) {
      Scierror(999,"Error: in scanf: incorrect first argument\r\n");
      return 0;
    }
    iarg=2;
    maxrow=*istk(l1);

  }
  else {
    iarg=1;
    maxrow=1;
  }
  GetRhsVar(iarg,"c",&m1,&n1,&l1); /** format **/
  n_count=StringConvert(cstk(l1))+1;  /* conversion */
  /** Read a line with Scilab read function **/
  C2F(xscion)(&iflag);
  if (n_count>1) {
    Scierror(999,"Error: in scanf: format cannot include \\n \r\n");
    return 0;
  }

  nrow=maxrow; 
  rowcount = -1; /* number-1 of result lines already got */
  while (1) {
    rowcount++;
    if ((maxrow >= 0) && (rowcount >= maxrow)) break;
    /* get a line */
    C2F(xscion)(&iflag);
    C2F(zzledt)(String,&len,&lline,&status,&interrupt,&iflag,strlen(String));

    if(status != 0) 
      {
	Scierror(999,"Error: in scanf\r\n");
	return 0;
      }
    if (lline == 0) {String[0] = ' ';lline=1;}
    /** use the scaned line as input **/
    args = Rhs; /* args set to Rhs on entry */
    if (do_scanf("scanf",(FILE *) 0,cstk(l1),&args,String,&retval,buf,type) < 0) return 0;
    if ( retval == EOF) {
      /* 
	 Scierror(999,"Error: in %s: end of file reached\r\n",fname);
	 return 0;
      */
    }
    if ((err=Store_Scan(&nrow,&ncol,type_s,type,&retval,&retval_s,
			buf,&data,rowcount,args)) <0 ) {
      switch (err) {
      case MISMATCH:
	if (maxrow>=0) {
	  Free_Scan(rowcount,ncol,type_s,&data);
	  Scierror(999,"Error: in fscanf: data mismatch\r\n");
	  return 0;
	}
	break;
      case MEM_LACK:
	Free_Scan(rowcount,ncol,type_s,&data);
	Scierror(999,"Error: in scanf: cannot allocate more memory \r\n");
	return 0;
	break;
      }
      if (err ==MISMATCH) break;
    }
  } /*  while (1) */

  /* create Scilab variables with each column of data */
  err=Sci_Store(rowcount,ncol,data,type_s,retval_s);
  if (err==MEM_LACK) { Scierror(999,"Error: in sscanf: cannot allocate more memory \r\n");}
  return 0;
} 

/*********************************************************************
 * Scilab sscanf function
 *********************************************************************/

int int_objsscanf(char *fname,unsigned long fname_len)
{
  static int l1, m1, n1,l2,m2,n2,iarg,maxrow,nrow,rowcount,ncol;
  int args,retval,retval_s,err,n_count,lw,il1,ild1,skip;
  int k;

  entry *data;
  rec_entry buf[MAXSCAN];
  sfdir  type[MAXSCAN],type_s[MAXSCAN];
  char* str;

  Nbvars = 0;
  CheckRhs(2,3);
  if (Rhs==3) {
    GetRhsVar(1,"i",&m1,&n1,&l1);
    if (m1*n1!=1) {
      Scierror(999,"Error: in sscanf: incorrect first argument\r\n");
      return 0;
    }
    iarg=2;
    maxrow=*istk(l1);

  }
  else {
    iarg=1;
    maxrow=1;
  }
  lw = iarg + Top - Rhs; /* Scilab string vector */
  if (! C2F(getwsmat)("sscanf",&Top,&lw,&m1,&n1,&il1,&ild1,6L)) return 0;
  GetRhsVar(iarg+1,"c",&m2,&n2,&l2); /* Format */
  n_count=StringConvert(cstk(l2))+1;  /* conversion */
  if (maxrow >= 0) 
    if (maxrow*n_count>m1*n1) {
      Scierror(999,"Error: in sscanf: not enough entries in str\r\n");
      return 0;
    }

  k=0;
  nrow=maxrow; 
  rowcount = -1; /* number-1 of result lines already got */
  while (1) {
    rowcount++;
    if ((maxrow >= 0) && (rowcount >= maxrow)) break;
    if ( k >= m1*n1 ) break;
    skip=*istk(ild1+k)-1;
    SciStrtoStr(istk(il1+skip),&n_count,istk(ild1+k),&str);
    k +=n_count;
      
    args = Rhs; /* args set to Rhs on entry */
    err = do_scanf("sscanf",(FILE *)0,cstk(l2),&args,str,&retval,buf,type);
    free(str);
    if ( err < 0 )  return 0;
    if ( retval == EOF) {
      /* 
	 first returned argument wil be set to -1 
	 Scierror(999,"Error: in %s: end of string reached\r\n",fname);
	 return 0;
      */
    }
    if ((err=Store_Scan(&nrow,&ncol,type_s,type,&retval,&retval_s,
			buf,&data,rowcount,args)) <0 ) {
      switch (err) {
      case MISMATCH:
	if (maxrow>=0) {
	  Free_Scan(rowcount,ncol,type_s,&data);
	  Scierror(999,"Error: in sscanf: data mismatch\r\n");
	  return 0;
	}
	break;
      case MEM_LACK:
	Free_Scan(rowcount,ncol,type_s,&data);
	Scierror(999,"Error: in sscanf: cannot allocate more memory \r\n");
	return 0;
	break;
      }
      if (err==MISMATCH) break;
    }
  } /* while */
  /* create Scilab variables with each column of data */
  err=Sci_Store(rowcount,ncol,data,type_s,retval_s);
  if (err==MEM_LACK) { Scierror(999,"Error: in sscanf: cannot allocate more memory \r\n");}
  return 0;
}

/*********************************************************************
 * Scilab fscanf function
 *********************************************************************/

int int_objfscanf(char *fname,unsigned long fname_len)
{
  static int l1, m1, n1,l2,m2,n2,iarg,maxrow,nrow,rowcount,ncol;
  FILE  *f;
  int args,retval,retval_s,err;
  entry *data;
  long int pos;

  rec_entry buf[MAXSCAN];
  sfdir  type[MAXSCAN],type_s[MAXSCAN];

  Nbvars = 0;
  CheckRhs(2,3);

  if (Rhs==3) {
    GetRhsVar(1,"i",&m1,&n1,&l1);
    if (m1*n1 != 1 ) {
      Scierror(999,"Error: in fscanf: incorrect first argument\r\n");
      return 0;
    }
    iarg=2;
    maxrow=*istk(l1);
  }
  else {
    iarg=1;
    maxrow=1;
  }

  GetRhsVar(iarg,"i",&m1,&n1,&l1);
  GetRhsVar(iarg+1,"c",&m2,&n2,&l2);/* format */

  StringConvert(cstk(l2));  /* conversion */
  if ((f= GetFile(istk(l1))) == (FILE *)0)
    {
      Scierror(999,"fprintf:\t wrong file descriptor %d\r\n",*istk(l1));
      return 0;
    }
  nrow=maxrow; 
  rowcount = -1;
  while (1) {
    rowcount++;
    if ((maxrow >= 0) && (rowcount >= maxrow)) break;
    args = Rhs; /* args set to Rhs on entry */
    pos=ftell(f);
    if ( do_scanf("fscanf",f,cstk(l2),&args,(char *)0,&retval,buf,type) < 0 )  return 0;
    if ( retval == EOF) {
      /* 
      Scierror(999,"Error: in %s: end of file reached\r\n",fname);
      return 0;
      */
    }
    if ((err=Store_Scan(&nrow,&ncol,type_s,type,&retval,&retval_s,
			buf,&data,rowcount,args)) <0 ) {
      switch (err) {
      case MISMATCH:
	if (maxrow>=0) {
	  Free_Scan(rowcount,ncol,type_s,&data);
	  Scierror(999,"Error: in fscanf: data mismatch\r\n");
	  return 0;
	}
	fseek(f,pos,SEEK_SET);
	break;
      case MEM_LACK:
	Free_Scan(rowcount,ncol,type_s,&data);
	Scierror(999,"Error: in fscanf: cannot allocate more memory \r\n");
	return 0;
	break;
      }
      if (err==MISMATCH) break;
    }
  } /* while */


  /* create Scilab variable with each column of data */
  err=Sci_Store(rowcount,ncol,data,type_s,retval_s);
  if (err==MEM_LACK) { Scierror(999,"Error: in sscanf: cannot allocate more memory \r\n");}
  return 0;
}  


/*********************************************************************
 * Scilab numtokens
 *********************************************************************/

int int_objnumTokens(char *fname,unsigned long fname_len)
{
  static int l1,m1,n1,l2,un=1;
  Nbvars = 0;
  CheckRhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  StringConvert(cstk(l1));  /* conversion */
  CreateVar(2, "d", &un, &un, &l2);
  *stk(l2) = (double) NumTokens(cstk(l1));
  LhsVar(1) = 2;
  PutLhsVar();
  return 0;
}  


/*********************************************************************
 * Scilab fprintfMat function
 * fprintfMat('pipo',rand(2,2),'%f',['comment un';'comment 2'])
 *********************************************************************/

int int_objfprintfMat(char *fname,unsigned long fname_len)
{
  int l1, m1, n1,l2,m2,n2,m3,n3,l3,i,j,mS,nS;
  FILE  *f;
  char **Str2;
  char *Format;
  Nbvars = 0;
  CheckRhs(1,4); 
  CheckLhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);/* file name */
  GetRhsVar(2,"d",&m2,&n2,&l2);/* data */
  if ( Rhs >= 3) 
    {
      GetRhsVar(3,"c",&m3,&n3,&l3);/* format */
      StringConvert(cstk(l3));  /* conversion */
      Format = cstk(l3);
    }
  else 
    {
      Format = "%f";
    }
  if ( Rhs >= 4 )
    {
      GetRhsVar(4,"S",&mS,&nS,&Str2);
    }
  if (( f = fopen(cstk(l1),"w")) == (FILE *)0) 
    {
      Scierror(999,"Error: in function %s, cannot open file %s\r\n",
	       fname,cstk(l1));
      return 0;
    }

  if ( Rhs >= 4 )
    {
      for ( i=0 ; i < mS*nS ; i++) 
	fprintf(f,"%s\n",Str2[i]);
    }

  for (i = 0 ; i < m2 ; i++ ) 
    {
      for ( j = 0 ; j < n2 ; j++) 
	{
	  fprintf(f,Format,*stk(l2+i + m2*j));
	  fprintf(f," ");
	}
      fprintf(f,"\n");
    }
  fclose(f);
  LhsVar(1)=0 ; /** no return value **/
  if ( Rhs >= 4) FreeRhsSVar(Str2);
  PutLhsVar();
  return 0;
}  

/*********************************************************************
 * Scilab fscanMat function
 * fscanfMat('pipo')
 * [A,b]=fscanfMat('pipo')
 *********************************************************************/
#define INFOSIZE 1024

static int  Info_size = 0;
static char *Info= NULL;
static int ReadLine __PARAMS((FILE *fd,int *mem));


int int_objfscanfMat(char *fname,unsigned long fname_len)
{
  char **Str;
  int mem=0;
  double x;
  static int l1, m1, n1,l2,m2,n2;
  int i,j,rows,cols,lres,n;
  int vl=-1;
  FILE  *f;
  char *Format;
  if ( Info == NULL ) {
    if (( Info =malloc(INFOSIZE*sizeof(char)))==NULL) {
      Scierror(999,"Error: in function %s, cannot allocate enough memory\r\n",fname);
      return 0;
    }
    Info_size = INFOSIZE;
  }

  Nbvars = 0;
  CheckRhs(1,1); /** just 1 **/
  CheckLhs(1,2);
  GetRhsVar(1,"c",&m1,&n1,&l1);/* file name */
  if ( Rhs == 2) 
    {
      GetRhsVar(2,"c",&m2,&n2,&l2);/* format */
      StringConvert(cstk(l2));  /* conversion */
      Format = cstk(l2);
    }
  else 
    {
      Format = 0;
    }
  if (( f = fopen(cstk(l1),"r")) == (FILE *)0) 
    {
      Scierror(999,"Error: in function %s, cannot open file %s\r\n",
	       fname,cstk(l1));
      return 0;
    }
  /*** first pass to get colums and rows ***/
  strcpy(Info,"--------");
  n =0; 
  while ( sscanf(Info,"%lf",&x) <= 0 && n != EOF ) 
    { 
      n=ReadLine(f,&mem); 
      if ( mem == 1) {
	free(Info);Info=NULL;
	fclose(f);
	Scierror(999,"Error: in function %s, cannot allocate enough memory\r\n",fname);
	return 0;
      }
      vl++;
    }
  if ( n == EOF )
    {
      free(Info);Info=NULL;
      fclose(f);
      Scierror(999,"Error: in function %s, cannot read data in file %s\r\n",
	       fname,cstk(l1));
      return 0;
    }
  cols = NumTokens(Info);
  rows = 1;
  while (1) 
    { 
      n=ReadLine(f,&mem);
      if ( mem == 1) {
	free(Info);Info=NULL;
	fclose(f);
	Scierror(999,"Error: in function %s, cannot allocate enough memory\r\n",fname);
	return 0;
      }
      if ( n == EOF ||  n == 0 ) break;
      if ( sscanf(Info,"%lf",&x) <= 0) break;
      rows++;
    }
  if ( cols == 0 || rows == 0) rows=cols=0;
  CreateVar(Rhs+1, "d", &rows, &cols, &lres);
  /** second pass to read data **/
  rewind(f);
  /** skip non numeric lines **/
  if ( Lhs >= 2 && vl != 0 ) {
    if ((Str = malloc((vl+1)*sizeof(char *)))==NULL) {
      free(Info);Info=NULL;
      fclose(f);
      Scierror(999,"Error: in function %s, cannot allocate enough memory\r\n", fname);
      return 0;
    }
    Str[vl]=NULL;
  }

  for ( i = 0 ; i < vl ; i++) 
    {
      ReadLine(f,&mem);
      if ( mem == 1) {
	free(Info);Info=NULL;
	fclose(f);
	for (j=0;j<i;j++) free(Str[j]);
	free(Str);
	Scierror(999,"Error: in function %s, cannot allocate enough memory\r\n",fname);
	return 0;
      }
      if ( Lhs >= 2) {
	if ((Str[i]=malloc((strlen(Info)+1)*sizeof(char)))==NULL) { 
	  free(Info);Info=NULL;
	  fclose(f);
	  for (j=0;j<i;j++) free(Str[j]);
	  free(Str);
	  Scierror(999,"Error: in function %s, cannot allocate enough memory\r\n", fname);
	  return 0;
	}
	strcpy(Str[i],Info);
      }
    }

  if ( Lhs >= 2) {
    int un=1,zero=0,l;
    if ( vl > 0 ) 
      {
	CreateVarFromPtr(Rhs+2,"S",&vl,&un,Str);
	FreeRhsSVar(Str);
      }
    else 
      {CreateVar(Rhs+2,"c",&zero,&zero,&l);}
    LhsVar(2)=Rhs+2;
  }

  for (i=0; i < rows ;i++)
    for (j=0;j < cols;j++)
      { 
	double xloc;
	fscanf(f,"%lf",&xloc);
	*stk(lres+i+rows*j)=xloc;
      }
  fclose(f);
  LhsVar(1)=Rhs+1;
  PutLhsVar();
  /* just in case Info is too Big */ 
  if ( Info_size > INFOSIZE ) 
    {
      Info_size = INFOSIZE;
      Info = realloc(Info,Info_size*sizeof(char));
    }
  return 0;
}  


static int ReadLine(FILE *fd,int *mem)
{
  int n=0;
  char * Info1;
  while (1)
    {
      char c = (char) getc(fd);
      if ( n > Info_size ) 
	{
	  Info_size += INFOSIZE;
	  if (( Info1 = realloc(Info,Info_size*sizeof(char)))==NULL) {
	    *mem=1;
	    return EOF;
	  }
	  Info=Info1;
	}
      Info[n]= c ; 
      if ( c == '\n') { Info[n] = '\0' ; return 1;}
      else if ( c == (char)EOF ) return EOF;  
      n++;
    }
}

int NumTokens(char *string)
{
  char buf[128];
  int n=1;
  int lnchar=0,ntok=-1;
  int length = strlen(string)+1;
  if (string != 0)
    { 
      /** Counting leading white spaces **/
      sscanf(string,"%*[ \t\n]%n",&lnchar);
      while ( n != 0 && n != EOF && lnchar <= length  )
	{ 
	  int nchar1=0,nchar2=0;
	  ntok++;
	  n= sscanf(&(string[lnchar]),
		    "%[^ \n\t]%n%*[ \t\n]%n",buf,&nchar1,&nchar2);
	  lnchar += (nchar2 <= nchar1) ? nchar1 : nchar2 ;
	}
      return(ntok);
    }
  return(FAIL);
}


/***************************************************************
 * Emulation of Ansi C XXscanf functions 
 * The number of scaned object is hardwired (MAXSCAN) 
 * and scaned strings (%s,%c %[) are limited to MAX_STR characters
 *
 * XXXX Could be changed to eliminate the MAXSCAN limitation 
 * 
 ****************************************************************/

#define MAX_STR 1024

#define VPTR void * 


typedef int (*PRINTER) __PARAMS((FILE *, char *,...));
typedef int (*FLUSH) __PARAMS((FILE *));

int voidflush(FILE *fp)
{
  return 0;
}

static int do_scanf (char *fname, FILE *fp, char *format, int *nargs, char *strv, int *retval, rec_entry *buf, sfdir *type)
{
  int i;
  char sformat[MAX_STR];
  void *ptrtab[MAXSCAN];
  int nc[MAXSCAN];
  int n_directive_count=0;
  char save,directive;
  char *p,*p1;
  register char *q;
  char *target,*sval;
  int l_flag=0, h_flag=0,width_flag,width_val,ign_flag,str_width_flag=0;
  int num_conversion = -1;	/* for error messages and counting arguments*/


  PRINTER printer;		/* pts at fprintf() or sprintf() */
  if (fp == (FILE *) 0)		
    {
      /* doing sscanf or scanf */
      target = strv;
      printer = (PRINTER) sscanf;
    }
  else
    {
      /* doing fscanf */
      target = (char *) fp;
      printer = (PRINTER) fscanf;
    }
  
  q = format;
  *retval = 0;

  /* Traverse format string, doing scanf(). */
  while (1)
    {
      /* scanf */
      p=q;
      while (*q != '%' && *q != '\0' ) q++;
      if ( *q == '%' && *(q+1) == '%' ) 
	{
	  q=q+2;
	  while (*q != '%' && *q != '\0' ) q++;
	}
      if (*q == 0) 
	{
	  break ;
	}

      q++; /** q point to character following % **/

      
      /* 
       * We have found a conversion specifier, figure it out,
       * then scan the data asociated with it.
       */


      /* mark the '%' with p1 */
      
      p1 = q - 1; 

      /* check for width field */

      while ( isdigit(((int)*q)) ) q++;
      width_flag =0;

      if ( p1+1 != q ) 
	{	  
	  char w= *q;
	  *q='\0';
	  width_flag = 1;
	  sscanf(p1+1,"%d",&width_val);
	  *q=w;
	}

      /* check for ignore argument */

      ign_flag=0;

      if (*q == '*')
	{
	  /* Ignore the argument in the input args */
	  /*num_conversion = Max(num_conversion-1,0);*/
	  ign_flag = 1;
	  q++;
	}
      else

      /* check for %l or %h */

      l_flag = h_flag = 0;

      if (*q == 'l')
	{
	  q++;
	  l_flag = 1;
	}
      else if (*q == 'h')
	{
	  q++;
	  h_flag = 1;
	}

      /* directive points to the scan directive  */

      directive = *q++;

      if ( directive == '[' )
	{
	  char *q1=q--;
	  /** we must find the closing bracket **/
	  while ( *q1 != '\0' && *q1 != ']') q1++;
	  if ( *q1 == '\0') 
	    {
	      Scierror(998,"Error:\tscanf, unclosed [ directive\r\n");
	      return RET_BUG;
	    }
	  if ( q1 == q +1 || strncmp(q,"[^]",3)==0 ) 
	    {
	      q1++;
	      while ( *q1 != '\0' && *q1 != ']') q1++;  
	      if ( *q1 == '\0') 
		{
		  Scierror(998,"Error:\tscanf unclosed [ directive\r\n");
		  return RET_BUG;
		}
	    }
	  directive = *q1++;
	  q=q1;
	}

      /** accumulate characters in the format up to next % directive **/
      /*** unused 
      while ( *q != '\0' && *q != '%' ) q++;
      if ( *q == '%' && *(q+1) == '%' ) 
	{
	  q=q+2;
	  while (*q != '%' && *q != '\0' ) q++;
	}
	**/
      save = *q;
      /* *q = 0; */
      
      /** if (debug) Sciprintf("Now directive [%s],%c\r\n",p,directive); **/
      
      if ( ign_flag != 1) 
	{
	  num_conversion++;
	  if ( num_conversion > MAXSCAN ) 
	    {
	      Scierror(998,"Error:\tscanf too many (%d > %d) conversion required\r\n",
		       num_conversion,MAXSCAN);
	      return RET_BUG;
	    }
	  switch (directive )
	    {
	    case ']':
	      if (width_flag == 0 ) str_width_flag = 1;
	      if (width_flag == 1 && width_val > MAX_STR-1 )
		{
		  Scierror(998,"Error:\tscanf, width field %d is too long (> %d) for %%[ directive\r\n",
			   width_val,MAX_STR-1);
		  return RET_BUG;
		}
	      if ((buf[num_conversion].c=malloc(MAX_STR))==NULL) return MEM_LACK;
	      ptrtab[num_conversion] =  buf[num_conversion].c;
	      type[num_conversion] = SF_S;
	      break;
	    case 's':
	      if (l_flag + h_flag) {
		Scierror(998,"Error:\tscanf: bad conversion\r\n");
		return RET_BUG;
	      }
	      if (width_flag == 0 ) str_width_flag = 1;
	      if (width_flag == 1 && width_val > MAX_STR-1 )
		{
		  Scierror(998,"Error:\tscanf, width field %d is too long (< %d) for %%s directive\r\n",
			   width_val,MAX_STR-1);
		  return RET_BUG;
		}
	      if ((buf[num_conversion].c=malloc(MAX_STR))==NULL) return MEM_LACK;
	      ptrtab[num_conversion] =  buf[num_conversion].c;
	      type[num_conversion] = SF_S;
	      break;
	    case 'c':
	      if (l_flag + h_flag) {
		Scierror(998,"Error:\tscanf: bad conversion\r\n");
		return RET_BUG;
	      }
	      if ( width_flag == 1 ) 
		nc[num_conversion ] = width_val;
	      else
		nc[num_conversion ] = 1;
	      if (width_flag == 1 && width_val > MAX_STR-1 )
		{
		  Scierror(998,"Error:\tscanf width field %d is too long (< %d) for %%c directive\r\n",
			   width_val,MAX_STR-1);
		  return RET_BUG;
		}
	      if ((buf[num_conversion].c=malloc(MAX_STR))==NULL) return MEM_LACK;
	      ptrtab[num_conversion] =  buf[num_conversion].c;
	      type[num_conversion] = SF_C;
	      break;
	    case 'o':
	    case 'u':
	    case 'x':
	    case 'X':
	      if ( l_flag ) 
		{
		  ptrtab[num_conversion] =  &buf[num_conversion].lui;
		  type[num_conversion] = SF_LUI;
		}
	      else if ( h_flag) 
		{
		  ptrtab[num_conversion] =  &buf[num_conversion].sui;
		  type[num_conversion] = SF_SUI;
		}
	      else 
		{
		  ptrtab[num_conversion] =  &buf[num_conversion].ui;
		  type[num_conversion] = SF_UI;
		}
	      break;
	    case 'D':
	      ptrtab[num_conversion] =  &buf[num_conversion].li;
	      type[num_conversion] = SF_LI;
	      break;
	    case 'n':
	      /** count the n directives since they are not counted by retval **/
	      n_directive_count++;
	    case 'i':
	    case 'd':
	      if ( l_flag ) 
		{
		  ptrtab[num_conversion] =  &buf[num_conversion].li;
		  type[num_conversion] = SF_LI;
		}
	      else if ( h_flag) 
		{
		  ptrtab[num_conversion] =  &buf[num_conversion].si;
		  type[num_conversion] = SF_SI;
		}
	      else 
		{
		  ptrtab[num_conversion] =  &buf[num_conversion].i;
		  type[num_conversion] = SF_I;
		}
	      break;
	    case 'e':
	    case 'f':
	    case 'g':
	    case 'E':
	    case 'G':
	      if (h_flag)
		{
		  Scierror(998,"Error:\tscanf: bad conversion\r\n");
		  return RET_BUG;
		}
	      else if (l_flag) 
		{
		  ptrtab[num_conversion] =  &buf[num_conversion].lf;
		  type[num_conversion] = SF_LF;
		}
	      else
		{
		  ptrtab[num_conversion] =  &buf[num_conversion].f;
		  type[num_conversion] = SF_F;
		}
	      break;
	    default:
	      Scierror(998,"Error:\tscanf: bad conversion\r\n");
	      return RET_BUG;
	    }
	  *q = save;
	}
    }
  /** we replace %s and %[ directive with a max length field **/
  
  if ( str_width_flag == 1) 
    {
      char *f1=format;
      char *f2=sformat;
      char *slast = sformat + MAX_STR-1 -4;
      while ( *f1 != '\0'  ) 
	{
	  int n;
	  *f2++ = *f1++;
	  
	  if ( *(f1-1) == '%' && ( *(f1) == 's'  || *(f1) == '['))
	    {
	      n=sprintf(f2,"%d",MAX_STR-1);
	      f2 += n;
      	      *f2++ = *f1++;
	    }
	  if ( f2 == slast )
	    {
	      Scierror(998,"Error:\tscanf, format is too long (> %d) \r\n",MAX_STR-1);
	      return RET_BUG;
	    }
	}
      *f2='\0';
      format = sformat;
    }
    

  /** Calling scanf function : 
    Only  num_conversion +1 qrgument are used 
    the last arguments transmited points to nothing 
    but this is not a problem since they won't be used ***/


  *retval = (*printer) ((VPTR) target,format,ptrtab[0],ptrtab[1],ptrtab[2],
			ptrtab[3],ptrtab[4],ptrtab[5],ptrtab[6],ptrtab[7],
			ptrtab[8],ptrtab[9],ptrtab[10],ptrtab[11],ptrtab[12],
			ptrtab[13],ptrtab[14],ptrtab[15],ptrtab[16],ptrtab[17],
			ptrtab[18],ptrtab[19],ptrtab[20],ptrtab[21],ptrtab[22],
			ptrtab[23],ptrtab[24],ptrtab[25],ptrtab[26],ptrtab[27],
			ptrtab[28],ptrtab[29]);
  /** *nargs counts the number of corectly scaned arguments **/
  *nargs = Min(num_conversion+1,Max(*retval+n_directive_count,0));

  for ( i=1 ; i <= *nargs ; i++) 
    if ( type[i-1]  == SF_C ) {
      sval=(char *) ptrtab[i-1];
      sval[nc[i-1]]='\0';
    }
  return 0;
}


/***************************************************************
								
  do_printf: code extraced from RLab and hacked for Scilab 
              by Jean-Philippe Chancelier 1998. 

    Copyright (C) 1995  Ian R. Searle
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *************************************************************** */


/*---------- types and defs for doing printf ------------*/
#define  PF_C		0
#define  PF_S		1
#define  PF_D		2	/* int conversion */
#define  PF_LD		3	/* long int */
#define  PF_F		4	/* float conversion */

/* for switch on number of '*' and type */

#define  AST(num,type)  (5*(num)+(type))

/* Buffer for printf **/

#define MAX_SPRINTF_SIZE  4096
static char sprintf_buff[MAX_SPRINTF_SIZE];
static char *sprintf_limit = sprintf_buff + MAX_SPRINTF_SIZE;

static int do_printf (char *fname, FILE *fp, char *format, int nargs, int argcnt, int lcount, char **strv)
{
  int previous_t=0; 
  int m1;
  char save;
  char *p;
  register char *q;
  char *target;
  int l_flag, h_flag;		/* seen %ld or %hd  */
  int ast_cnt;
  int ast[2];
  double dval = 0.0;
  int rval=0;
  char *sval;
  int num_conversion = 0;	/* for error messages */
  int pf_type = 0;		/* conversion type */
  PRINTER printer;		/* pts at fprintf() or sprintf() */
  FLUSH   flush;
  int arg_cnt,ccount;

  int retval;			/* Attempt to return C-printf() return-val */
  arg_cnt = argcnt;
  ccount = 1;
  q = format;
  retval = 0;

  if (fp == (FILE *) 0)		
    {
      /* doing sprintf */
      target = sprintf_buff;
      flush = voidflush;
      printer = (PRINTER) sprintf;
    }
  else if ( fp == stdout ) 
    {
      /** doing printf **/
      target =  (char *) 0; /* unused */
      flush = fflush;
      printer = (PRINTER) sciprint2;
    }
  else 
    {
      /* doing fprintf */
      target = (char *) fp;	/* will never change */
      flush = fflush;
      printer = (PRINTER) fprintf;
    }

  /* Traverse format string, doing printf(). */
  while (1)
    {
      if (fp)			/* printf */
	/** XXXX on pourrait couper en deux pour separer fp==stdout et fp == file **/
	{
	  while (*q != '%')
	    {
	      switch (*q) 
		{
		case 0 : 
		  fflush (fp);
		  return (retval);
		  break;
		case '\\':
		  q++;
		  switch (*q) 
		    {
		    case 0 : 
		      fflush (fp);
		      return (retval);
		      break;
		    case 'r':
		      (*printer) ((VPTR) target, "\r");
		      q++;
		      retval++;
		      break;
		    case 'n':
		      if ( fp == stdout ) 
			(*printer) ((VPTR) target, "\r");
		      (*printer) ((VPTR) target, "\n");
		      q++;
		      retval++;
		      break;
		    case 't':
		      (*printer) ((VPTR) target, "\t");
		      q++;
		      retval++;
		      break;
		    case '\\':
		      (*printer) ((VPTR) target, "\\");
		      q++;
		      retval++;
		      break;
		    default:
		      /**  putc (*q, fp); **/
		      (*printer) ((VPTR) target, "%c",*q);
		      q++;
		      retval++;
		    }
		  break;
		default:
		  /**  putc (*q, fp); **/
		  (*printer) ((VPTR) target, "%c",*q);
		  q++;
		  retval++;
		  break;
		}
	    }
	}
      else
	{
	  /* sprintf() */
	  while (*q != '%')
	    {
	      if (*q == 0)
		{
		  if (target > sprintf_limit)	/* damaged */
		    {
		      Scierror(998,"Error:\tsprintf problem, buffer too small\r\n");
		      return RET_BUG;
		    }
		  else
		    {
		      /* really done */
		      *target = '\0';
		      *strv = sprintf_buff;
		      return (retval);
		    }
		}
	      else
		{
		  *target++ = *q++;
		  retval++;
		}
	    }
	}

      num_conversion++;

      if (*++q == '%')		/* %% */
	{
	  if (fp)
	    {
	      /** putc (*q, fp); **/
	      (*printer) ((VPTR) target, "%c",*q);
	      retval++;
	    }
	  else
	    {
	      *target++ = *q;
	    }
	  q++;
	  continue;
	}

      /* 
       * We have found a conversion specifier, figure it out,
       * then print the data associated with it.
       */


      /* mark the '%' with p */
      p = q - 1;

      /* eat the flags */
      while (*q == '-' || *q == '+' || *q == ' ' ||
	     *q == '#' || *q == '0')
	q++;

      ast_cnt = 0;		/* asterisk count */
      if (*q == '*')
	{
	  /* Use current arg as field width spec */
	  rval=GetScalarInt(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&m1);
	  if (rval <= 0) {
	    if (rval== NOT_ENOUGH_ARGS) goto bad;
	    return rval;
	  }
			   
	  ast[ast_cnt++] = m1;
	  q++;

	}
      else
	while ( isdigit(((int)*q)))  q++;
      /* width is done */

      if (*q == '.')		/* have precision */
	{
	  q++;
	  if (*q == '*')
	    {
	      /* Use current arg as precision spec */
	      rval=GetScalarInt(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&m1);
	      if (rval <= 0) {
		if (rval== NOT_ENOUGH_ARGS) goto bad;
		return rval;
	      }
	      ast[ast_cnt++] = m1;
	      q++;
	    }
	  else
	    while ( isdigit(((int)*q)) ) q++;
	}


      l_flag = h_flag = 0;

      if (*q == 'l')
	{
	  q++;
	  l_flag = 1;
	}
      else if (*q == 'h')
	{
	  q++;
	  h_flag = 1;
	}

      /* Set pf_type and load val */
      switch (*q++)
	{
	case 's':
	  if (l_flag + h_flag)
	    Scierror(998,"Warning:\tprintf: bad conversion l or h flag mixed with s directive\r\n");
	  rval=GetString(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&sval);
	  if (rval <= 0) {
	    if (rval== NOT_ENOUGH_ARGS) goto bad;
	    return rval;
	  }
	  pf_type = PF_S;
	  break;
	case 'c':
	  if (l_flag + h_flag)
	    Scierror(998,"Warning:\tprintf: bad conversion l or h flag mixed with c directive\r\n");
	  rval=GetString(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&sval);
	  if (rval <= 0) {
	    if (rval== NOT_ENOUGH_ARGS) goto bad;
	    return rval;
	  }
	  pf_type = PF_C;
	  break;
	case 'd':
	  rval=GetScalarDouble(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&dval);
	  if (rval <= 0) {
	    if (rval== NOT_ENOUGH_ARGS) goto bad;
	    return rval;
	  }
	  pf_type = PF_D;
	  break;

	case 'o':
	  Scierror(998,"Error:\tprintf: \"o\" format not allowed\r\n");
	  return RET_BUG;
	  break;

	case 'x':
	  rval=GetScalarDouble(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&dval);
	  if (rval <= 0) {
	    if (rval== NOT_ENOUGH_ARGS) goto bad;
	    return rval;
	  }
	  pf_type = PF_D;
	  break;

	case 'X':
	  rval=GetScalarDouble(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&dval);
	  if (rval <= 0) {
	    if (rval== NOT_ENOUGH_ARGS) goto bad;
	    return rval;
	  }
	  pf_type = PF_D;
	  break;

	case 'i':
	case 'u':
	  /* use strod() here */
	  rval=GetScalarDouble(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&dval);
	  if (rval <= 0) {
	    if (rval== NOT_ENOUGH_ARGS) goto bad;
	    return rval;
	  }
	  pf_type = l_flag ? PF_LD : PF_D;
	  break;

	case 'e':
	case 'g':
	case 'f':
	case 'E':
	case 'G':
	  if (h_flag + l_flag)
	    {
	      Scierror(998,"Error:\tprintf: bad conversion\r\n");
	      return RET_BUG;
	    }
	  /* use strod() here */
	  rval=GetScalarDouble(fname,&previous_t,&arg_cnt,nargs,&ccount,lcount,&dval);
	  if (rval <= 0) {
	    if (rval== NOT_ENOUGH_ARGS) goto bad;
	    return rval;
	  }
	  pf_type = PF_F;
	  break;

	default:
	  Scierror(998,"Error:\tprintf: bad conversion\r\n");
	  return RET_BUG;
	}

      save = *q;
      *q = 0;
      /* ready to call printf() */
      /* 
       * target:   The output file (or variable for sprintf())
       * p:        the beginning of the format
       * ast:      array with asterisk values
       */
      switch (AST (ast_cnt, pf_type))
	{
	case AST (0, PF_C):
	  retval += (*printer) ((VPTR) target, p, sval[0]);
	  free(sval);
	  break;

	case AST (1, PF_C):
	  retval += (*printer) ((VPTR) target, p, ast[0], sval[0]);
	  free(sval);
	  break;

	case AST (2, PF_C):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1],sval[0]);
	  free(sval);
	  break;

	case AST (0, PF_S):
	  retval += (*printer) ((VPTR) target, p, sval);
	  free(sval);
	  break;

	case AST (1, PF_S):
	  retval += (*printer) ((VPTR) target, p, ast[0], sval);
	  free(sval);
	  break;

	case AST (2, PF_S):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1], sval);
	  free(sval);
	  break;

	case AST (0, PF_D):
	  retval += (*printer) ((VPTR) target, p, (int) dval);
	  break;

	case AST (1, PF_D):
	  retval += (*printer) ((VPTR) target, p, ast[0], (int) dval);
	  break;

	case AST (2, PF_D):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1], (int) dval);
	  break;

	case AST (0, PF_LD):
	  retval += (*printer) ((VPTR) target, p, (long int) dval);
	  break;

	case AST (1, PF_LD):
	  retval += (*printer) ((VPTR) target, p, ast[0], (long int) dval);
	  break;

	case AST (2, PF_LD):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1], (long int) dval);
	  break;

	case AST (0, PF_F):
	  retval += (*printer) ((VPTR) target, p, dval);
	  break;

	case AST (1, PF_F):
	  retval += (*printer) ((VPTR) target, p, ast[0], dval);
	  break;

	case AST (2, PF_F):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1], dval);
	  break;
	}
      if (fp == (FILE *) 0)
	while (*target)
	  target++;
      *q = save;
    }
  return (retval);
 bad:
  (*printer) ((VPTR) target, "\n");
  (*flush) ((FILE *) target);
  Scierror(998,"Error:\tprintf: not enough arguments\r\n");
  return RET_BUG;
}





/****************************************************
 * Utility functions 
 ****************************************************/

static int  GetString(char *fname, int *previous_t, int *arg, int narg, int *ic, int ir, char **sval)
{
  int mx,nx,il,ild,lw,k,one=1;
  char *p;
  
  if (*previous_t != 2) {
    *arg = *arg+1;*ic=1;
    *previous_t = 2;
  }
  lw = *arg + Top - Rhs;
  
  if (! C2F(getwsmat)(fname,&Top,&lw,&mx,&nx,&il,&ild, strlen(fname))) return RET_BUG;
  else {
    if ( *ic>nx ) {
      *arg=*arg+1;
      if (*arg>narg ) return NOT_ENOUGH_ARGS;
      *ic=1;
      lw = *arg + Top - Rhs;
      if (! C2F(getwsmat)(fname,&Top,&lw,&mx,&nx,&il,&ild, strlen(fname))) return RET_BUG;
    }
  }
  if (ir>mx) return RET_END;
  k=ir-1+mx*(*ic-1);
  if (SciStrtoStr(istk(il-1+*istk(ild+k)),&one,istk(ild+k),&p) < 0) return MEM_LACK;
  *ic=*ic+1;
  *sval = p;
  return OK;
}

/** changes `\``n` --> `\n` idem for \t and \r  **/

static int StringConvert(char *str)
{
  char *str1;
  int count=0;
  str1 = str;
  
  while ( *str != 0) 
    {
      if ( *str == '\\' ) 
	{
	  switch ( *(str+1)) 
	    {
	    case 'n' : *str1 = '\n' ; str1++; str += 2;count++;break;
	    case 't' : *str1 = '\t' ; str1++; str += 2;break;
	    case 'r' : *str1 = '\r' ; str1++; str += 2;break;
	    default : *str1 = *str; str1++; str++;break;
	    }
	}
      else 
	{
	  *str1 = *str; str1++; str++;
	}
    }
  *str1 = '\0';
  return count;
}
static int GetScalarInt(char *fname, int *previous_t, int *arg, int narg, int *ic, int ir, int *ival)
{
  int mx,nx,lx;

  if (*previous_t != 1) {
    *arg=*arg+1;*ic=1;
    *previous_t = 1;
  }

  if (! C2F(getrhsvar)(arg,"i",&mx,&nx,&lx,1L))
    return RET_BUG;
  else {
    if ( (*ic>nx) || (*previous_t != 1)) {
      *arg=*arg+1;
      if (*arg > narg ) return NOT_ENOUGH_ARGS;
      *ic=1;
      if (! C2F(getrhsvar)(arg,"i",&mx,&nx,&lx,1L))
	return RET_BUG;
    }
  }
  if (ir>mx) return RET_END;
  *ival =  *(istk(lx+ir-1+mx*(*ic-1)));
   *ic=*ic+1;
  return OK;
}

static int GetScalarDouble(char *fname, int *previous_t, int *arg, int narg, int *ic, int ir, double *dval)
{
  int mx,nx,lx;

  if (*previous_t != 1) {
    *arg = *arg+1;
    *ic=1;
    *previous_t = 1;
  }
  if (! C2F(getrhsvar)(arg,"d",&mx,&nx,&lx,1L))
    return RET_BUG;
  else {
    if ( *ic>nx) {
      *arg=*arg+1;
      if (*arg > narg ) return NOT_ENOUGH_ARGS;
      *ic=1;
      if (! C2F(getrhsvar)(arg,"d",&mx,&nx,&lx,1L))
	return RET_BUG;
    }
  }
  if (ir>mx) return RET_END;
  *dval =  *(stk(lx+ir-1+mx*(*ic-1)));
  *ic=*ic+1;
  return OK;
}


static int Sci_Store(int nrow, int ncol, entry *data, sfdir *type, int retval_s)
{
  int cur_i,i,j,i1,one=1,zero=0,k,l,iarg,colcount;
  sfdir cur_type;
  char ** temp;
     
  /* create Scilab variable with each column of data */
  if (ncol+Rhs > intersiz ){
    Scierror(998,"Error:\ttoo many directive in scanf\r\n");
    return RET_BUG;
  }
  iarg=Rhs;
  if (Lhs > 1) {
    CreateVar(++iarg, "d", &one, &one, &l);
    *stk(l) = (double) retval_s;
    LhsVar(1)=iarg;
    if (ncol==0) goto Complete;

    for ( i=0 ; i < ncol ; i++) { 
      if ( (type[i] == SF_C) || (type[i] == SF_S) ) {
	if( (temp = (char **) malloc(nrow*ncol*sizeof(char **)))==NULL) return MEM_LACK;
	k=0;
	for (j=0;j<nrow;j++) temp[k++]=data[i+ncol*j].s;
	CreateVarFromPtr(++iarg, "S", &nrow, &one, temp);
	free(temp);
	for (j=0;j<nrow;j++) free(data[i+ncol*j].s);
      }
      else {
	CreateVar(++iarg, "d", &nrow, &one, &l);
	for ( j=0 ; j < nrow ; j++) 
	  *stk(l+j)= data[i+ncol*j].d;
      }

      LhsVar(i+2)=iarg;
    }
    free(data);
    /** we must complete the returned arguments up to Lhs **/
  Complete:
    for ( i = ncol+2; i <= Lhs ; i++) 
      {
	iarg++;
	CreateVar(iarg,"d",&zero,&zero,&l);
	LhsVar(i) = iarg;
      }
  }
  else {/* Lhs==1 */
    char *ltype="cblock";
    int multi=0,endblk,ii,n;

    cur_type=type[0];
    
    for (i=0;i<ncol;i++)
      if (type[i] != cur_type) 	{
	multi=1;
	break;
      }
    if (multi) {
      i=strlen(ltype);
      iarg=Rhs;
      CreateVarFromPtr(++iarg, "c", &one, &i, &ltype); /* the mlist type field */
      cur_type=type[0];
      i=0;cur_i=i;

      while (1) {
	if (i < ncol)  
	  endblk=(type[i] != cur_type);
	else
	  endblk=1;
	if (endblk) {
	  colcount=i - cur_i;
	  if (nrow==0) {
	    CreateVar(++iarg, "d", &zero, &zero, &l);}
	  else if ( (cur_type == SF_C) || (cur_type == SF_S) ) {
	    if( (temp = (char **) malloc(nrow*colcount*sizeof(char **)))==NULL) return MEM_LACK;
	    k=0;
	    for (i1=cur_i;i1<i;i1++)
	      for (j=0;j<nrow;j++) temp[k++]=data[i1+ncol*j].s;
	    CreateVarFromPtr(++iarg, "S", &nrow, &colcount,temp);
	    free(temp);
	    for (i1=cur_i;i1<i;i1++)
	      for (j=0;j<nrow;j++) free(data[i1+ncol*j].s);
	  }
	  else {
	    CreateVar(++iarg, "d", &nrow, &colcount, &l);
	    ii=0;
	    for (i1=cur_i;i1<i;i1++) {
	      for ( j=0 ; j < nrow ; j++) 
		*stk(l+j+nrow*ii)= data[i1+ncol*j].d;
	      ii++;
	    }
	  }
	  if (i>=ncol) break;
	  cur_i=i;
	  cur_type=type[i];

	}
	i++;
      }
      n=iarg-Rhs; /* number of list fields*/
      
      iarg++;
      i=Rhs+1;
      C2F(mkmlistfromvars)(&i,&n);
      LhsVar(1)=i;
    }
    else {
      if (nrow==0) {
	CreateVar(Rhs+1, "d", &zero, &zero, &l);}
      else if ( (cur_type == SF_C) || (cur_type == SF_S) ) {
	if( (temp = (char **) malloc(nrow*ncol*sizeof(char **)))==NULL) return MEM_LACK;
	k=0;
	for (i1=0;i1<ncol;i1++)
	  for (j=0;j<nrow;j++) temp[k++]=data[i1+ncol*j].s;
	CreateVarFromPtr(Rhs+1, "S", &nrow, &ncol, temp);
	free(temp);
	for (i1=0;i1<ncol;i1++)
	  for (j=0;j<nrow;j++) free(data[i1+ncol*j].s);
      }
      else {
	CreateVar(Rhs+1, "d", &nrow, &ncol, &l);
	ii=0;
	for (i1=0;i1<ncol;i1++) {
	  for ( j=0 ; j < nrow ; j++) 
	    *stk(l+j+nrow*ii)= data[i1+ncol*j].d;
	  ii++;
	}
      }
    }
    LhsVar(1)=Rhs+1;
  }
  PutLhsVar();
  return 0;
}

/* ************************************************************************
 *   Store data scanned by a single call to do_scan in line rowcount of data 
 *   table 
 ************************************************************************/

static int Store_Scan(int *nrow, int *ncol, sfdir *type_s, sfdir *type, int *retval, int *retval_s, rec_entry *buf, entry **data, int rowcount, int n)
{ 
  int i,j,nr,nc,err;
  entry * Data;
  int blk=20; /* block size for memory allocation */
  nr=*nrow;
  nc=*ncol;

  if (rowcount==0) {
    for ( i=0 ; i < MAXSCAN ; i++) type_s[i]=SF_F; /* initialisation */
    if (nr<0) nr=blk;
    nc=n;
    *ncol=nc;
    *retval_s=*retval;
    if (n==0) {
      return 0;
    }
    if ( (*data = (entry *) malloc(nc*nr*sizeof(entry)))==NULL) {
      err= MEM_LACK;
      goto bad1;
    }
    for ( i=0 ; i < nc ; i++) type_s[i]=type[i];

  }
  else {
    /* check if number of data read match with previous number */
    if ( (n !=nc ) || (*retval_s != *retval) ){
      err=MISMATCH;
      goto bad2;
    }
    /* check if types of data read match with previous types */
    for ( i=0 ; i < nc ; i++)
      if (type[i] != type_s[i]) {
	err=MISMATCH;
	goto bad2;
      }

    /* check for memory and realloc if necessary*/
    if (rowcount>= nr) {
      nr=nr+blk;
      *nrow=nr;
      if ( (*data = (entry *) realloc(*data,nc*nr*sizeof(entry)))==NULL) {
	err= MEM_LACK;
	goto bad2;
      }
    }
  } 
  Data=*data;
  /* store values scanned in a new row */
  for ( i=0 ; i < nc ; i++) 
    {
      switch ( type_s[i] )
	{
	case SF_C:
	case SF_S:
	  Data[i+nc*rowcount].s=buf[i].c;
	  break;
	case SF_LUI:
	  Data[i+nc*rowcount].d=(double)buf[i].lui;
	  break;
	case SF_SUI:
	  Data[i+nc*rowcount].d=(double)buf[i].sui;
	  break;
	case SF_UI:
	  Data[i+nc*rowcount].d=(double)buf[i].ui;
	  break;
	case SF_LI:
	  Data[i+nc*rowcount].d=(double)buf[i].li;
	  break;
	case SF_SI:
	  Data[i+nc*rowcount].d=(double)buf[i].si;
	  break;
	case SF_I:
	  Data[i+nc*rowcount].d=(double)buf[i].i;
	  break;
	case SF_LF:
	  Data[i+nc*rowcount].d=buf[i].lf;
	  break;
	case SF_F:
	  Data[i+nc*rowcount].d=(double)buf[i].f;
	  break;
	}
    } /* rowcount */
  return 0;
 bad1:
  /* free allocated strings in scan buffer */
  for ( j=0 ; j < MAXSCAN ; j++)
    if ( (type_s[j] ==  SF_C) || (type_s[j] ==  SF_S))  free(buf[j].c);
  
 bad2: 
  return err;
}



static void Free_Scan(int nrow, int ncol, sfdir *type_s, entry **data)
{
  int i,j;
  entry * Data;
  Data=*data;

  if (nrow != 0) {
    for ( j=0 ; j < ncol ; j++)
      if ( (type_s[j] ==  SF_C) || (type_s[j] ==  SF_S) ) 
	/* free allocated strings in scan data area */
	for ( i=0 ; i < nrow ; i++) {
	  free(Data[j+ncol*i].s);
	}
  }
  /* free scaned data area */
  if (ncol>0) free(Data);
}

/********************************************************
 * Converts a Scilab array of  String coded as integer array 
 * into a regular string.
 * entries of the original array are catenated, separated by 
 * '\n'   char
 ********************************************************/

int SciStrtoStr(int *Scistring, int *nstring, int *ptrstrings, char **strh)
{
  char *s,*p;
  int li,ni,*SciS,i,job=1;
  
  li=ptrstrings[0];
  ni=ptrstrings[*nstring] - li + *nstring +1;
  p=(char *) malloc(ni);
  if (p ==NULL)  return MEM_LACK;
  SciS= Scistring;
  s=p;
  for ( i=1 ; i<*nstring+1 ; i++) 
    {
      ni=ptrstrings[i]-li;
      li=ptrstrings[i];
      F2C(cvstr)(&ni,SciS,s,&job,(long int)ni);
      SciS += ni;
      s += ni;
      if (i<*nstring) {
	*s='\n';
	s++;
      }
    }
  *s='\0';
  *strh=p;
  return 0;
}


