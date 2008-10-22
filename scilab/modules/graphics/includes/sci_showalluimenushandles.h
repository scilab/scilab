/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
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
/* file: sci_showalluimenushandles.h                                      */
/* desc : interface for howalluimenushandles routine                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_SHOWALLUIMENUSHANDLES_H_
#define _INT_SHOWALLUIMENUSHANDLES_H_

#include "ObjectStructure.h"

/**
* interface function for the showalluimenushandles routine.
*
* @param[in] fname     name of the routine (ie showalluimenushandles)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_showalluimenushandles( char * fname, unsigned long fname_len ) ;

int SciShowAllUimenus( sciPointObj* pparent ) ;

#endif /* _INT_SHOWALLUIMENUSHANDLES_H_ */
