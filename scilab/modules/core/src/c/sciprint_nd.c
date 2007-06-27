/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Allan CORNET */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "sciprint_nd.h"
#include <stdarg.h>
#ifdef _MSC_VER
#include "../../gui/src/c/wsci/wtext.h"
#endif
#include "xscion.h"

#define MAXPRINTF 512

void sciprint_nd(char *fmt,...) 
{
	int i, count=0, lstr;
  va_list args;
  char buf[MAXPRINTF];
  va_start(args,fmt);
#if defined(linux) || defined(_MSC_VER)
	count = vsnprintf (buf,MAXPRINTF-1, fmt, args);
	if (count == -1)
	{
		buf[MAXPRINTF-1]='\0';
	}
#else
	(void ) vsprintf(buf, fmt, args );
#endif
  lstr=strlen(buf);

  C2F(xscion)(&i);
  if (i == 0) 
    {
      printf("%s",buf); 
    }
  else 
    {
#ifdef _MSC_VER
	TextPutS (getTextWin(), buf);
#else
    C2F(xscisrn)(buf,&lstr,0L);
#endif

    }
  va_end(args);
}

