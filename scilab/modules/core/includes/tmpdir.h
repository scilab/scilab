/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __TMPDIR_H__
#define __TMPDIR_H__

#include <wchar.h>

#include "machine.h"

/**
 * @TODO add comment
 *
 * @param void  
 */
void C2F(settmpdir)(void);

/**
 * @TODO add comment
 *
 * @param void  
 */
void C2F(tmpdirc)(void);

/**
 * returns TMPDIR directory value
 * @param void  
 * @return <ReturnValue>
 */
char *getTMPDIR(void);


/**
* returns TMPDIR directory value (wide string)
* @param void  
* @return <ReturnValue>
*/
wchar_t *getTMPDIRW(void);

#endif /* __TMPDIR_H__ */
/*--------------------------------------------------------------------------*/ 

