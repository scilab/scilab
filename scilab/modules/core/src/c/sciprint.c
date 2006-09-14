/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#if defined (__STDC__) || defined(_MSC_VER)
  #include <stdarg.h>
#else
  #include <varargs.h>
#endif 

#include <stdio.h>

#if _MSC_VER
#include "../../gui/src/c/wsci/wgnuplib.h"
#endif

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
  #define vsnprintf _vsnprintf 
  TW textwin;
#endif
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
  extern int PutString(char *Str);
#else
  extern int C2F(xscisrn)();
#endif
extern int getdiary __PARAMS(());
extern int C2F(xscion)();
extern void diary_nnl __PARAMS((char *str,int *n));
/*-----------------------------------------------------------------------------------*/ 
#define MAXPRINTF 512
/*-----------------------------------------------------------------------------------*/ 
#if defined(__STDC__) || defined(_MSC_VER)
  void  sciprint(char *fmt,...) 
#else 
  void sciprint(va_alist) va_dcl
#endif 
{
	int i;
	integer lstr;
	va_list ap;
	char s_buf[MAXPRINTF];
	int count=0;
#if defined(__STDC__) || defined(_MSC_VER)
	va_start(ap,fmt);
#else
	char *fmt;
	va_start(ap);
	fmt = va_arg(ap, char *);
#endif

#if defined(linux) || defined(_MSC_VER)
	count = vsnprintf (s_buf,MAXPRINTF-1, fmt, ap );
	if (count == -1)
	{
		s_buf[MAXPRINTF-1]='\0';
	}
#else
	(void )vsprintf(s_buf, fmt, ap );
#endif
	lstr=strlen(s_buf);

	C2F(xscion)(&i);
	if (i == 0) 
	{
		printf("%s",s_buf); 
	}
	else 
	{
		#if _MSC_VER
		 TextPutS (&textwin,s_buf);
		 PutString(s_buf);
		#else
		 C2F(xscisrn)(s_buf,&lstr,0L);
		#endif
		
	}
	if (getdiary()) diary_nnl(s_buf,&lstr);
	va_end(ap);
}
/*-----------------------------------------------------------------------------------*/ 
/* as sciprint but with an added first argument which is ignored (used in do_printf) */
#if _MSC_VER
  int sciprint2 (int iv, char *fmt,...)
  {
	  int i, count,lstr;
	  va_list ap;
	  char s_buf[MAXPRINTF];
	  va_start (ap, fmt);
	  /* next three lines added for diary SS */
	  count = _vsnprintf (s_buf,MAXPRINTF-1, fmt, ap);
	  if (count == -1)
	  {
		  s_buf[MAXPRINTF-1]='\0';
	  }

	  lstr = strlen (s_buf);

	  C2F (xscion) (&i);
	  if (i == 0)
	  {
		  printf ("%s", s_buf);
	  }
	  else
	  {
		  TextPutS (&textwin, s_buf);
		  PutString(s_buf);
	  }
	  if (getdiary()) diary_nnl(s_buf,&lstr);

	  va_end (ap);
	  return count;
  }
#else
#if defined(__STDC__)
  int  sciprint2(int iv,char *fmt,...) 
#else 
  int sciprint2(va_alist) va_dcl
#endif 
  {
	  int i,retval;
	  integer lstr;
	  va_list ap;
	  char s_buf[MAXPRINTF];
	  int count=0;
#if defined(__STDC__)
	  va_start(ap,fmt);
#else
	  int iv;
	  char *fmt;
	  va_start(ap);
	  iv = va_arg(ap,int);
	  fmt = va_arg(ap, char *);
#endif

	  C2F(xscion)(&i);
	  if (i == 0) 
	  {
		  retval= vfprintf(stdout, fmt, ap );
	  }
	  else 
	  {
#ifdef linux
		  retval= vsnprintf (s_buf,MAXPRINTF-1, fmt, ap);
		  if (retval == -1)
		  {
			  s_buf[MAXPRINTF-1]='\0';
		  }
#else
		  retval= vsprintf(s_buf, fmt, ap );
#endif

		  lstr=strlen(s_buf);
		  C2F(xscisncr)(s_buf,&lstr,0L);
	  }

	  if (getdiary()) 
	  {
#ifdef linux
		  retval= vsnprintf (s_buf,MAXPRINTF-1, fmt, ap);
		  if (retval == -1)
		  {
			  s_buf[MAXPRINTF-1]='\0';
		  }
#else
		  retval= vsprintf(s_buf, fmt, ap );
#endif
		  lstr=strlen(s_buf);
		  diary_nnl(s_buf,&lstr);
	  }

	  va_end(ap);
	  return retval;

  }
#endif  
/*-----------------------------------------------------------------------------------*/ 
