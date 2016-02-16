/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre LEDRU
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*----------------------------------------------------------------------------*/
typedef int (*GatefuncS) (char *fname, int l);
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif
/*----------------------------------------------------------------------------*/
