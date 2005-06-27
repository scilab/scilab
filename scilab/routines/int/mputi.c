/* Copyright INRIA */
#include <stdio.h>
#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#endif
#include "../machine.h"
#include "../fileio/st.h"

struct soundstream ftf;
extern FILE *GetFile();
extern int GetSwap();
extern void sciprint __PARAMS ((char *fmt,...));

extern int SWAP(char type[],integer *fd); /* Defined in mgeti.c */

#define MPUTI(Type,Fswap) {\
Type *RES;\
Type val; \
RES= (Type *)res;\
for ( i=0; i< *n; i++)  { \
  val=(Type) *RES++; \
  if ( swap) val = Fswap(val); \
  fwrite(&val,sizeof(Type),1,fa); }\
}


void C2F(mputi) (fd,res,n,type,ierr)
     integer *res;
     integer *n,*ierr;
     char type[];
     integer *fd;
{  
  char c1,c2;
  int i,nc,swap;
  FILE *fa;
  ft_t ft;
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
      sciprint("mputi : format is of 0 length  \r\n",type);
      *ierr=1;
      return;
    }
  if (fa)
    { 
      switch ( type[0] )
	{
	case 'l' : 
	  swap=SWAP(type,fd);
	  if(swap<0) {*ierr=1;return;}
	  MPUTI(long,swapl);
	  break;
	case 's' : 
	  swap=SWAP(type,fd);
	  if(swap<0) {*ierr=1;return;}
	  MPUTI(short,swapw);
	  break;
	case 'c' : 
	  for ( i=0; i< *n; i++) 
	    {
	      char val;
	      val =(char) *RES_c++;
	      fwrite(&val,sizeof(char),1,fa);
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
	      switch (c2 )
		{
		case 'l' :
		  /* Write long, big-endian: big end first. 
		     68000/SPARC style. */
		  for ( i=0; i< *n; i++) 
		    {
		      unsigned long val;
		      val =(unsigned long) *RES_ul++;
		      wblong(ft, val);
		    }
		  break;
		case 's' :
		  /* Write short, big-endian: big end first. 
		     68000/SPARC style. */
		  for ( i=0; i< *n; i++) 
		    {
		      unsigned short val;
		      val =(unsigned short) *RES_uc++;
		      wbshort(ft,val);
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
		  /* Write long, little-endian: little end first. 
		     VAX/386 style.*/
		  for ( i=0; i< *n; i++) 
		      {
			unsigned long val;
			val =(unsigned long) *RES_ul++;
			wllong(ft, val);
		      }
		  break;
		case 's' :
		  /* Write short, little-endian: little end first. 
		     VAX/386 style.*/
		  for ( i=0; i< *n; i++) 
		    {
		      unsigned short val;
		      val =(unsigned short) *RES_us++;
		      wlshort(ft,val);
		    }
		  break;
		default: 
		  MPUTI(unsigned long,swapl);
		  break;
		}
	      break;
	    case 's' : 
	      MPUTI(unsigned short,swapw);
	      break;
	    case 'c' :
	      for ( i=0; i< *n; i++) 
		{
		  unsigned char val;
		  val =(unsigned char) *RES_uc++;
		  fwrite(&val,sizeof(unsigned char),1,fa);
		}
	      break;
	    default :
	      sciprint("mputi : %s format not recognized \r\n",type);
	      *ierr=1;
	      return;
	    }
	  break;
	default :
	  sciprint("mputi : %s format not recognized \r\n",type);
	  *ierr=1;
	  return;
	}
      *ierr = ferror(fa);
      return;
    }
  sciprint("No input file \r\n");
  *ierr=1;
}
