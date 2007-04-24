/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "realmain.h"
#include "MALLOC.h"
#include "sciprint.h"
#ifndef _MSC_VER
#include "xscion.h"
#include "getarg.h"
#endif
#include "inffic.h" /* get_sci_data_strings */
#include "scirun.h"
#include "sciquit.h"
#include "getarg.h"
#include "tmpdir.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
extern void sci_clear_and_exit(int n);
extern int C2F(inisci)(int *,int *,int *);
extern void sci_usr1_signal(int n);
#ifdef _MSC_VER
#include "ExceptionMessage.h"
#else
extern int IsNoInteractiveWindow(void);
extern void InitXsession(void);
extern void main_sci (int argc,char ** argv, char *startup, int lstartup,int memory);

#endif
/*-----------------------------------------------------------------------------------*/
static void strip_blank(char *source);
/*-----------------------------------------------------------------------------------*/
static int  no_startup_flag=0;
/*-----------------------------------------------------------------------------------*/
#define BSIZE 128
#ifndef PATH_MAX
#define PATH_MAX 1024
#endif
/*-----------------------------------------------------------------------------------*/
void realmain(int no_startup_flag_l,char *initial_script,int initial_script_type,int memory)
{
  static int ini=-1;
  int ierr=0;
  char *startup=(char*)MALLOC(sizeof(char)*PATH_MAX+1);
  Set_no_startup_flag(no_startup_flag_l);

  /* create temp directory */
  C2F(settmpdir)();

  /* signals */
  #ifdef ENABLESIG
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
  #endif

  /*  prepare startup script  */

  if ( no_startup_flag_l == 0) 
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
      main_sci(argc,argv,startup,strlen(startup),memory);
    }
  else 
    {
    	if (! IsNoInteractiveWindow() )
    	{
    		InitXsession();
    	}
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
			ExceptionMessage(GetExceptionCode(),NULL);
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
  FREE(startup);
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
#ifndef _MSC_VER
char ** create_argv(int *argc)
{
  int i;
  char **argv;
  *argc = C2F(sciiargc)() + 1;
  if ( ( argv = malloc((*argc)*sizeof(char *))) == NULL) return NULL;
  for ( i=0 ; i < *argc ; i++) 
    {
      char buf[BSIZE];
      C2F(scigetarg)(&i,buf,BSIZE);
      buf[BSIZE-1]='\0';
      strip_blank(buf);
      argv[i] = malloc((strlen(buf)+1)*sizeof(char));
      if ( argv[i] == NULL) return NULL;
      strcpy(argv[i],buf);
    }
  return argv;
}
#endif
/*-----------------------------------------------------------------------------------*/
static void strip_blank(char *source)
{
  char *p;
  p = source;
  /* look for end of string */
  while(*p != '\0') p++;
  while(p != source)
  {
    p--;
    if(*p != ' ') break;
    *p = '\0';
  }
}
/*-----------------------------------------------------------------------------------*/

