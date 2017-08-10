/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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

typedef struct {
    double t0, tf, dti, dtf, ermx;
    int iu[5], nuc, nuv, ilin, nti, ntf, ny, nea, itmx, nex,
        nob, ntob, ntobi, nitu, ndtu;
} STR_ICSEZ;


#ifdef _MSC_VER
OPTIMIZATION_IMPEXP STR_NIRD    C2F(nird);
OPTIMIZATION_IMPEXP STR_FPRF2C  C2F(fprf2c);
OPTIMIZATION_IMPEXP STR_OPTIM   C2F(optim);
OPTIMIZATION_IMPEXP STR_ICSEZ   C2F(icsez);

#else
#ifndef __OPTIMIZATION_GW_HXX__
extern STR_NIRD     C2F(nird);
extern STR_FPRF2C   C2F(fprf2c);
extern STR_OPTIM    C2F(optim);
extern STR_ICSEZ    C2F(icsez);
#else
STR_NIRD    C2F(nird);
STR_FPRF2C  C2F(fprf2c);
STR_OPTIM   C2F(optim);
STR_ICSEZ    C2F(icsez);
#endif
#endif
#endif /* !__COMMOM_STRUCTURE_OPTIMIZATION_H__ */
