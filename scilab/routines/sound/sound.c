/*********************************************
 * A set of functions for reading and writing binary 
 * datas in a machine independant way 
 * using functions from a sound library 
 * Copyright ENPC/ Chancelier 
 *********************************************/

#include <stdio.h>
#ifdef __STDC__
#include <stdlib.h>
#endif
#include <string.h>
#include <math.h>
#ifdef __STDC__
#include <float.h>
#endif
#include <limits.h>
#include <ctype.h>

#include "../graphics/Math.h"

#include "st.h"


#if (defined(sun) && !defined(SYSV)) 
char *strerror __PARAMS((int errcode));
#endif


static int swap = 0;

#define MAXF 20
static int CurFile =-1;
static struct soundstream ftf;

static FILE *ftformat[MAXF]={(FILE*) 0}; 
static int ftswap[MAXF]={0}; /** swap status for each file **/
static int ftmode[MAXF]={0}; /** mode for each file **/
static int fttype[MAXF]={0}; /** type (Fortran,C) for each file must
                                 be zero initialized **/
static char * ftname[MAXF]={0};/** name for each file **/



/**************************************
 * returns the first available position
 * in File array (ftformat) or -1 
 * if no more position is available
 **************************************/

void C2F(getfiledesc)(fd) 
     integer *fd;
{
  int i ;
  for ( i = 1 ; i < MAXF ; i++) {
    if ( fttype[i] == 0 ) {
      *fd = i;
      return ;
    }
  }
  *fd = -1;
}

/**************************************
 * add a file in the files table
 **************************************/

void C2F(addfile)(fd,fa,swap,type,mode,filename,ierr)
     integer *fd;
     FILE * fa;
     integer *swap;
     integer *type;
     integer *mode;
     integer *ierr;
     char* filename;
{
  char* name;
  name= (char *) malloc((strlen(filename)+1)*sizeof(char));
  if ( name == (char *) 0) {
    *ierr=1;
    return;
  }
  if (*type==2) 
    ftformat[*fd] = fa;
  else if (*type==-1) 
    ftformat[*fd] = stdin;
  else if (*type==-2) 
    ftformat[*fd] = stdout;
  else if (*type==-3) 
    ftformat[*fd] = stderr;
  else
    ftformat[*fd] = (FILE *) 0;
  ftswap[*fd] = *swap;
  fttype[*fd] = *type;
  ftmode[*fd] = *mode;
  ftname[*fd] = name;
  strcpy(name,filename);
  *ierr=0;
}

/**************************************
 * get file info in the files table
 **************************************/

void C2F(getfileinfo)(fd,fa,swap,type,mode,filename,lf,ierr)
     integer *fd;
     FILE * fa;
     integer *swap;
     integer *type;
     integer *mode;
     integer *ierr;
     integer *lf;
     char* filename;
{
  if (*fd<0 || *fd>=MAXF ) {
    *ierr=1;
    return;
  }
  if ( fttype[*fd] == 0 ) {
    *ierr=2;
    return;
  }
  fa = ftformat[*fd];
  *swap = ftswap[*fd];
  *type = fttype[*fd];
  *mode = ftmode[*fd];
  strcpy(filename,ftname[*fd]);
  *lf=strlen(filename);
  *ierr=0;
}
/**************************************
 * get file type (C or Fortran)
 **************************************/

void C2F(getfiletype)(fd,type,ierr)
     integer *fd;
     integer *type;
     integer *ierr;
 
{
  if (*fd<0 || *fd>=MAXF ) {
    *ierr=1;
    return;
  }
  if ( fttype[*fd] == 0 ) {
    *ierr=2;
    return;
  }
  *type = fttype[*fd];
  *ierr=0;
}


/**************************************
 * delete a file in the files table
 **************************************/

void C2F(delfile)(fd)
     integer *fd;
{
  if (*fd>=0 && *fd<MAXF ) 
    {
      ftformat[*fd] = (FILE*) 0;
      ftswap[*fd] = 0;
      fttype[*fd] = 0;
      ftmode[*fd] = 0;
      if ( ftname[*fd]!=  (char *)0) free(ftname[*fd]);
      ftname[*fd] = (char *)0;
    }
}
/**************************************
 * convert mopen status to an integer 
 **************************************/

int Status2Mode(status)
     char *status;
{
  int bin, plus, md, n, k;
  /* default values */
  bin = 0;
  plus = 0;
  md = 0;
  n=strlen(status);
  for (k=0;k<n;k++){
    if (status[k]=='r') md = 1;
    else if (status[k]=='w') md = 2;	
    else if (status[k]=='a') md = 3;	
    else if (status[k]=='+') plus = 1;
    else if (status[k]=='b') bin = 1;
  }
  return(100*md+10*plus+bin);
}


/**************************************
 * returns the file associated to int fd 
 * or NULL if no such file. 
 **************************************/

FILE *GetFile(fd)
     integer *fd;
{
  int fd1;
  fd1 = (*fd != -1) ?  Min(Max(*fd,0),MAXF-1) : CurFile ;
  if ( fd1 != -1 ) 
    return(ftformat[fd1]);
  else
    return((FILE *) 0);
}

/**************************************
 * returns the swap status of file fd 
 **************************************/

int GetSwap(fd)
     integer *fd;
{
  int fd1;
  fd1 = (*fd != -1) ?  Min(Max(*fd,0),MAXF-1) : CurFile ;
  if ( fd1 != -1 ) 
    return(ftswap[fd1]);
  else
    return(0);
}


/*********************************************
 * check machine status (little or big endian)
 *********************************************/

int islittle_endian()
{
  int	littlendian = 1;
  char	*endptr;
  endptr = (char *) &littlendian;
  return (int) *endptr;
}

int C2F(getendian)()
{
  return islittle_endian();
}

/*********************************************
 * Opens file given by file and return it's id 
 * in fd. 
 * status can be "r","w","a" or "rb","wb","ab"
 *********************************************/

void C2F(mopen)(fd,file,status,f_swap,res,error)
     int *error,*f_swap;
     char *file,*status;
     double *res;
     int *fd;
{   
  int	littlendian = 1,type = 2,ierr,mode;
  char	*endptr;
  FILE * fa;
  /* next line added by ss 16/10/98 */
  swap =0;
  *error=0;
  endptr = (char *) &littlendian;
  if ( (!*endptr) )
    {
      if( *f_swap == 1 ) 
	swap = 1;
      else 
	swap =0;
    }
  /*  sciprint("    Byte Swap status %d \r\n",swap);*/

  C2F(getfiledesc)(fd);
  if ( *fd == -1 )
    {
      *error=1; /* Too many opened files */
      return;
    }
  fa=fopen(file,status);
  if (! fa )
    {     
      *error=2; /* Could not open the file*/
      return;
    }
  mode=Status2Mode(status);
  C2F(addfile)(fd,fa,&swap,&type,&mode,file,&ierr);
  if (ierr)
    {
      *error=3; /* Not enough memory;*/
      return;
    }
  CurFile = *fd;
  *res = (double)ferror(fa);
}

/**************************************
 * close the file with id *fd if *id != -1 and *id != -2
 * the current file if *id = -1 
 * all opened file if *id = -2 
 **************************************/

void C2F(mclose) (fd,res)
     double *res;
     integer *fd;
{     
  int fd1;
  int res1 = 1;
  *res = 0.0;
  switch ( *fd )
    {
    case -2 :
      /* closing all opened files */
      for ( fd1=0; fd1< MAXF; fd1++) {
	if ( ftformat[fd1] )
	  {
	    fclose( ftformat[fd1] );
	    res1 = ferror( ftformat[fd1]);
	    if (res1 != 0) *res =1;
	    C2F(delfile)(&fd1);
	  }
      }
      break;
    default :
      fd1 = (*fd == -1 ) ? CurFile : Min(Max(*fd,0),MAXF-1);
      if ( fd1 != -1 ) 
	{
	  if ( ftformat[fd1] )
	    {
	      if (fclose(ftformat[fd1])) {
		*res = (double)ferror(ftformat[fd1]);
	      }
	      C2F(delfile)(&fd1);
	    }
	  else
	    {
	      *res = 0.0;
	      sciprint("File %d not active \r\n",fd1);
	    }
	}
      else 
	{
	  *res = -1.0;
	  sciprint("No file to close \r\n",fd1);
	}
    }
}

/*********************************************
 * check eof 
 *********************************************/


void C2F(meof) (fd,res)
     double *res;
     integer *fd;
{       
  FILE *fa= GetFile(fd);
  *res = fa ? feof(fa) : 1;
}

/*********************************************
 * clear errors 
 *********************************************/


void C2F(mclearerr) (fd)
     integer *fd;
{       
  FILE *fa= GetFile(fd);
  clearerr(fa);
}

/*********************************************
 * seek function 
 *********************************************/

#if (defined(sun) && !defined(SYSV)) || defined(sgi)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif 

void C2F(mseek) (fd,offset,flag,err)
     integer *fd,*offset,*err;
     char *flag;
{     
  int iflag;
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
  int irep;
#endif
  FILE *fa= GetFile(fd);
  *err=0;
  if ( fa == (FILE *) 0 ) 
    {
      sciprint("mseek: wrong file logical unit \r\n");
      *err=1;
      return;
    }
  if ( strncmp(flag,"set",3)==0 ) 
    iflag = SEEK_SET;
  else if ( strncmp(flag,"cur",3)==0 ) 
    iflag = SEEK_CUR;
  else if ( strncmp(flag,"end",3)==0 ) 
    iflag = SEEK_END;
  else 
    {
      sciprint("mseek : flag = %s not recognized \r\n");
      *err=1;
      return;
    }
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
  irep = fseek(fa,(long) *offset,iflag) ;
  if ( irep != 0 ) 
    {
      sciprint(strerror(irep));
      *err=1;
    }
  else
    *err=0;
#else
  if (fseek(fa,(long) *offset,iflag) == -1 ) 
    {
      sciprint("mseek: error\r\n");
      *err=1;
    }
  else 
    *err=0;
#endif
}

/*********************************************
 * tell function 
 *********************************************/

void C2F(mtell) (fd,offset,err)
     integer *fd,*err;
     double *offset;
{     
  FILE *fa= GetFile(fd);
  if ( fa == (FILE *) 0 ) 
    {
      sciprint("mtell: wrong file logical unit \r\b");
      *err=1;
      return;
    }
  *err=0;
  *offset = (double) ftell(fa) ;
}



/*********************************************
 * mput functions  write data in a machine 
 * independant way (i.e write in little-endian) 
 * or can be used to write in little or big endian 
 *********************************************/


/*===============================================
 * function to write data without type conversion
 *===============================================*/
#define MPUT_CHAR_NC(Type) \
{\
   Type *val = (Type *) res ; \
   fwrite(val,sizeof(Type),n,fa); \
}
		
#define MPUT_NC(Type,Fswap) \
{ \
   Type *val = (Type *) res ; \
   Type vali; \
   for ( i=0; i< n; i++)  \
           { \
	      vali = *val++; \
	      if ( swap) vali = Fswap(vali); \
	      fwrite(&vali,sizeof(Type),1,fa); \
	    } \
}
#define MPUT_GEN_NC(Type,Fswap,cf) \
      switch ( cf )  \
	{ \
	        case ' ': MPUT_NC(Type,Fswap); break; \
		case 'b': \
		  swap = (islittle_endian()==1) ? 1 : 0; \
		  MPUT_NC(Type,Fswap); break; \
		case 'l': \
		  swap = (islittle_endian()==1) ? 0 : 1; \
		  MPUT_NC(Type,Fswap); break; \
		default: \
		  sciprint("mput : %s format not recognized \r\n",type); \
		  *ierr=1;return; \
				    }
void C2F(mputnc) (fd,res,n1,type,ierr)
     void *res;
     integer *n1,*ierr,*fd;
     char type[];
{  
  char c1,c2;
  int i,swap,n;
  FILE *fa;
  n=*n1;
  *ierr=0;
  if ((fa = GetFile(fd)) ==NULL) {
    sciprint("No input file associated to logical unit %d\r\n",*fd);
    *ierr=3;
    return;
  }
  swap = GetSwap(fd);

  c1 = ( strlen(type) > 1) ? type[1] : ' '; 
  c2 = ( strlen(type) > 2) ? type[2] : ' '; 
  switch ( type[0] )
    {
    case 'i' : MPUT_GEN_NC(int,swapi,c1);       break;
    case 'l' : MPUT_GEN_NC(long,swapl,c1);      break;
    case 's' : MPUT_GEN_NC(short,swapw,c1);     break;
    case 'c' : MPUT_CHAR_NC(char) ;          break;
    case 'd' : MPUT_GEN_NC(double,swapd,c1);    break;
    case 'f' : MPUT_GEN_NC(float,swapf,c1);     break;
    case 'u' :
      switch ( c1 )
	{
	case 'i' :  MPUT_GEN_NC(unsigned int,swapi,c2); break;
	case 'l' :  MPUT_GEN_NC(unsigned long,swapl,c2); break;
	case 's' :  MPUT_GEN_NC(unsigned short,swapw,c2); break;
	case ' ' :  MPUT_GEN_NC(unsigned int,swapi,' '); break;
	case 'c' :  MPUT_CHAR_NC(unsigned char); break;
	default :  *ierr=1;return ;
	}
      break;
    default : *ierr=1; break;
    }
}
/*================================================
 * function to write data stored in double
 *================================================*/
/** used for char **/
#define MPUT_CHAR(Type) \
      for ( i=0; i< n; i++)  \
	{ \
	    Type  val = (char) *res++; \
	    fwrite(&val,sizeof(Type),1,fa); \
        }

/** write in a machine independant way : i.e data 
    is swaped if necessary to output little-endian 
    data **/
		
#define MPUT(Type,Fswap) for ( i=0; i< n; i++)  \
           { \
	      Type val; \
	      val =(Type) *res++; \
	      if ( swap) val = Fswap(val); \
	      fwrite(&val,sizeof(Type),1,fa); \
	    }

/** The output mode is controlled by type[1] **/
#define MPUT_GEN(Type,Fswap,cf) \
      switch ( cf )  \
	{ \
	        case ' ': MPUT(Type,Fswap); break; \
		case 'b': \
		  swap = (islittle_endian()==1) ? 1 : 0; \
		  MPUT(Type,Fswap); break; \
		case 'l': \
		  swap = (islittle_endian()==1) ? 0 : 1; \
		  MPUT(Type,Fswap); break; \
		default: \
		  sciprint("mput : %s format not recognized \r\n",type); \
		  *ierr=1;return; \
				    }

void mput2 (fa,swap,res,n,type,ierr)
     double *res;
     integer n,*ierr,swap;
     char type[];
     FILE *fa;
{  
  char c1,c2;
  int i;
  ft_t ft = &ftf;
  *ierr=0;
  ft->fp = fa;
  c1 = ( strlen(type) > 1) ? type[1] : ' '; 
  c2 = ( strlen(type) > 2) ? type[2] : ' '; 
  switch ( type[0] )
    {
    case 'i' : MPUT_GEN(int,swapi,c1);       break;
    case 'l' : MPUT_GEN(long,swapl,c1);      break;
    case 's' : MPUT_GEN(short,swapw,c1);     break;
    case 'c' : MPUT_CHAR(char) ;          break;
    case 'd' : MPUT_GEN(double,swapd,c1);    break;
    case 'f' : MPUT_GEN(float,swapf,c1);     break;
    case 'u' :
      switch ( c1 )
	{
	case 'i' :  MPUT_GEN(unsigned int,swapi,c2); break;
	case 'l' :  MPUT_GEN(unsigned long,swapl,c2); break;
	case 's' :  MPUT_GEN(unsigned short,swapw,c2); break;
	case ' ' :  MPUT_GEN(unsigned int,swapi,' '); break;
	case 'c' :  MPUT_CHAR(unsigned char); break;
	default :  *ierr=1;return ;
	}
      break;
    default : *ierr=1; break;
    }
}

void C2F(mput) (fd,res,n,type,ierr)
     double *res;
     integer *n,*ierr,*fd;
     char type[];
{
  int nc,swap;
  FILE *fa;
  *ierr=0;
  if ((nc = strlen(type)) == 0) 
    {
      sciprint("mput : format is of length 0\r\n",type);
      *ierr=2;
      return;
    }
  if ((fa = GetFile(fd)) !=NULL)
    {
      swap = GetSwap(fd);
      mput2(fa,swap,res,*n,type,ierr);
      if (*ierr > 0)
	sciprint("mput : %s format not recognized \r\n",type);
    }
  else {
    sciprint("No input file associated to logical unit %d\r\n",*fd);
    *ierr=3;
  }
}



/*****************************************************************
 * mget functions  read data in a machine 
 * independant way (i.e write in little-endian) 
 * or can be used to read in little or big endian 
 * if read fails *ierr contains the number of properly read items 
 *****************************************************************/


/* =================================================
 * reads data and store them without type conversion 
 * =================================================*/
#define MGET_CHAR_NC(Type) \
{\
   Type *val = (Type *) res ;\
   items=fread(val,sizeof(Type),n,fa);\
}

#define MGET_NC(Type,Fswap) \
{\
   Type *val = (Type *) res ;\
   items=fread(val,sizeof(Type),n,fa);\
   if (swap) \
      for (i=0;i<items;i++) \
         val[i]=Fswap(val[i]);\
}


#define MGET_GEN_NC(NumType,Fswap,cf)\
{ \
    switch (cf) { \
          case ' ': MGET_NC(NumType,Fswap);break; \
	  case 'b': swap = (islittle_endian()==1)? 1:0; \
			    MGET_NC(NumType,Fswap); break; \
  	  case 'l': swap = (islittle_endian()==1) ? 0:1; \
		  MGET_NC(NumType,Fswap);  break; \
   	  default: sciprint("mget : %s format not recognized \r\n",type); \
	     *ierr=1; return; \
                } \
}
void C2F(mgetnc)(fd,res,n1,type,ierr)
     void *res;
     integer *n1,*ierr,*fd;
     char type[];
{  
  char c1,c2;
  int i,items=*n1,n=*n1;
  FILE *fa;


  *ierr=0;
  if ((fa = GetFile(fd)) ==NULL) {
    sciprint("No input file associated to logical unit %d\r\n",*fd);
    *ierr=3;
    return;
  }
  swap = GetSwap(fd);
  c1 = ( strlen(type) > 1) ? type[1] : ' '; 
  c2 = ( strlen(type) > 2) ? type[2] : ' '; 
  switch ( type[0] )
    {
    case 'i' : MGET_GEN_NC(int,swapi,c1);break;
    case 'l' : MGET_GEN_NC(long,swapl,c1);break;
    case 's' : MGET_GEN_NC(short,swapw,c1);break;
    case 'c' : MGET_CHAR_NC(char) ; break;
    case 'd' : MGET_GEN_NC(double,swapd,c1);break;
    case 'f' : MGET_GEN_NC(float,swapf,c1);break;
    case 'u' :
      switch ( c1 )
	{
	case 'i' :  MGET_GEN_NC(unsigned int,swapi,c2); break;
	case 'l' :  MGET_GEN_NC(unsigned long,swapl,c2); break;
	case 's' :  MGET_GEN_NC(unsigned short,swapw,c2); break;
	case ' ' :  MGET_GEN_NC(unsigned int,swapi,' '); break;
	case 'c' :  MGET_CHAR_NC(unsigned char); break;
	default : *ierr=1; return; break;
	}
      break;
    default :
      *ierr=1;
      return ;
    }
  if ( items != n ) 
    {
      *ierr = -(items) -1 ;
      /** sciprint("Read %d out of \r\n",items,n); **/
    }
  return;
}

/* =================================================
 * reads data and store them in double
 * =================================================*/

/* conversion macro  */
#define CONVGD(Type) \
{\
   Type *val = (Type *) res ;\
   for ( i = items-1 ; i >=0 ; i--)  \
     res[i] = val[i]; \
}

#define MGET_GEN(NumType,Fswap,cf)  MGET_GEN_NC(NumType,Fswap,cf); CONVGD(NumType);
#define MGET_CHAR(NumType)  MGET_CHAR_NC(NumType); CONVGD(NumType); 

/* reads data and store them in double  */
void mget2(fa,swap,res,n,type,ierr)
     double *res;
     integer n,*ierr,swap;
     char type[];
     FILE *fa;
{  
  char c1,c2;
  int i,items=n;
  ft_t ft = &ftf; 
  *ierr=0;
  ft->fp = fa;
  c1 = ( strlen(type) > 1) ? type[1] : ' '; 
  c2 = ( strlen(type) > 2) ? type[2] : ' '; 
  switch ( type[0] )
    {
    case 'i' : MGET_GEN(int,swapi,c1);break;
    case 'l' : MGET_GEN(long,swapl,c1);break;
    case 's' : MGET_GEN(short,swapw,c1);break;
    case 'c' : MGET_CHAR(char) ; break;
    case 'd' : MGET_GEN(double,swapd,c1);break;
    case 'f' : MGET_GEN(float,swapf,c1);break;
    case 'u' :
      switch ( c1 )
	{
	case 'i' :  MGET_GEN(unsigned int,swapi,c2); break;
	case 'l' :  MGET_GEN(unsigned long,swapl,c2); break;
	case 's' :  MGET_GEN(unsigned short,swapw,c2); break;
	case ' ' :  MGET_GEN(unsigned int,swapi,' '); break;
	case 'c' :  MGET_CHAR(unsigned char); break;
	default : *ierr=1; return; break;
	}
      break;
    default :
      *ierr=1;
      return ;
    }
  if ( items != n ) 
    {
      *ierr = -(items) -1 ;
      /** sciprint("Read %d out of \r\n",items,n); **/
    }
  return;
}

void C2F(mget) (fd,res,n,type,ierr)
     double *res;
     integer *n,*ierr,*fd;
     char type[];
{  

  int nc,swap;
  FILE *fa;
  nc=strlen(type);
  *ierr=0;
  if ( nc == 0) 
    {
      sciprint("mget : format is of length 0\r\n",type);
      *ierr=2;
      return;
    }
  fa = GetFile(fd);
  if (fa ) 
    {
      swap = GetSwap(fd);
      mget2(fa,swap,res,*n,type,ierr);
      if (*ierr > 0)
	sciprint("mget : %s format not recognized \r\n",type);
    }
  else {
    sciprint("No input file associated to logical unit %d\r\n",*fd);
    *ierr=3;
  }
}



/*********************************************
 * read a string 
 *********************************************/

void C2F(mgetstr) (fd,start,n,ierr)

     char **start;
     integer *ierr,*n,*fd;
{ 
  int count;
  FILE *fa;
  fa = GetFile(fd);
  *ierr=0;
  if (fa)
    { 
      *start= (char *) malloc((*n+1)*sizeof(char));
      if ( *start == (char *) 0)
 	{       
	  sciprint("No more memory \r\n");
	  *ierr=1;
	  return;
	}
      count=fread(*start,sizeof(char),*n,fa);
      (*start)[*n]='\0';
      if ( count != *n ) 
	{
	  *ierr = - count -1;
	}
      return;
    }
  sciprint("No input file \r\n");
  *ierr=1;
}

void C2F(mgetstr1) (fd,start,n,ierr)
     char *start;
     integer *ierr,*n,*fd;
{ 
  int count;
  FILE *fa;
  fa = GetFile(fd);
  *ierr=0;
  if (fa)
    { 
      count=fread(start,sizeof(char),*n,fa);
      start[*n]='\0';
      if ( count != *n ) 
	{
	  *ierr = - count -1;
	}
      return;
    }
  sciprint("No input file \r\n");
  *ierr=1;
}

/*********************************************
 * write a string 
 *********************************************/

void C2F(mputstr) (fd,str,res,ierr)
     char *str;
     double *res;
     int *ierr,*fd;
{   
  FILE *fa;
  fa = GetFile(fd);
  if (!fa) 
    {
      sciprint("No input file \r\n");
      *ierr=1;
    }
  else 
    {
      fprintf(fa,"%s",str);
      (*res) = (*ierr) =ferror(fa);
    }
}


