
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#ifndef SCI_OTHER_GEN
#define SCI_OTHER_GEN

#include "dynlib_randlib.h"

/* header for the Mersenne Twister */
RANDLIB_IMPEXP unsigned long randmt(void);
RANDLIB_IMPEXP int set_state_mt_simple(double s);
RANDLIB_IMPEXP int set_state_mt(double seed_array[]);
RANDLIB_IMPEXP void get_state_mt(double state[]);

/* header for kiss */
RANDLIB_IMPEXP unsigned long kiss(void);
RANDLIB_IMPEXP int set_state_kiss(double g1, double g2, double g3, double g4);
RANDLIB_IMPEXP void get_state_kiss(double g[]);

/* header for clcg2 */
RANDLIB_IMPEXP unsigned long clcg2(void);
RANDLIB_IMPEXP int set_state_clcg2(double g1, double g2);
RANDLIB_IMPEXP void get_state_clcg2(double g[]);

/* header for scilab urand */
RANDLIB_IMPEXP unsigned long urandc(void);
RANDLIB_IMPEXP int set_state_urand(double g);
RANDLIB_IMPEXP void get_state_urand(double g[]);

/* header for scilab fsultra */
RANDLIB_IMPEXP unsigned long fsultra(void);
RANDLIB_IMPEXP int set_state_fsultra(double g[]);
RANDLIB_IMPEXP int set_state_fsultra_simple(double g1,double g2);
RANDLIB_IMPEXP void get_state_fsultra(double g[]);

#endif /** SCI_OTHER_GEN   **/




