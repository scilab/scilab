
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
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

#ifndef SCI_OTHER_GEN
#define SCI_OTHER_GEN

#include "dynlib_randlib.h"

/* header for the Mersenne Twister */
RANDLIB_IMPEXP unsigned long int randmt(void);
RANDLIB_IMPEXP int set_state_mt_simple(double s);
RANDLIB_IMPEXP int set_state_mt(double seed_array[]);
RANDLIB_IMPEXP void get_state_mt(double state[]);

/* header for kiss */
RANDLIB_IMPEXP unsigned long int kiss(void);
RANDLIB_IMPEXP int set_state_kiss(double g1, double g2, double g3, double g4);
RANDLIB_IMPEXP void get_state_kiss(double g[]);

/* header for clcg2 */
RANDLIB_IMPEXP unsigned long int clcg2(void);
RANDLIB_IMPEXP int set_state_clcg2(double g1, double g2);
RANDLIB_IMPEXP void get_state_clcg2(double g[]);

/* header for scilab urand */
RANDLIB_IMPEXP unsigned long int urandc(void);
RANDLIB_IMPEXP int set_state_urand(double g);
RANDLIB_IMPEXP void get_state_urand(double g[]);

#endif /** SCI_OTHER_GEN   **/




