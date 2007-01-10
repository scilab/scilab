/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/* Francois VOGEL  sciprint_full function */
/* HUANG Xu */
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
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
  #define vsnprintf _vsnprintf
  extern char *QueryStringError(char *Tag);
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
/* any string of length greater than MAXPRINTF gets truncated in sciprint */
#define MAXPRINTF 512
/* MAXCHARSSCIPRINT_FULL is for sciprint_full - more than this gets truncated */
#define MAXCHARSSCIPRINT_FULL 5000  /* */
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
//	count = vsnprintf (s_buf,MAXPRINTF-1, fmt, ap );
{
	#if _MSC_VER
		char *LocalizedString=QueryStringError(fmt);
		if (LocalizedString)
		{
			count= vsnprintf(s_buf,MAXPRINTF-1, LocalizedString, ap );
			if (LocalizedString) {FREE(LocalizedString);LocalizedString=NULL;}
		}
		else
		{
			count= vsnprintf(s_buf,MAXPRINTF-1, fmt, ap );
		}
	#else
		count= vsnprintf(s_buf,MAXPRINTF-1, fmt, ap );
	#endif
	if (count == -1)
	{
		s_buf[MAXPRINTF-1]='\0';
	}
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
/* sciprint geared towards long strings (>MAXPRINTF) */
/* the long string is splitted in elements of length equal to the number of columns  */
/* from lines()                                                                      */
#if defined(__STDC__) || defined(_MSC_VER)
  void sciprint_full(char *fmt,...) 
#else 
  void sciprint_full(va_alist) va_dcl
#endif 
{
	integer lstr;
	va_list ap;
  char *s_buf=NULL;
  char *split_s_buf=NULL;
  int count=0;
  int p_s=0;
  static integer colwidth;

	s_buf=MALLOC(sizeof(char)*(MAXCHARSSCIPRINT_FULL+1));
  if (s_buf == (char *) 0)
  {
     sciprint("sciprint_full: No more memory\r\n");
     return;
  }

  /* number of columns as set by command lines() */
  colwidth = C2F(iop).lct[4];  /* lct[4] in C is lct(5) from fortran */
  /* clamp to a minimum: value is arbitrary */
  if (colwidth < 20) {colwidth=20;}
  /* clamp to a maximum: value is selected so that each line fits in a single console line */
  /* this is needed because computation of the lines() value in ON_WND_TEXT_WM_SIZE is not */
  /* consistent with the limit before a carriage return occurs in TextPutStr - this latter */
  /* limit uses lptw->ScreenSize, which is set to x=120,y=80 at init and apparently never  */
  /* changed on window resizing                                                            */
  if (colwidth > 109) {colwidth=109;}

  split_s_buf=MALLOC(sizeof(char)*(colwidth+1));
  if (split_s_buf == (char *) 0)
  {
     sciprint("sciprint_full: No more memory\r\n");
     return;
  }

#if defined(__STDC__) || defined(_MSC_VER)
	va_start(ap,fmt);
#else
	char *fmt;
	va_start(ap);
	fmt = va_arg(ap, char *);
#endif

#if defined(linux) || defined(_MSC_VER)
	count = vsnprintf (s_buf,MAXCHARSSCIPRINT_FULL-1, fmt, ap );
	if (count == -1)
	{
		s_buf[MAXCHARSSCIPRINT_FULL-1]='\0';
	}
#else
	(void )vsprintf(s_buf, fmt, ap );
#endif

  va_end(ap);

  lstr=strlen(s_buf);

  if (lstr<colwidth)
  {
     sciprint(s_buf);
  } 
  else 
  {
     strncpy(split_s_buf,s_buf+p_s,colwidth-1);
     split_s_buf[colwidth]='\0';
     p_s=p_s+colwidth-1;
     sciprint(split_s_buf);
     sciprint("\n");
     while (p_s+colwidth-1<(int)lstr) 
     {
        strncpy(split_s_buf,s_buf+p_s,colwidth-1);
        split_s_buf[colwidth]='\0';
        p_s=p_s+colwidth-1;
        sciprint("  (cont'd) %s",split_s_buf);
        sciprint("\n");
     }
     strncpy(split_s_buf,s_buf+p_s,lstr-p_s);
     split_s_buf[lstr-p_s]='\0';
     sciprint("     (end) %s",split_s_buf);
  }

  if (s_buf){FREE(s_buf);s_buf=NULL;}
  if (split_s_buf){FREE(split_s_buf);split_s_buf=NULL;}

}
/*-----------------------------------------------------------------------------------*/ 
/* sciprint_l is used especially in Scierror, for it would not search the hashtable  */
/* in fact, it is the former version of sciprint									 */
/* changed by HUANG Xu									 */
#if defined(__STDC__) || defined(_MSC_VER)
  void  sciprint_l(char *fmt,...) 
#else 
  void sciprint_l(va_alist) va_dcl
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