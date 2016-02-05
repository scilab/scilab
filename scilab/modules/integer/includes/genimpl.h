
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
INTEGER_IMPEXP int C2F(genimpl2)(int *typ, int *first, int *last, int *res);

/**
 * TODO : comment
 * @param typ
 * @param first
 * @param step
 * @param last
 * @param res
 * @return
 */
INTEGER_IMPEXP int C2F(genimpl3)(int *typ, int *first, int *step, int *last, int *res);

/**
 * TODO : comment
 * @param typ
 * @param first
 * @param last
 * @param res
 * @return
 */
INTEGER_IMPEXP int C2F(genimpl2dim)(int *typ, int *first, int *last, int *res);

/**
 * TODO : comment
 * @param typ
 * @param first
 * @param step
 * @param last
 * @param res
 * @return
 */
INTEGER_IMPEXP int C2F(genimpl3dim)(int * typ, int * first, int * step, int * last, int * res);

#endif /* __GENIMPL_H__ */
