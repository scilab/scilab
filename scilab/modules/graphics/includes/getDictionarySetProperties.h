/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan Cornet
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GETDICTIONARYSETPROPERTIES_H__
#define __GETDICTIONARYSETPROPERTIES_H__


#include "dynlib_graphics.h"

/**
* get all "Set" Handle graphics properties name
* @param[out] size of string matrix
* @return a string matrix
*/
GRAPHICS_IMPEXP char **getDictionarySetProperties(int *sizearray);

#endif /* __GETDICTIONARYSETPROPERTIES_H__ */
/*--------------------------------------------------------------------------*/
