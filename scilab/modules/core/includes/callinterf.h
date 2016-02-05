/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __CALLINTERF__
#define __CALLINTERF__

/********************************************************
 * Table of hard coded Scilab interface
 *  to add a new interface add a declaration and a new entry
 *  at the end of the Interfaces array
 *  The maximum number of interfaces is bound to DynInterfStart
 *  ( file /core/src/c/addinter.h )
 ********************************************************/

/* 01  gw_user(); FREE */	    #include "gw_user.h"
/* 02 */						#include "../../linear_algebra/includes/gw_linear_algebra.h"
/* 03  gw_user(); FREE */
/* 04  gw_user(); FREE */
/* 05 */						#include "../../io/includes/gw_io.h"
/* 06 */						#include "../../elementary_functions/includes/gw_elementary_functions.h"
/* 07 */						#include "../../graphics/includes/gw_graphics.h"
/* 08 *///						#include "../../cacsd/includes/gw_cacsd0.h"
/* 09 *///						#include "../../cacsd/includes/gw_cacsd1.h"
/* 10 *///						#include "../../cacsd/includes/gw_cacsd2.h"
/* 11 */						#include "dynamic_gateways.h" /* optimization */
/* 12 *///						#include "../../differential_equations/includes/gw_differential_equations1.h"
/* 13 */						#include "gw_core.h"
/* 14  gw_user(); RESERVED to gw_user (was callinter.h.f / REMOVED) */
/* 15  gw_user(); FREE */
/* 16 						#include "../../polynomials/includes/gw_polynomials.h"*/
/* 17 */						#include "../../data_structures/includes/gw_data_structures1.h"
/* 18 */						#include "dynamic_gateways.h" /* signal_processing */
/* 19 */						#include "dynamic_gateways.h" /* interpolation */
/* 20 *///						#include "../../cacsd/includes/gw_cacsd3.h"
/* 21 *///						#include "../../string/includes/gw_string.h"
/* 22 */						#include "dynamic_gateways.h" /* symbolic */
/* 23 */						#include "../../boolean/includes/gw_boolean.h"
/* 24  gw_user2(); RESERVED to gw_user (was callinter.h.f / REMOVED) */
#include "gw_user2.h"
/* 25 */						#include "../../gui/includes/gw_gui.h"
/* 26 *///						#include "../../differential_equations/includes/gw_differential_equations2.h"
/* 27 *///						#include "../../sparse/includes/gw_sparse.h"
/* 28 *///						#include "../../cacsd/includes/gw_slicot.h"
/* 29 *///						#include "../../differential_equations/includes/gw_differential_equations3.h"
/* 30 *///						#include "../../differential_equations/includes/gw_differential_equations4.h"
/* 31 */     					#include "dynamic_gateways.h"	/* functions */
/* 32 *///						#include "../../differential_equations/includes/gw_differential_equations6.h"
/* 33 */                        #include "../../output_stream/includes/gw_output_stream.h"
/* 34 */						#include "../../fileio/includes/gw_fileio.h"
/* 35 */						#include "dynamic_gateways.h" /* arnoldi */
/* 36 gw_user(); FREE */
/* 37 */						#include "dynamic_gateways.h" /* statistics */
/* 38 */						#include "dynamic_gateways.h" /* randlib */
/* 39 */						#include "dynamic_gateways.h" /* special_functions */
/* 40 */						#include "dynamic_tclsci.h"
/* 41 */						#include "../../data_structures/includes/gw_data_structures2.h"
/* 42 */						#include "dynamic_gateways.h" /* history_browser */
/* 43 */						#include "../../integer/includes/gw_integer.h"
/* 44 */						#include "../../linear_algebra/includes/gw_linear_algebra2.h"
/* 45 */						#include "dynamic_gateways.h" /* scicos */
/* 46 */						#include "../../dynamic_link/includes/gw_dynamic_link.h"
/* 47 */						#include "../../windows_tools/includes/gw_windows_tools.h"
/* 48 */						#include "../../time/includes/gw_time.h"
/* 49 */						#include "dynamic_gateways.h"  /* sound */
/* 50 */						#include "../../localization/includes/gw_localization.h"
/* 51 */						#include "../../fftw/includes/gw_fftw.h"
/* 52 */						#include "../../jvm/includes/gw_jvm.h"
/* 53 */						#include "../../console/includes/gw_console.h"
/* 54 */						//#include "../../completion/includes/gw_completion.h"
/* 55 */						#include "../../history_manager/includes/gw_history_manager.h"
/* 56 */						#include "dynamic_gateways.h" /* spreadsheet */
/* 57 */						#include "dynamic_gateways.h" /* graphic_export */
/* 58 */						#include "dynamic_gateways.h" /* UMFPACK */
/* 59 */						#include "dynamic_gateways.h" /* matio */
/* 60 */						#include "dynamic_gateways.h" /* helptools */
/* 61 */						#include "../../call_scilab/includes/gw_call_scilab.h"
/* 62 */						#include "../../hdf5/includes/gw_hdf5.h" /* hdf5 */
/* 63 */						#include "dynamic_gateways.h" /* scinotes */
/* 64 */						#include "dynamic_gateways.h" /* xcos */
/* 65 */						#include "dynamic_gateways.h" /* action_binding */
/* 66 */                        #include "dynamic_parallel.h" /* parallel */
/* 67 */                        #include "dynamic_gateways.h" /* ui_data */
/* 68 */                        #include "dynamic_gateways.h" /* xml */
/* 69 */                        #include "dynamic_gateways.h" /* preferences */
/* 70 */                        #include "dynamic_gateways.h" /* graphic_objects */
/* 71 */                        #include "dynamic_gateways.h" /* external_objects */
/* 72 */                        #include "dynamic_gateways.h" /* external_objects_java */
/* 73 */			            #include "dynamic_gateways.h" /* MPI */


/** table of interfaces **/

typedef struct
{
    int  (*fonc)(void);
} OpTab ;

/**
 * call the appropriate interface according to the value of k
 * @param k the number of the interface
 * @return
 */
int C2F(callinterf) (int *k);

/**
 * long jump to stop interface computation
 */
void errjump ();


#endif /*__CALLINTERF__*/
