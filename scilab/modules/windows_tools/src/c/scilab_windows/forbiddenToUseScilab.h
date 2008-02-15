/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
/*--------------------------------------------------------------------------*/ 
#ifndef __FORBIDDENTOUSESCILAB_H__
#define __FORBIDDENTOUSESCILAB_H__

#include "machine.h" /* BOOL */

/*
* stop scilab if windows < 256 colors
* @return TRUE or FALSE
*/
BOOL forbiddenToUseScilab(void);

#endif /* __FORBIDDENTOUSESCILAB_H__ */
/*--------------------------------------------------------------------------*/ 
