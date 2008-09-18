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

#ifndef __SCIHOME_H__
#define __SCIHOME_H__

#include "BOOL.h"

/**
* Set SCIHOME environment variable
* @return TRUE or FALSE
*/
BOOL setSCIHOME(void);

/**
* get SCIHOME  environment variable
* @return a string SCIHOME
*/
char *getSCIHOME(void);

#endif /* __SCIHOME_H__ */
/*--------------------------------------------------------------------------*/ 
