/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __SYSTEMC_H__
#define __SYSTEMC_H__

#include "machine.h" /* C2F */

/**
 * Execute a command by the system
 * @param[in] command the command
 * @param[out] the status of the operation
 */

int C2F(systemc)(char *command, int *stat);

#endif
