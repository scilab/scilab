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
#include <string.h>
#if defined(__linux__)
#define __USE_FORTIFY_LEVEL 0 /* Avoid dependency on GLIBC_2.11 (__longjmp_chk) */
#endif
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include "csignal.h"
#include "addinter.h" /* for DynInterfStart */
#include "callinterf.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
jmp_buf jmp_env;
/*--------------------------------------------------------------------------*/
/**
 ** Static function table
 ** Watch out the positions are crutial !!!
 ** @TODO : Make this less crappy...
 **/
#define INTERFACES_MAX 72
static OpTab Interfaces[INTERFACES_MAX] =
{
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
    /* 15  */ {gw_user}, /* free position may be used */
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
    /* 35  */ {gw_dynamic_arnoldi},
    /* 36  */ {gw_user}, /* free position may be used */
    /* 37  */ {gw_dynamic_statistics},
    /* 38  */ {gw_dynamic_randlib},
    /* 39  */ {gw_dynamic_special_functions},
    /* 40  */ {gw_dynamic_tclsci},
    /* 41  */ {gw_data_structures2},
    /* 42  */ {gw_dynamic_history_browser},
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
    /* 53  */ {gw_console},
    /* 54  */ {gw_completion},
    /* 55  */ {gw_history_manager},
    /* 56  */ {gw_dynamic_spreadsheet},
    /* 57  */ {gw_dynamic_graphic_export},
    /* 58  */ {gw_dynamic_umfpack},
    /* 59  */ {gw_dynamic_matio},
    /* 60  */ {gw_dynamic_helptools},
    /* 61  */ {gw_call_scilab},
    /* 62  */ {gw_hdf5},
    /* 63  */ {gw_dynamic_scinotes},
    /* 64  */ {gw_dynamic_xcos},
    /* 65  */ {gw_dynamic_action_binding},
    /* 66  */ {gw_dynamic_parallel},
    /* 67  */ {gw_dynamic_ui_data},
    /* 68  */ {gw_dynamic_xml},
    /* 69  */ {gw_dynamic_preferences},
    /* 70  */ {gw_dynamic_graphic_objects},
    /* 71  */ {gw_dynamic_external_objects},
    /* 72  */ {gw_dynamic_external_objects_java}

};
/*--------------------------------------------------------------------------*/
/**
 * call the apropriate interface according to the value of k
 * @param k the number of the interface
 * @return 0
 */
int C2F(callinterf) (int *k)
{
    static int count = 0;

    if ( count == 0)
    {
        if ( setjmp(jmp_env) != 0 )
        {
            Scierror(999, _("Aborting current computation\n"));
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
            Scierror(999, _("Error: Not a valid gateway ID %d. Should be between %d and %d.\n"), *k, 1, INTERFACES_MAX);
            count = 0;
            return 0;
        }
        else
        {
            (*(Interfaces[*k - 1].fonc))();
        }
    }
    count--;

    return 0;
}
/*--------------------------------------------------------------------------*/
/**
* long jump to stop interface computation
*/
void errjump()
{
    longjmp(jmp_env, -1);
}
/*--------------------------------------------------------------------------*/
