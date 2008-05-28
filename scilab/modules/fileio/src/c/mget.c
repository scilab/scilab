/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "mget.h"
#include "filesmanagement.h"
#include "../../../libs/libst/misc.h"
#include "sciprint.h"
#include "islittleendian.h"
#include "machine.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
struct soundstream ftf;
int swap = 0;
/*--------------------------------------------------------------------------*/
/* =================================================
* reads data and store them without type conversion 
* =================================================*/
#define MGET_CHAR_NC(Type) \
{\
	Type *val = (Type *) res ;\
	items=(int)fread(val,sizeof(Type),n,fa);\
}
/*--------------------------------------------------------------------------*/
#define MGET_NC(Type,Fswap) \
{\
	Type *val = (Type *) res ;\
	items=(int)fread(val,sizeof(Type),n,fa);\
	if (swap) \
	for (i=0;i<items;i++) \
	val[i]=Fswap(val[i]);\
}
/*--------------------------------------------------------------------------*/
#define MGET_GEN_NC(NumType,Fswap,cf)\
{ \
	switch (cf) { \
		case ' ': \
		  MGET_NC(NumType,Fswap);break; \
		case 'b': \
		  swap = (islittleendian()==1)? 1:0; \
		  MGET_NC(NumType,Fswap); break; \
	  case 'l': \
	    swap = (islittleendian()==1) ? 0:1; \
	    MGET_NC(NumType,Fswap);  break; \
	  default: \
	  sciprint(_("%s: Wrong value for input argument #%d: '%s' or '%s' or '%s' expected.\n"),"mget",4," ","b","l"); \
	    *ierr=1; return; \
	} \
}
/*--------------------------------------------------------------------------*/
void C2F(mgetnc)(integer *fd, void * res, integer *n1, char *type, integer *ierr)
{  
	char c1,c2;
	int i,items=*n1,n=*n1;
	FILE *fa;


	*ierr=0;
	if ((fa = GetFileOpenedInScilab(*fd)) ==NULL) {
		sciprint(_("%s: No input file associated to logical unit %d.\n"),"mget",*fd);
		*ierr=3;
		return;
	}
	swap = GetSwapStatus(*fd);
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
		/** sciprint("Read %d out of\n",items,n); **/
	}
	return;
}
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
#define MGET_GEN(NumType,Fswap,cf)  MGET_GEN_NC(NumType,Fswap,cf); CONVGD(NumType);
#define MGET_CHAR(NumType)  MGET_CHAR_NC(NumType); CONVGD(NumType); 
/*--------------------------------------------------------------------------*/
/* reads data and store them in double  */
void mget2(FILE *fa, integer swap2, double *res, integer n, char *type, integer *ierr)
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
		/** sciprint("Read %d out of\n",items,n); **/
	}
	return;
}
/*--------------------------------------------------------------------------*/
void C2F(mget) (integer *fd, double *res, integer *n, char *type, integer *ierr)
{  
	int nc,swap2;
	FILE *fa;
	nc=(int)strlen(type);
	*ierr=0;
	if ( nc == 0) 
	{
		sciprint(_("%s: Wrong size for input argument #%d: Non-empty string expected.\n"),"mput",4,type);
		*ierr=2;
		return;
	}
	fa = GetFileOpenedInScilab(*fd);
	if (fa ) 
	{
		swap2 = GetSwapStatus(*fd);
		mget2(fa,swap2,res,*n,type,ierr);
		if (*ierr > 0) sciprint(_("%s: Wrong value for input argument #%d: Format not recognized.\n"),"mget",4);
	}
	else 
	{
		sciprint(_("%s: No input file associated to logical unit %d.\n"),"mget",*fd);
		*ierr=3;
	}
}
/*--------------------------------------------------------------------------*/
