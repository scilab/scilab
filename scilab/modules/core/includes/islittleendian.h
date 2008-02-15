/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __ISLITTLEENDIAN_H__
#define __ISLITTLEENDIAN_H__

#include "machine.h"

/**
* islittleendian 
* @return if it is a little endian (0 or 1)
*/
int islittleendian(void);

/**
* C2F(getendian)
* @param void  
* @return the endian (0 or 1)
*/
int C2F(getendian)(void);

#endif /* __ISLITTLEENDIAN_H__ */
/*--------------------------------------------------------------------------*/
