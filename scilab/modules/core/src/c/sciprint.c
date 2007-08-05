/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/* Francois VOGEL  sciprint_full function */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "sciprint.h"
#ifdef _MSC_VER
#include "../../gui/src/c/wsci/wgnuplib.h"
#else
#include "../../gui/src/c/xsci/xscisrn.h"
#endif

#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "message_scilab.h"
#include "../../localization/includes/QueryStringMessage.h"
#include "../../console/includes/ShellPrintf.h"
#include "scilabmode.h"
#include "xscion.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #define vsnprintf _vsnprintf
  TW textwin;
  TW * getTextWin( void )
  {
    return &textwin;
  }
#endif

/* Variable used in order to avoid multiple call on xscion which 
 * will always return the same thing 
 * -1 here is used to specific that it is not initialized
*/

/*-----------------------------------------------------------------------------------*/ 
extern int getdiary __PARAMS(());
extern void diary_nnl __PARAMS((char *str,int *n));
/*-----------------------------------------------------------------------------------*/ 
void sciprint(char *fmt,...) 
{
	static int xscionSingleton = -1;
	int i;
	integer lstr;
	va_list ap;
	char s_buf[MAXPRINTF];
	int count=0;

	va_start(ap,fmt);

#if defined(linux) || defined(_MSC_VER)
{
	count= vsnprintf(s_buf,MAXPRINTF-1, fmt, ap );
	if (count == -1)
	{
		s_buf[MAXPRINTF-1]='\0';
	}
}
#else
	(void )vsprintf(s_buf, fmt, ap );
#endif
	lstr=strlen(s_buf);
	if (xscionSingleton==-1) {
		/* We haven't called this function before ... Then we call it and 
		   store the result once for all because it won't change */
		C2F(xscion)(&xscionSingleton);
	}

	if (xscionSingleton == 0) 
	{
		printf("%s",s_buf); 
	}
	else 
	{
		#ifdef _MSC_VER
		 TextPutS (getTextWin(),s_buf);
		#else
		 C2F(xscisrn)(s_buf,&lstr,0L);
		#endif
		if (getScilabMode() != SCILAB_NWNI) ShellPrintf(s_buf);
	}
	if (getdiary()) diary_nnl(s_buf,&lstr);
	va_end(ap);
}
/*-----------------------------------------------------------------------------------*/ 
/* as sciprint but with an added first argument which is ignored (used in do_printf) */
int sciprint2 (int iv, char *fmt,...)
{
	va_list ap;
	int count = 0;
	char s_buf[MAXPRINTF];

	va_start (ap, fmt);
	count= vsnprintf(s_buf,MAXPRINTF-1, fmt, ap );
	va_end (ap);
	if (count == -1)
	{
		s_buf[MAXPRINTF-1]='\0';
	}

	sciprint(s_buf);
	return count;
}
/*-----------------------------------------------------------------------------------*/ 
/* sciprint geared towards long strings (>MAXPRINTF) */
/* the long string is splitted in elements of length equal to the number of columns  */
/* from lines()                                                                      */
void sciprint_full(char *fmt,...) 
{
  integer lstr;
  va_list ap;
  char *s_buf=NULL;
  char *split_s_buf=NULL;
  int count=0;
  int p_s=0;
  static integer colwidth;
  char *MSG;

  s_buf=MALLOC(sizeof(char)*(MAXCHARSSCIPRINT_FULL+1));
  if (s_buf == (char *) 0)
  {
     message_scilab("core_message_145");
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
     message_scilab("core_message_145");
     return;
  }

	va_start(ap,fmt);

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
     sciprint("%s",s_buf);
  } 
  else 
  {
     strncpy(split_s_buf,s_buf+p_s,colwidth-1);
     split_s_buf[colwidth]='\0';
     p_s=p_s+colwidth-1;
     sciprint("%s",split_s_buf);
     sciprint("\n");
     while (p_s+colwidth-1<(int)lstr) 
     {
        strncpy(split_s_buf,s_buf+p_s,colwidth-1);
        split_s_buf[colwidth]='\0';
        p_s=p_s+colwidth-1;
        MSG=QueryStringMessage("core_message_146");
        if (MSG)
        {
            sciprint(MSG,split_s_buf);
            sciprint("\n");
        }
        if (MSG) {FREE(MSG);MSG=NULL;}
     }
     strncpy(split_s_buf,s_buf+p_s,lstr-p_s);
     split_s_buf[lstr-p_s]='\0';
     MSG=QueryStringMessage("core_message_147");
     if (MSG)
     {
         sciprint(MSG,split_s_buf);
         sciprint("\n");
     }
     if (MSG) {FREE(MSG);MSG=NULL;}
  }

  if (s_buf){FREE(s_buf);s_buf=NULL;}
  if (split_s_buf){FREE(split_s_buf);split_s_buf=NULL;}

}
/*-----------------------------------------------------------------------------------*/ 
