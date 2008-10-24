/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __INFFIC_H__
#define __INFFIC_H__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
/** 
* definitions for get_sci_data_strings
* MANUAL_ID returns "$MANCHAPTERS"
* STARTUP_ID returns "exec('SCI/etc/scilab.start',-1);"
* DEMOS_ID returns "scilab_demos();"
* SAVE_ID returns "home/scilab.save"
* QUIT_ID returns "exec('SCI/etc/scilab.quit',-1);quit;"
* QUIT_ERRCATCH_ID returns "exec('SCI/etc/scilab.quit','errcatch',-1);quit;"
*/
#define MANUAL_ID 0 
#define STARTUP_ID 1
#define DEMOS_ID 2
#define SAVE_ID 3
#define QUIT_ID 4
#define QUIT_ERRCATCH_ID 5

void C2F(inffic)(int *iopt, char *name, int *nc);

void C2F(infficl)(int *iopt, int *nc);

char *get_sci_data_strings(int n);

#endif /*__INFFIC_H__*/
/*--------------------------------------------------------------------------*/ 
