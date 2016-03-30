/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#ifndef __INFFIC_H__
#define __INFFIC_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
/**
* definitions for get_sci_data_strings
* SAVE_ID returns "home/scilab.save"
* STARTUP_ID returns "exec('SCI/etc/scilab.start',-1);"
* QUIT_ID returns "exec('SCI/etc/scilab.quit',-1);quit;"
* QUIT_ERRCATCH_ID returns "exec('SCI/etc/scilab.quit','errcatch',-1);quit;"
*/

#define SAVE_ID 0
#define STARTUP_ID 1
#define QUIT_ID 2
#define QUIT_ERRCATCH_ID 3
#define MAX_ID QUIT_ERRCATCH_ID

/**
* get string associated with iopt
* used by sigbas fortran routine
*/

void C2F(inffic)(int *iopt, char *name, int *nc);

/**
* get string associated with n
* used by sigbas fortran routine
* @return
*/
char *get_sci_data_strings(int n);

#endif /*__INFFIC_H__*/
/*--------------------------------------------------------------------------*/
