/**************************************
 * Copyright Jean-Philippe Chancelier
 * ENPC
 **************************************/
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include "csignal.h"
#include "addinter.h" /* for DynInterfStart */
#include "Os_specific.h" /* for DynInterfStart */
#include "callinterf.h"
#include "machine.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_dynamic_scicos.h"

static  jmp_buf jmp_env;

static void sci_sigint_addinter(int n);

/**
 ** Static function table
 ** Watch out the positions are crutial !!!
 ** @TODO : Make this less crappy...
 **/
static OpTab Interfaces[] = {
    /* 01  */ {C2F(gw_user)}, /* free position may be used */
	/* 02  */ {C2F(gw_linear_algebra)},
	/* 03  */ {C2F(gw_user)}, /* free position may be used */
	/* 04  */ {C2F(gw_user)}, /* free position may be used */
	/* 05  */ {C2F(gw_io)},
	/* 06  */ {C2F(gw_elementary_functions)},
	/* 07  */ {C2F(gw_graphics)},
	/* 08  */ {C2F(gw_cacsd0)},
	/* 09  */ {C2F(gw_cacsd1)},
	/* 10  */ {C2F(gw_cacsd2)},
	/* 11  */ {C2F(gw_optim)},
	/* 12  */ {C2F(gw_differential_equations1)},
	/* 13  */ {C2F(gw_core)},
	/* 14  */ {C2F(gw_user)},
	/* 15  */ {C2F(gw_metanet)},
	/* 16  */ {C2F(gw_polynomials)},
	/* 17  */ {C2F(gwdatastructures1)},
	/* 18  */ {C2F(gw_signal)},
	/* 19  */ {C2F(gw_interpolation)},
	/* 20  */ {C2F(gw_cacsd3)},
	/* 21  */ {C2F(gw_string)},
	/* 22  */ {C2F(gw_symbolic)},
	/* 23  */ {C2F(gw_boolean)},
	/* 24  */ {C2F(gw_user2)},
	/* 25  */ {C2F(gw_gui)},
	/* 26  */ {C2F(gw_differential_equations2)},
	/* 27  */ {C2F(gw_sparse)},
	/* 28  */ {C2F(gw_slicot)},
	/* 29  */ {C2F(gw_differential_equations3)},
	/* 30  */ {C2F(gw_differential_equations4)},
	/* 31  */ {C2F(gw_user2)},/* free position may be used */
	/* 32  */ {C2F(gw_differential_equations6)},
	/* 33  */ {C2F(gw_user2)}, /* free position may be used */
	/* 34  */ {C2F(gw_fileio)},
	/* 46  */ {C2F(gw_arnoldi)},
	/* 36  */ {C2F(gw_special_functions1)},
	/* 37  */ {C2F(gw_statistics)},
	/* 38  */ {C2F(gw_randlib)},
	/* 39  */ {C2F(gw_special_functions2)},
	/* 40  */ {C2F(gw_tclsci)},
	/* 41  */ {C2F(gwdatastructures2)},
	/* 42  */ {C2F(gw_pvm)},
	/* 43  */ {C2F(gw_integer)},
	/* 44  */ {C2F(gw_linear_algebra2)},
	/* 45  */ {C2F(gw_dynamic_scicos)},
	/* 46  */ {C2F(gw_dynamic_link)},
	/* 47  */ {C2F(gw_windows_tools)},
	/* 48  */ {C2F(gw_time)},
	/* 49  */ {C2F(gw_sound)},
	/* 50  */ {C2F(gw_localization)},
	/* 51  */ {C2F(gw_fftw)},
	/* 52  */ {C2F(gw_jvm)},
	/* 53  */ {C2F(gw_shell)},
	/* 54  */ {C2F(gw_completion)},
	/* 55  */ {C2F(gw_history_manager)},
	/* 56  */ {C2F(gw_spreadsheet)},
	/* 57  */ {C2F(gw_graphic_export)}

};



/***********************************************************
 * interface function
 ***********************************************************/

/**
 ** Bruno : Unused, but may be used one day ?? Just commented
 ** Just for you Sly... God bless my kindness
 **/

/** { */

/* static int c_local_interf = 9999; */

/* int C2F(Nogw_slicot)(void) */
/* { */
/* 	sciprint("%s interface not loaded.\n","slicot"); */
/* 	C2F(error)(&c_local_interf); */
/* 	return 0; */
/* } */

/** } */

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
      if (sig_ok) {
		  signal(SIGINT,sci_sigint_addinter);
	  }
      if (( returned_from_longjump = setjmp(jmp_env)) != 0 )
		  {
			  if (sig_ok) {
				  signal(SIGINT, controlC_handler);
			  }
			  Scierror(999,_("SIGSTP: aborting current computation\n"));
			  count = 0;
			  return 0;
		  }
    }
  count++;
  if (*k > DynInterfStart) {
    C2F(userlk)(k);
  } else {
    (*(Interfaces[*k-1].fonc))();
  }
  count--;
  if (count == 0) {
    if (sig_ok) {
		signal(SIGINT, controlC_handler);
	}
  }
  return 0;
}



static void sci_sigint_addinter(int n)
{
  int c;
  sciprint(_("Trying to stop scilab in the middle of an interface.\n"));
  sciprint(_("Do you really want to abort computation (y or n ?) \n"));
  c = getchar();
  if ( c == 'y' ) errjump(n);
}


/*-------------------------------------
 * long jump to stop interface computation
 *-------------------------------------*/
void  errjump(int n)
{
  longjmp(jmp_env,-1);
}
