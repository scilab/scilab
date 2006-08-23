/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef sun 
	#ifndef SYSV
	#include <sys/ieeefp.h>
	#endif
#endif


#ifdef _MSC_VER
#include "wtext.h"
#endif

#include "MALLOC.h"
#include "sciquit.h"
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(deletewin)(integer *number);
extern char *get_sci_data_strings(int n);
extern int Get_no_startup_flag(void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void C2F (tmpdirc) (void);
extern int IsFromC(void);
extern int IsFromJava(void);
extern void AllGraphWinDelete(void);
extern void C2F(freegmem)(void);
extern void C2F(freemem)(void);

#ifdef _MSC_VER
extern LPTW GetTextWinScilab(void);
extern BOOL IsWindowInterface(void);
extern void RestoreConsoleColors(void);
extern int TerminateJVMs(void);
#endif

#ifdef WITH_TK
extern int CloseTCLsci(void);
#endif
/*-----------------------------------------------------------------------------------*/ 
static int CloseConsoleGUI(void);
static int CloseConsoleGUI(void);
static int CloseGraphicsWindows(void);

int ExitScilab(void);
int C2F(sciquit)(void);
int sci_exit(int n);
/*-----------------------------------------------------------------------------------*/ 
static int CloseConsoleGUI(void)
{
	#if _MSC_VER
	LPTW lptw=GetTextWinScilab();
	TextClose (lptw);
	TextMessage ();		/* process messages */
	#else

	#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int ExitScilab(void)
{
	int bOK=FALSE;
	int i=0;
		
    if ( Get_no_startup_flag() == 0) 
    {
      char *quit_script =  get_sci_data_strings(5);
      C2F(scirun)(quit_script,strlen(quit_script));
    }

	AllGraphWinDelete();

	#ifdef _MSC_VER
		if ( IsWindowInterface() ) 
		{
			CloseConsoleGUI();
		}
		else
		{
			if ( !IsFromC() && !IsFromJava() )  RestoreConsoleColors();
		}
	#else
		C2F (xscion) (&i);
		if (i != 0)	CloseConsoleGUI();
	#endif

	#ifdef _MSC_VER /* Bug sous Linux lors de la liberation memoire */
		C2F(freegmem)();
		C2F(freemem)();
	#endif

	#ifdef WITH_TK
		CloseTCLsci();
	#endif

	#ifdef _MSC_VER
		TerminateJVMs();
	#endif

	/** clean tmpfiles **/
	C2F(tmpdirc)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int C2F(sciquit)(void)
{
	int status = 0;
	ExitScilab();
	sci_exit(status) ;
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int sci_exit(int n) 
{
  /* really exit */
  #ifdef sun 
		#ifndef SYSV
  	{
    	char *mode, **out, *in;
    	ieee_flags("clearall","exeption","all", &out);
  	}
		#endif 
	#endif 

  exit(n);
  return(0);
}
/*-----------------------------------------------------------------------------------*/
