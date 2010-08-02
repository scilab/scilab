
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
#ifndef __GENMCUPROD_H__
#define __GENMCUPROD_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param job
 * @param in
 * @param out
 * @param na
 * @param m
 * @param n
 * @return
 */
INTEGER_IMPEXP int C2F(genmcuprod)(int *typ,int *job,void * in, void * out, int * na,int * m,int * n);
INTEGER_IMPEXP int C2F(genmcuprodd)(int *typ,int *job,void * in, double * out, int * na,int * m,int * n);
#endif /* __GENMCUPROD_H__ */

