/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) DIGITEO - 2009
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include "csignal.h"
#include "addinter.h" /* for DynInterfStart */
#include "Os_specific.h" /* for DynInterfStart */
#include "callinterf.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static  jmp_buf jmp_env;
/*--------------------------------------------------------------------------*/
static void sci_sigint_addinter(int n);
/*--------------------------------------------------------------------------*/
/**
 ** Static function table
 ** Watch out the positions are crutial !!!
 ** @TODO : Make this less crappy...
 **/
#define INTERFACES_MAX 64
static OpTab Interfaces[INTERFACES_MAX] = {
    /* 01  */ {gw_user}, /* free position may be used */
	/* 02  */ {gw_linear_algebra},
	/* 03  */ {gw_user}, /* free position may be used */
	/* 04  */ {gw_user}, /* free position may be used */
	/* 05  */ {gw_io},
	/* 06  */ {gw_elementary_functions},
	/* 07  */ {gw_graphics},
	/* 08  */ {gw_cacsd0},
	/* 09  */ {gw_cacsd1},
	/* 10  */ {gw_cacsd2},
	/* 11  */ {gw_dynamic_optimization},
	/* 12  */ {gw_differential_equations1},
	/* 13  */ {gw_core},
	/* 14  */ {gw_user}, /* RESERVED (see callinter.h) */
	/* 15  */ {gw_dynamic_metanet},
	/* 16  */ {gw_polynomials},
	/* 17  */ {gw_data_structures1},
	/* 18  */ {gw_dynamic_signal_processing},
	/* 19  */ {gw_dynamic_interpolation},
	/* 20  */ {gw_cacsd3},
	/* 21  */ {gw_string},
	/* 22  */ {gw_dynamic_symbolic},
	/* 23  */ {gw_boolean},
	/* 24  */ {gw_user2}, /* RESERVED (see callinter.h) */
	/* 25  */ {gw_gui},
	/* 26  */ {gw_differential_equations2},
	/* 27  */ {gw_sparse},
	/* 28  */ {gw_slicot},
	/* 29  */ {gw_differential_equations3},
	/* 30  */ {gw_differential_equations4},
	/* 31  */ {gw_dynamic_functions},
	/* 32  */ {gw_differential_equations6},
	/* 33  */ {gw_output_stream},
	/* 34  */ {gw_fileio},
	/* 46  */ {gw_dynamic_arnoldi},
	/* 36  */ {gw_special_functions1},
	/* 37  */ {gw_dynamic_statistics},
	/* 38  */ {gw_dynamic_randlib},
	/* 39  */ {gw_special_functions2},
	/* 40  */ {gw_tclsci},
	/* 41  */ {gw_data_structures2},
	/* 42  */ {gw_pvm},
	/* 43  */ {gw_integer},
	/* 44  */ {gw_linear_algebra2},
	/* 45  */ {gw_dynamic_scicos},
	/* 46  */ {gw_dynamic_link},
	/* 47  */ {gw_windows_tools},
	/* 48  */ {gw_time},
	/* 49  */ {gw_dynamic_sound},
	/* 50  */ {gw_localization},
	/* 51  */ {gw_fftw},
	/* 52  */ {gw_jvm},
	/* 53  */ {gw_shell},
	/* 54  */ {gw_completion},
	/* 55  */ {gw_history_manager},
	/* 56  */ {gw_dynamic_spreadsheet},
	/* 57  */ {gw_graphic_export},
	/* 58  */ {gw_dynamic_umfpack},
	/* 59  */ {gw_dynamic_matio},
	/* 60  */ {gw_dynamic_helptools},
	/* 61  */ {gw_call_scilab},
	/* 62  */ {gw_dynamic_hdf5},
	/* 63  */ {gw_dynamic_xpad},
	/* 64  */ {gw_dynamic_xcos}
};
/*--------------------------------------------------------------------------*/
static int sig_ok = 0;
/*--------------------------------------------------------------------------*/
/**
 * call the apropriate interface according to the value of k
 * iflagint is no more used here ....
 * @param k the number of the interface
 * @return 0
 */
int C2F(callinterf) (int *k)
{
	int returned_from_longjump ;
	static int count = 0;

	if ( count == 0)
    {
		if (sig_ok) 
		{
			if (signal(SIGINT,sci_sigint_addinter) == SIG_ERR) 
			{
				fprintf(stderr,"Could not set the signal SIGINT to the handler.\n");
			}
		}
		if (( returned_from_longjump = setjmp(jmp_env)) != 0 )
		{
			if (sig_ok) 
			{
				if (signal(SIGINT, controlC_handler) == SIG_ERR) 
				{
					fprintf(stderr,"Could not set the signal SIGINT to the handler.\n");
				}
			}
			Scierror(999,_("SIGSTP: aborting current computation\n"));
			count = 0;
			return 0;
		}
	}
	count++;
	if (*k > DynInterfStart) 
	{
		C2F(userlk)(k);
	} 
	else
	{
		if ( (*k > INTERFACES_MAX) || (*k < 1) )
		{
			Scierror(999,_("Error: Not a valid gateway ID %d.\n"), *k);
			count = 0;
			return 0;
		}
		else 
		{
			(*(Interfaces[*k-1].fonc))();
		}
	}
	count--;
	if (count == 0) 
	{
		if (sig_ok) 
		{
			if (signal(SIGINT, controlC_handler) == SIG_ERR) 
			{
				fprintf(stderr,"Could not set the signal SIGINT to the handler.\n");
			}
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static void sci_sigint_addinter(int n)
{
	int c = 0;
	sciprint(_("Trying to stop scilab in the middle of an interface.\n"));
	sciprint(_("Do you really want to abort computation (y or n ?) \n"));
	c = getchar();
	if ( c == 'y' ) errjump(n);
}
/*--------------------------------------------------------------------------*/
/**
* long jump to stop interface computation
*/
void  errjump(int n)
{
	longjmp(jmp_env,-1);
}
/*--------------------------------------------------------------------------*/
