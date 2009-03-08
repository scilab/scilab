/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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
/* LineRead procedure reads lines in ascii files coming from Unix Dos or Mac 
 * End of line may be \Cr\lf (Dos) \Lf (Unix) \Cr (Mac)
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "readline.h"
#include "filesmanagement.h"
#include "charEncoding.h"
#include "stack-def.h"
/*--------------------------------------------------------------------------*/
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif 
/*--------------------------------------------------------------------------*/
#define CR 13
#define LF 10
/*--------------------------------------------------------------------------*/
/**
* @params cnt : number of charaters returned in buf 
* @params nr : number of characters read 
*/
int LineRead(FILE *fd,char buf[],int n,int *cnt,int *nr)
{
  int c,count,info;
  long int offset;
  char *tmpChar = NULL;
	char szTemp[bsiz];
  count=0;
  *nr=0;
  
  while (1) 
  {
    c=fgetc(fd);
    *nr=*nr+1;
    if (c==LF) 
	{ 
		/* LF reached first: unix file */
		buf[count++]=(char)0;
		info=1; /* EOL reached */
		break;
	}
    else if (c==CR) 
	{
		/* CR reached first: Dos or Mac file */
		c=fgetc(fd);
		*nr=*nr+1;
		if (c==EOF) 
		{
			buf[count++]=(char)0;
			info=0;/* EOF reached after an EOL */
			break;
		}
		else if (c==LF) 
		{ 
			/* LF after CR : Dos file */
			buf[count++]=(char)0;
			info=1; /* EOL reached */
			break;
		}
		else if (c!=LF) 
		{
			/* Mac file */
			offset=-1;
			fseek(fd,offset,SEEK_CUR);
			*nr=*nr-1;
			buf[count++]=(char)0;
			info=1;  /* EOL reached */
			break;
		}
    }
    else if (c==EOF) 
	{
		/* EOF reached before any EOL*/
		buf[count++]=(char)0;
		if (count==1) 
			info=-1; /* EOF reached */
		else
			info=3;
		break;
    }
    else 
	{
      buf[count++]=(char)c;
      if (count==n-1) 
	  {
		buf[count++]=(char)0;
		info=2; /* buffer full */
		break;
      }
    }
  }
  *cnt=count;
  /** Convert from system locale encoding to UTF encoding */
  tmpChar = localeToUTF(buf, szTemp);
  *cnt = (int)strlen(tmpChar)+1;
  strcpy(buf,tmpChar);
  return(info);
}
/*--------------------------------------------------------------------------*/ 
void C2F(readnextline)(int *fd,char buf[],int *n,int *count,int *nr,int *ierr)
{
	FILE *fa= GetFileOpenedInScilab(*fd);

	*ierr=LineRead(fa,buf,*n,count,nr);
}
/*--------------------------------------------------------------------------*/
