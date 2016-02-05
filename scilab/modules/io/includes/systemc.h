/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
#ifndef __SYSTEMC_H__
#define __SYSTEMC_H__

#include "dynlib_io.h"
/**
 * Execute a command by the system
 * @param[in] command the command
 * @param[out] the status of the operation
 */
IO_IMPEXP int systemc(char *command, int *stat);

/**
 * Execute a command by the system
 * @param[in] command the command
 * @param[out] the status of the operation
 */
IO_IMPEXP int systemcW(wchar_t* _pstCommand, int *stat);

#endif
