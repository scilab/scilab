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
#ifndef __CLUNI0_H__
#define __CLUNI0_H__
#include "dynlib_fileio.h"
#include "machine.h"

/**
* !!! DEPRECATED see expandPathVariable.h !!!
*/

/**
 * expand in_name to produce out_name
 *
 * @param in_name the input string that we want to expand
 * @param out_name the output string
 * @param out_n number of character of out_name
 * @param lin length of in_name
 * @param lout length of out_name
 * @return 0
 */
FILEIO_IMPEXP int C2F(cluni0)(char *in_name, char *out_name, int *out_n, long int lin, long int lout);

#endif /* __CLUNI0_H__ */
