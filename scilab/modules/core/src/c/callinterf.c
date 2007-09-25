/**************************************
 * Copyright Jean-Philippe Chancelier 
 * ENPC 
 **************************************/
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>


#include "sciprint.h"
#include "csignal.h"

#include "addinter.h" /* for DynInterfStart */
#include "Os_specific.h" /* for DynInterfStart */

#include "error_scilab.h"
#include "message_scilab.h"
#include "callinterf.h"

#include "machine.h"

static  jmp_buf jmp_env; 

extern int C2F(error) __PARAMS((int *));

static void sci_sigint_addinter(int n);

/***********************************************************
 * interface function 
 ***********************************************************/

static int c_local_interf = 9999;

int C2F(Nogw_slicot)(void)
{
	message_scilab("%s interface not loaded.","slicot");
	C2F(error)(&c_local_interf);
	return 0;
}

static int sig_ok = 0;


/**
 * call the apropriate interface according to the value of k 
 * iflagint is no more used here ....
 * @param k the number of the interface
 * @param iflagint obsolete (no longer used)
 * @return 
 */
int C2F(callinterf) (int *k)
{
  int returned_from_longjump ;
  static int count = 0;
  if ( count == 0) 
    {
      if (sig_ok) signal(SIGINT,sci_sigint_addinter);
      if (( returned_from_longjump = setjmp(jmp_env)) != 0 )
	{
	  if (sig_ok) signal(SIGINT, controlC_handler);
	  error_scilab(999,"SIGSTP: aborting current computation");
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
  message_scilab("Trying to stop scilab in the middle of an interface.");
  message_scilab("Do you really want to abort computation (y or n ?) ");
  c = getchar();
  if ( c == 'y' ) errjump(n);
}



/*-------------------------------------
 * long jump to stop interface computation 
 *-------------------------------------*/

void errjump(int n)
{
  longjmp(jmp_env,-1); 
}
