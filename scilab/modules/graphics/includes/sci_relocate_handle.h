/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_relocate_handle.h                                            */
/* desc : interface for relocate_handle routine                           */
/*------------------------------------------------------------------------*/

#ifndef _INT_RELOCATE_HANDLE_H_
#define _INT_RELOCATE_HANDLE_H_

/**
* interface function for the relocate_handle routine.
*
* Method called for relocating handles. The input should contains a vector of handles
* which will be relocated and a single handle which is the new parent.
*
* @param[in] fname     name of the routine (ie relocate_handle)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_relocate_handle( char * fname, unsigned long fname_len ) ;

#endif /* _INT_RELOCATE_HANDLE_H_ */
