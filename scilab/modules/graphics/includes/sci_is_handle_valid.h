/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_is_handle_valid.h                                            */
/* desc : interface for is_handle_valid routine                           */
/*------------------------------------------------------------------------*/

#ifndef _SCI_IS_HANDLE_VALID_H_
#define _SCI_IS_HANDLE_VALID_H_

/**
* interface function for the param3d routine :
* isValid = is_handle_valid(h)
*
* @param[in] fname      name of the routine (ie is_handle_valid)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_is_handle_valid( char * fname, unsigned long fname_len ) ;

#endif /* _SCI_IS_HANDLE_VALID_H_ */
