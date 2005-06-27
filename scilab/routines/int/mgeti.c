/* Copyright INRIA */
#include <stdio.h>
#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#endif
#include "../machine.h"
#include "../fileio/st.h"

int SWAP(char type[],integer *fd); /* Defined at the end of this file */

struct soundstream ftf;
extern FILE *GetFile();
extern int GetSwap();
extern void sciprint __PARAMS ((char *fmt,...));
extern int islittle_endian(void); /* Defined in SCI/routines/fileio/sound.c */

#define MGETI(Type,Fswap) {\
Type *RES;\
Type val; \
RES=(Type *)res;\
for ( i=0; i< *n; i++)  { \
  if ( fread(&val,sizeof(Type),1,fa) != 1) {items= i;break;};\
  if ( swap) val = Fswap(val);\
  *RES++ = val; }\
}

/*****************************************************************
 * read n items of type type 
 * if read fails *ierr contains the number of properly read items 
 ****************************************************************/



void C2F(mgeti) (fd,res,n,type,ierr)
     integer *res;
     integer *n,*ierr,*fd;
     char type[];
{  
  char c1,c2;
  int i,items=-1,nc,swap;
  ft_t ft;
  FILE *fa;
  char *RES_c;
  unsigned char *RES_uc;
  unsigned long *RES_ul;
  unsigned short *RES_us;

  RES_c=(char *)res;
  RES_uc=(unsigned char *)res;
  RES_ul=(unsigned long *)res;
  RES_us=(unsigned short *)res;

  fa = GetFile(fd);
  swap = GetSwap(fd);
  ft = &ftf; 
  ft->fp = fa;
  nc=strlen(type);
  if ( nc == 0) 
    {
      sciprint("mgeti : format is of 0 length  \r\n",type);
      *ierr=1;
      return;
    }

  if (fa)
    { 
      switch ( type[0] ) {
	case 'l' : 
	  swap=SWAP(type,fd);
	  if(swap<0) {*ierr=1;return;}
	  MGETI(long,swapl);
	  break;
	case 's' : 
	  swap=SWAP(type,fd); 
	  if(swap<0) {*ierr=1;return;}
	  MGETI(short,swapw);
	  break;
	case 'c' :
	  for ( i=0; i< *n; i++) 
	    {
	      char val;
	      if ( fread(&val,sizeof(char),1,fa) != 1) 
		 {items= i;break;}
	      *RES_c++ = val;
	    }
	  break;
	case 'u' :
	  if ( strlen(type) > 1) c1=type[1] ;
	  else c1=' ';

	  switch ( c1 )
	    {
	    case 'b' :
	      if ( strlen(type) > 2) c2=type[2];
	      else c2=' ';
	      switch ( c2)
		{
		case 'l' :
		  /* Read long, big-endian: big end first. 
		     68000/SPARC style. */
		  for ( i=0; i< *n; i++) 
		    {
		      unsigned long val;
		      val = rblong(ft);
		      if ( feof(fa) != 0)  {items= i;break;}
		      *RES_ul ++ = val ;
		    }
		  break;
		case 's' :
		  /* Read short, big-endian: big end first. 
		     68000/SPARC style. */
		  for ( i=0; i< *n; i++) 
		    {
		      unsigned short val;
		      val = rbshort(ft);
		      if ( feof(fa) != 0)  {items= i;break;}
		      *RES_us++ = val;
		    }
		  break;
		}
	      break;
	    case 'l' : 
	      if ( strlen(type) > 2) c2=type[2];
	      else c2=' ';
	      switch ( c2 )
		{
		case 'l' :
		  /* Read long, little-endian: little end first. 
		     VAX/386 style.*/
		  for ( i=0; i< *n; i++) 
		      {
			unsigned long val;
			val = rllong(ft);
			if ( feof(fa) != 0)  {items= i;break;}
			*RES_ul++ = val;
		      }
		  break;
		case 's' :
		  /* Read short, little-endian: little end first. 
		     VAX/386 style.*/
		  for ( i=0; i< *n; i++) 
		    {
		      unsigned short val;
		      val = rlshort(ft);
		      if ( feof(fa) != 0)  {items= i;break;}
		      *RES_us++ = val;
		    }
		  break;
		default: 
		  MGETI(unsigned long,swapl);
		  break;
		}
	      break;
	    case 's' : 
	      MGETI(unsigned short,swapw);
	      break;
	    case 'c' :
	      for ( i=0; i< *n; i++) 
		{
		  unsigned char  val;
		  if ( fread(&val,sizeof(unsigned char),1,fa)!= 1) 
		    {items= i;break;};
		  *RES_uc++ = val;
		}
	      break;
	    default :
	      sciprint("mget : %s format not recognized \r\n",type);
	      *ierr=1;
	      return;
	    }
	  break;
	default :
	  sciprint("mget : %s format not recognized \r\n",type);
	  *ierr=1;
	  return;
	}
      if ( items != -1 ) 
	{
	  *ierr = -(items) -1 ;
	  /** sciprint("Read %d out of \r\n",items,*n); **/
	}
      return;
    }
  sciprint("No input file \r\n");
  *ierr=1;
}
int SWAP(type,fd)
char type[];
integer *fd;
{
int nc,swap;
  nc=strlen(type);
  swap = GetSwap(fd);
  if ( nc > 1) {
    switch (type[1])  {
    case 'b': 
      if (islittle_endian()==1) swap=1;else swap=0; 
      break; 
    case 'l': 
      if (islittle_endian()==1) swap=0;else swap=1; 
      break; 
    default:
      sciprint("mgeti : unknown format %s  \r\n",type);
      swap=-1;
    }
  }
return swap;
}
