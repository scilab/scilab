/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "realmain.h"
/*-----------------------------------------------------------------------------------*/
extern void C2F (settmpdir)(void);
extern void sci_clear_and_exit(int n);
extern int C2F (sciquit)(void);
extern int C2F(inisci)(int *,int *,int *);
extern int C2F(scirun)(char *startup,int lstartup);
extern void sci_usr1_signal(int n);
extern char *get_sci_data_strings(int n);
extern int sci_exit(int n);
/*-----------------------------------------------------------------------------------*/
static int  no_startup_flag=0;
/*-----------------------------------------------------------------------------------*/
void realmain(int nowin,int no_startup_flag,char *initial_script,int initial_script_type,int memory)
{
  static int ini=-1;
  int ierr=0;
  char startup[256];

  Set_no_startup_flag(no_startup_flag);

  /* create temp directory */
  C2F(settmpdir)();
  /* signals */
  signal(SIGINT,sci_clear_and_exit);
  #ifdef SIGBUS
  signal(SIGBUS,sci_clear_and_exit);
  #endif
  signal(SIGSEGV,sci_clear_and_exit);
  #if SIGQUIT
  signal(SIGQUIT,sci_clear_and_exit);
  #endif
  #ifdef SIGHUP
  signal(SIGHUP,sci_clear_and_exit);
  #endif
  #ifdef SIGUSR1
  signal(SIGUSR1,sci_usr1_signal);
  #endif

  #ifdef _MSC_VER
  signal(SIGILL,sci_clear_and_exit);
  signal(SIGFPE,sci_clear_and_exit);
  signal(SIGTERM,sci_clear_and_exit);
  signal(SIGBREAK,sci_clear_and_exit);
  signal(SIGABRT,sci_clear_and_exit);
  #endif

  /*  prepare startup script  */

  if ( no_startup_flag == 0) 
  {
	/* execute a startup */
    if ( initial_script != NULL ) switch ( initial_script_type ) 
	{
		case 0 : 
			sprintf(startup,"%s;exec('%s',-1)",get_sci_data_strings(1),initial_script);
		break;
		case 1 : 
			sprintf(startup,"%s;%s;",get_sci_data_strings(1),initial_script);
		break;
	}
    else sprintf(startup,"%s;",get_sci_data_strings(1));
  }
  else 
  {
	/* No startup but maybe an initial script  */
    if ( initial_script != NULL ) switch ( initial_script_type ) 
	{
	    case 0 : 
	      sprintf(startup,"exec('%s',-1)",initial_script); break;
	    case 1 : 
	      sprintf(startup,"%s;",initial_script);   break;
	}
    else sprintf(startup," ");
  }

  #ifndef _MSC_VER
  if ( nowin == 0 ) 
    {
      int argc=0;
      char **argv=NULL;

      argv = create_argv(&argc);

      /* we are in window mode */
      SetXsciOn();
      main_sci(argc,argv,startup,strlen(startup),memory);
    }
  else 
    {
      /* initialize scilab interp  */
      C2F(inisci)(&ini, &memory, &ierr);
      if (ierr > 0) sci_exit(1) ;
      /* execute the initial script and enter scilab */ 
      C2F(scirun)(startup,strlen(startup));
    }
  #else
	/* initialize scilab interp  */
	C2F(inisci)(&ini, &memory, &ierr);
	if (ierr > 0) sci_exit(1) ;

	/* execute the initial script and enter scilab */ 
	#ifndef _DEBUG
	_try
	{
		C2F(scirun)(startup,strlen(startup));
	}
	_except (EXCEPTION_EXECUTE_HANDLER) 
	{
		Rerun:
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s).\nScilab may become unstable.\n",ExceptionString);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
		_try
		{
			C2F(scirun)("",strlen(""));
		}
		_except (EXCEPTION_EXECUTE_HANDLER) 
		{
			goto Rerun;
		}

	}
	#else
		C2F(scirun)(startup,strlen(startup));
	#endif
  #endif

  /* cleaning */
  C2F(sciquit)();
}
/*-----------------------------------------------------------------------------------*/
void Set_no_startup_flag(int start)
{
	no_startup_flag=start;
}
/*-----------------------------------------------------------------------------------*/
int Get_no_startup_flag(void)
{
	return no_startup_flag;
}
/*-----------------------------------------------------------------------------------*/

