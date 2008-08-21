/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <stdint.h> 
#else
#define int32_t long
#define uint32_t unsigned long
#endif

#include "machine.h"
#include "mputi.h"
#include "../../../../libs/libst/misc.h"
#include "sciprint.h"
#include "filesmanagement.h"
#include "localization.h"

extern struct soundstream ftf; /* Defined in mgeti.c */



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


void C2F(mputi) (integer *fd,integer *res,integer *n,char type[],integer *ierr)
{  
  char c1,c2;
  int i,nc,swap;
  FILE *fa = NULL;
  ft_t ft;
  char *RES_c;
  unsigned char *RES_uc;
  uint32_t *RES_ul;
  unsigned short *RES_us;

  RES_c=(char *)res;
  RES_uc=(unsigned char *)res;
  RES_ul=(uint32_t *)res;
  RES_us=(unsigned short *)res;

  fa = GetFileOpenedInScilab(*fd);
  swap = GetSwapStatus(*fd);
  ft = &ftf; 
  ft->fp = fa;
  nc=(int)strlen(type);
  if ( nc == 0) 
    {
      sciprint(_("%s: format is of 0 length.\n"),"mputi", type);
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
	  MPUTI(int32_t,swapl);
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
		      uint32_t val;
		      val =(uint32_t) *RES_ul++;
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
			uint32_t val;
			val =(uint32_t) *RES_ul++;
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
		  MPUTI(uint32_t,swapl);
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
	      sciprint(_("%s: %s format not recognized.\n"),"mputi",type);
	      *ierr=1;
	      return;
	    }
	  break;
	default :
	  sciprint(_("%s: %s format not recognized.\n"),"mputi",type);
	  *ierr=1;
	  return;
	}
      *ierr = ferror(fa);
      return;
    }
  sciprint(_("%s: No input file.\n"),"mputi");
  *ierr=1;
}
