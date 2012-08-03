/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre LEDRU
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*----------------------------------------------------------------------------*/
typedef int (*GatefuncS) (char *fname, int l);
/*----------------------------------------------------------------------------*/
/**
 * This function manages the call of a native function which has been added
 * through addinter with PutLhsVar (Compatibility Scilab 4.x and 5.1)
 *
 * @param fname name of the function
 * @param F
 * @param SharedLibraryName name of the library
 */
int sci_gateway(char *fname, GatefuncS F);
/*----------------------------------------------------------------------------*/
/**
* This function manages the call of a native function which has been added
* through addinter without PutLhsVar in gateway (as for internal scilab functions)
*
* @param fname name of the function
* @param F
* @param SharedLibraryName name of the library
*/
int sci_gateway_without_putlhsvar(char *fname, GatefuncS F);
/*----------------------------------------------------------------------------*/
