
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENIMPL_H__
#define __GENIMPL_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param first
 * @param last
 * @param res
 * @return 
 */
INTEGER_IMPEXP int C2F(genimpl2)(int *typ,int *first,int *last,int *res);

/**
 * TODO : comment 
 * @param typ
 * @param first
 * @param step
 * @param last
 * @param res
 * @return 
 */
INTEGER_IMPEXP int C2F(genimpl3)(int *typ,int *first,int *step,int *last,int *res);

/**
 * TODO : comment 
 * @param typ
 * @param first
 * @param last
 * @param res
 * @return 
 */
INTEGER_IMPEXP int C2F(genimpl2dim)(int *typ,int *first,int *last,int *res);

/**
 * TODO : comment 
 * @param typ
 * @param first
 * @param step
 * @param last
 * @param res
 * @return 
 */
INTEGER_IMPEXP int C2F(genimpl3dim)(int * typ,int * first,int * step,int * last,int * res);

#endif /* __GENIMPL_H__ */
