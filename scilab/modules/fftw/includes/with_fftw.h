/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __WITH_FFTW_H__
#define __WITH_FFTW_H__

#include "machine.h" /* C2F , BOOL*/

/** 
* check if it is with FFTW
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
int C2F(withfftw)(int *rep);

/** 
* check if it is with FFTW
* @return TRUE or FALSE
*/
BOOL withfftw(void);

#endif /* __WITH_FFTW_H__ */
/*--------------------------------------------------------------------------*/ 
