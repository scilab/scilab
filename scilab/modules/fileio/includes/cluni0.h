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
