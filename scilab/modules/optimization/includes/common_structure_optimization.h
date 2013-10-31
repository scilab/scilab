/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __COMMOM_STRUCTURE_OPTIMIZATION_H__
#define __COMMOM_STRUCTURE_OPTIMIZATION_H__

#include "machine.h"
#include "dynlib_optimization.h"

typedef struct
{
    int nizs, nrzs, ndzs;
} STR_NIRD;

typedef struct
{
    double u1;
    int nc;
} STR_FPRF2C;

typedef struct
{
    char nomsub[80];
} STR_OPTIM;

#ifdef _MSC_VER
OPTIMIZATION_IMPEXP STR_NIRD    C2F(nird);
OPTIMIZATION_IMPEXP STR_FPRF2C  C2F(fprf2c);
OPTIMIZATION_IMPEXP STR_OPTIM   C2F(optim);
#else
#ifndef __OPTIMIZATION_GW_HXX__
extern STR_NIRD     C2F(nird);
extern STR_FPRF2C   C2F(fprf2c);
extern STR_OPTIM    C2F(optim);
#else
STR_NIRD    C2F(nird);
STR_FPRF2C  C2F(fprf2c);
STR_OPTIM   C2F(optim);
#endif
#endif
#endif /* !__COMMOM_STRUCTURE_OPTIMIZATION_H__ */
