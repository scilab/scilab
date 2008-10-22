/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * desc : interface for sci_uimenu routine   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _INT_UIMENU_H_
#define _INT_UIMENU_H_

/**
* interface function for the uimenu routine.
*
* @param[in] fname     name of the routine (ie uimenu)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_uimenu( char * fname, unsigned long fname_len ) ;

#endif /* _INT_UIMENU_H_ */
