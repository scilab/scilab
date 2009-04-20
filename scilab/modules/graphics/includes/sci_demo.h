/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_demo.h                                                       */
/* desc : function used to laubch a demo of a command                     */
/*------------------------------------------------------------------------*/

#ifndef _SCI_DEMO_H_
#define _SCI_DEMO_H_

#include "BOOL.h"

/**
 * Lacunch some code inside Scilab.
 * @param[in] fname Name of the routine from which demo is launched
 * @param[in] fname_len length of fname
 */
int sci_demo( char * fname, int fname_len);

#endif /* _SCI_DEMO_H_ */
