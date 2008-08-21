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
#include "mgeti.h"
#include "sciprint.h"
#include "../../../../libs/libst/misc.h"
#include "islittleendian.h"
#include "filesmanagement.h"
#include "localization.h"

struct soundstream ftf;


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



void C2F(mgeti) (integer *fd,integer *res,integer *n,char type[],integer *ierr)
{  
  char c1,c2;
  int i,items=-1,nc,swap;
  ft_t ft;
  FILE *fa;
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
      sciprint(_("%s: format is of 0 length.\n"),"mgeti",type);
      *ierr=1;
      return;
    }

  if (fa)
    { 
      switch ( type[0] ) {
	case 'l' : 
	  swap=SWAP(type,fd);
	  if(swap<0) {*ierr=1;return;}
	  MGETI(int32_t,swapl);
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
		      uint32_t val;
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
			uint32_t val;
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
		  MGETI(uint32_t,swapl);
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
	      sciprint(_("%s: %s format not recognized.\n"),"mgeti",type);
	      *ierr=1;
	      return;
	    }
	  break;
	default :
	  sciprint(_("%s: %s format not recognized.\n"),"mgeti",type);
	  *ierr=1;
	  return;
	}
      if ( items != -1 ) 
	{
	  *ierr = -(items) -1 ;
	  /** sciprint("Read %d out of\n",items,*n); **/
	}
      return;
    }
  sciprint(_("No input file\n"));
  *ierr=1;
}

int SWAP(char type[],integer *fd)
{
int nc,swap;
  nc=(int)strlen(type);
  swap = GetSwapStatus(*fd);
  if ( nc > 1) {
    switch (type[1])  {
    case 'b': 
      if (islittleendian()==1) swap=1;else swap=0; 
      break; 
    case 'l': 
      if (islittleendian()==1) swap=0;else swap=1; 
      break; 
    default:
      sciprint(_("%s: unknown format %s.\n"),"mgeti",type);
      swap=-1;
    }
  }
return swap;
}
