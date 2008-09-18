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
#ifndef __WARNINGMODE_H__
#define __WARNINGMODE_H__

#include "BOOL.h"
#include "machine.h" /* C2F */

/**
 * @TODO add comment
 *
 * @param ON    
 * @return <ReturnValue>
 */
BOOL setWarningMode(BOOL ON);

/**
 * @TODO add comment
 *
 * @param void  
 * @return <ReturnValue>
 */
BOOL getWarningMode(void);

/**
 * fortran version of getWarningMode 
 *
 * @param rep   
 * @return <ReturnValue>
 */
int C2F(iswarningon)(int *rep);

#endif /*  __WARNINGMODE_H__ */
/*--------------------------------------------------------------------------*/
