/**************************************
 * Copyright Jean-Philippe Chancelier 
 * ENPC 
 **************************************/
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#include "machine.h"
#include "sciprint.h"
#include "csignal.h"

#include "addinter.h" /* for DynInterfStart */
#include "Os_specific.h" /* for DynInterfStart */

#include "error_scilab.h"
#include "message_scilab.h"

static  jmp_buf jmp_env; 

extern int  C2F(error) __PARAMS((int *));

void errjump(int n);
void C2F(no_gw_tclsci)(void);
void C2F(NoPvm)(void);
void C2F(Nogw_scicos)(void);
void C2F(Nogw_cscicos)(void);
void C2F(Nogw_slicot)(void);
int ForceLink(void);

static void  sci_sigint_addinter(int n);

/***********************************************************
 * interface function 
 ***********************************************************/

static int c_local_interf = 9999;

void C2F(no_gw_tclsci)(void)
{
  message_scilab("core_message_121","tclsci");
  C2F(error)(&c_local_interf);
  return;
}

void C2F(NoPvm)(void)
{
  message_scilab("core_message_121","pvm");
  C2F(error)(&c_local_interf);
  return;
}

void C2F(Nogw_scicos)(void)
{
	message_scilab("core_message_121","scicos");
	C2F(error)(&c_local_interf);
	return;
}

void C2F(Nogw_cscicos)(void)
{
	message_scilab("core_message_121","scicos");
	C2F(error)(&c_local_interf);
	return;
}


void C2F(Nogw_slicot)(void)
{
	message_scilab("core_message_121","slicot");
	C2F(error)(&c_local_interf);
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
	  error_scilab(999,"core_error_147");
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
  message_scilab("core_message_122");
  message_scilab("core_message_123");
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
extern int   Intersci_contents __PARAMS((int));
extern int   Sparse_contents __PARAMS((int));

#ifndef _MSC_VER
int ForceLink(void)
{
	/*  
		commented for test purpose by Sylvestre (no sure it is mandatory
Blas_contents(0);
  Lapack_contents(0);
  Calelm_contents(0);
  Sun_contents(0);
  System2_contents(0);
  System_contents(0);
  Intersci_contents(0);
  Sparse_contents(0);*/
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
