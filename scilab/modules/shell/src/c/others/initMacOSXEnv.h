/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "realmain.h"

/* 
 * Specific wrapper for mac os X which is going to call realmin in a specific
 * thread.
 * Takes the same args as realmain
 * @param no_startup_flag_l
 * @param initial_script the initial script given to Scilab
 * @param initial_script_type Kind of script (file or code)
 * @param memory
 * @return 1 if OK or 0 if KO
 */ 

int initMacOSXEnv(int no_startup_flag_l, char *initial_script, InitScriptType initial_script_type, int memory);
