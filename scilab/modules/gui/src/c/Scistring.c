/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "Scistring.h"
#include "xscion.h"
#ifndef _MSC_VER 
#include "x_charproc.h"
#endif

void Scistring(char *str)
{
  int i;
  int n;
  C2F(xscion)(&i);
  if (i == 0) 
      fprintf(stdout,"%s",str);
  else {
#ifdef _MSC_VER
		TextPutS (&textwin, str);
		MyPutCh ('\n');
#else
		n=strlen(str);
		C2F(xscistring)(str,&n,0L);
#endif
  }
}
