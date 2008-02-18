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
/* file: sci_xsave.h                                                      */
/* desc : interface for sci_xsave routine                                 */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSAVE_H_
#define _INT_XSAVE_H_

/**
* interface function for the xgetmouse routine :
* xsave('fname' [, wid])
*
* @param[in] fname     name of the routine (ie xload)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xsave( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSAVE_H_ */
