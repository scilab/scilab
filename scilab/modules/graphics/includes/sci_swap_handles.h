/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: sci_swap_handles.h                                               */
/* desc : interface for swap_handles routine                              */
/*------------------------------------------------------------------------*/

#ifndef _INT_SWAP_HANDLES_H_
#define _INT_SWAP_HANDLES_H_

/**
* interface function for the swap_handles routine.
*
* This method is called to swap two handles position in the hierarchy.
* The input should be two single handle which will be swaped.
*
* @param[in] fname     name of the routine (ie swap_handles)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_swap_handles( char * fname, unsigned long fname_len ) ;

#endif /* _INT_SWAP_HANDLES_H_ */
