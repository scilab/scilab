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
/* file: sci_demo.h                                                       */
/* desc : function used to laubch a demo of a command                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_DEMO_
#define _INT_DEMO_

/**
 * Launch a demo of a scilab routine
 * @param[in] fname Came of the routine which demo is launched
 * @param[in] code  Scilab script of the demo
 * @param[in] flagx If *flagx == 1, display the code of the demo in the
 *                  console. Otherwise, silent execution.
 */
int sci_demo( char * fname, char * code, int * flagx ) ;

#endif /* _INT_DEMO_ */
