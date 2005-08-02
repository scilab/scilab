/**************************************
 * Copyright Jean-Philippe Chancelier 
 * ENPC 
 **************************************/
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#include "../machine.h"
#include "../os_specific/addinter.h" /* for DynInterfStart */
#include "../os_specific/Os_specific.h" /* for DynInterfStart */

static  jmp_buf jmp_env; 

extern int  C2F(error) __PARAMS((int *));
extern void sciprint __PARAMS((char* ,...));
extern int  Scierror __PARAMS((int iv,char *fmt,...));

extern void  errjump(int n);
extern void  sci_sig_tstp(int n);
extern void  controlC_handler(int n);
static void  sci_sigint_addinter(int n);

/***********************************************************
 * interface function 
 ***********************************************************/

static int c_local = 9999;

void C2F(NoTclsci)(void)
{
  sciprint("TclSci interface not loaded \n");
  C2F(error)(&c_local);
  return;
}

void C2F(NoPvm)(void)
{
  sciprint("pvm interface not loaded \n");
  C2F(error)(&c_local);
  return;
}



/** table of interfaces **/

typedef  struct  {
  void  (*fonc)();} OpTab ;

#include "callinterf.h"


/***********************************************************
 * call the apropriate interface according to the value of k 
 * iflagint is no more used here ....
 ***********************************************************/


static int sig_ok = 0;

int C2F(callinterf) ( int *k, int * iflagint)
{
  int returned_from_longjump ;
  static int count = 0;
  if ( count == 0) 
    {
      if (sig_ok) signal(SIGINT,sci_sigint_addinter);
      if (( returned_from_longjump = setjmp(jmp_env)) != 0 )
	{
	  if (sig_ok) signal(SIGINT, controlC_handler);
	  Scierror(999,"SIGSTP: aborting current computation\r\n");
	  count = 0;
	  return 0;
	}
    }
  count++;
  if (*k > DynInterfStart) 
    C2F(userlk)(k);
  else
    (*(Interfaces[*k-1].fonc))();
  count--;
  if (count == 0) { 
    if (sig_ok) signal(SIGINT, controlC_handler);
  }
  return 0;
}

static void sci_sigint_addinter(int n)
{
  int c;
  sciprint("Trying to stop scilab in the middle of an interface\n");
  sciprint("Do you really want to abort computation (y n  ?) ");
  c = getchar();
  if ( c == 'y' ) errjump(n);
}


/***********************************************************
 * Unused function just here to force linker to load some 
 * functions 
 ***********************************************************/

extern int   Blas_contents __PARAMS((int));
extern int   Lapack_contents __PARAMS((int));
extern int   Calelm_contents __PARAMS((int));
extern int   Sun_contents __PARAMS((int));
extern int   System2_contents __PARAMS((int));
extern int   System_contents __PARAMS((int));
extern int  Intersci_contents __PARAMS((int));
extern int  Sparse_contents __PARAMS((int));

#ifndef WIN32
int ForceLink(void)
{
  Blas_contents(0);
  Lapack_contents(0);
  Calelm_contents(0);
  Sun_contents(0);
  System2_contents(0);
  System_contents(0);
  Intersci_contents(0);
  Sparse_contents(0);
  return 0;
}
#endif

/*-------------------------------------
 * long jump to stop interface computation 
 *-------------------------------------*/

void errjump(int n)
{
  longjmp(jmp_env,-1); 
}
