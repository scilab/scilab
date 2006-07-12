
/* STARTUP PROCEDURE FOR UNIX FORTRAN PROGRAMS */

#include <Windows.h>
#include "stdio.h"
#include "signal.h"

#include "../Messages.h"
#include "../Warnings.h"
#include "../Errors.h"
#include "../../os_specific/win_mem_alloc.h" /* MALLOC */

extern int Console_Main(int argc, char **argv);

#ifndef SIGIOT
#ifdef SIGABRT
#define SIGIOT SIGABRT
#endif
#endif

#ifndef KR_headers
#undef VOID
#include "stdlib.h"
#endif

#ifndef VOID
#define VOID void
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO__STDC
#define ONEXIT onexit
  extern VOID f_exit();
#else
#ifndef KR_headers
  extern void f_exit(void);
#ifndef NO_ONEXIT
#define ONEXIT atexit
  extern int atexit(void (*)(void));
#endif
#else
#ifndef NO_ONEXIT
#define ONEXIT onexit
  extern VOID f_exit();
#endif
#endif
#endif

#ifdef KR_headers
  extern VOID f_init(), sig_die();
  extern int MAIN__();
#define Int /* int */
#else
  extern void f_init(void), sig_die(char*, int);
  extern int MAIN__(void);
#define Int int
#endif

  static VOID sigfdie(Int n)
  {
    sig_die(MSG_ERROR1, 1);
  }


  static VOID sigidie(Int n)
  {
    sig_die(MSG_ERROR2, 1);
  }

#ifdef SIGQUIT
  static VOID sigqdie(Int n)
  {
    sig_die(MSG_ERROR3, 1);
  }
#endif


  static VOID sigindie(Int n)
  {
    sig_die(MSG_ERROR4, 0);
  }

  static VOID sigtdie(Int n)
  {
    sig_die(MSG_ERROR5, 0);
  }

#ifdef SIGTRAP
  static VOID sigtrdie(Int n)
  {
    sig_die(MSG_ERROR6, 1);
  }
#endif


#ifdef KR_headers
  VOID send_xarg(xxargc,xxargv);
#else
  void send_xarg(int xxargc, char **xxargv);
#endif

int main (int argc, char **argv)
{
    /** JPC : 1998 pour fair un dll **/
	#define MAXCMDTOKENS 128
	int argcbis=-1;
	LPSTR argvbis[MAXCMDTOKENS];
	int i=0;
	int FindNW=0;

	for (i=0;i<argc;i++)
	{
		if ( (strcmp(argv[i],"-nw")==0) || (strcmp(argv[i],"-NW")==0) ) FindNW=1;
		if ( (strcmp(argv[i],"-nwni")==0) || (strcmp(argv[i],"-NWNI")==0) ) FindNW=1;
		if ( (strcmp(argv[i],"-nogui")==0) || (strcmp(argv[i],"-NOGUI")==0) ) FindNW=1;
	}

	if (FindNW==0)
	{
		char *nwparam=NULL;
		nwparam=(char*)MALLOC((strlen("-nw")+1)*sizeof(char));
		strcpy(nwparam,"-nw");
		for (i=0;i<argc;i++)
		{
			argvbis[i]=argv[i];
		}
		argvbis[argc]=nwparam;
		argcbis=argc+1;
		send_xarg(argcbis,argvbis);

	}
	else
	{
		for (i=0;i<argc;i++)
		{
			argvbis[i]=argv[i];
		}
		argcbis=argc;
	
	}
    	send_xarg(argcbis,argvbis);



    signal(SIGFPE, sigfdie);	/* ignore underflow, enable overflow */
#ifdef SIGIOT
    signal(SIGIOT, sigidie);
#endif
#ifdef SIGTRAP
    signal(SIGTRAP, sigtrdie);
#endif
#ifdef SIGQUIT
    if(signal(SIGQUIT,sigqdie) == SIG_IGN)
      signal(SIGQUIT, SIG_IGN);
#endif
    if(signal(SIGINT, sigindie) == SIG_IGN)
      signal(SIGINT, SIG_IGN);
    signal(SIGTERM,sigtdie);

#ifdef pdp11
    ldfps(01200); /* detect overflow as an exception */
#endif

    f_init();
#ifndef NO_ONEXIT
    ONEXIT(f_exit);
#endif
    Console_Main(argcbis,argvbis);
#ifdef NO_ONEXIT
    f_exit();
#endif
    exit(0);	/* exit(0) rather than return(0) to bypass Cray bug */
    return 0;	/* For compilers that complain of missing return values; */
		/* others will complain that this is unreachable code. */
  }
#ifdef __cplusplus
}
#endif



