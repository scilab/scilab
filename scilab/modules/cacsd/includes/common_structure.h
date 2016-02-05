/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
#ifndef __COMMOM_STRUCTURE_CACSD_H__
#define __COMMOM_STRUCTURE_CACSD_H__

#include "machine.h"
#include "dynlib_cacsd.h"

typedef struct
{
    int info, ierr;
} STR_ARL2C;

typedef struct
{
    double gnrm;
} STR_NO2F;

#ifdef _MSC_VER
CACSD_IMPEXP STR_ARL2C C2F(arl2c);
CACSD_IMPEXP STR_NO2F C2F(no2f);
#else
#ifdef __CACSD_GW_HXX__
extern STR_ARL2C C2F(arl2c);
extern STR_NO2F C2F(no2f);
#else
STR_ARL2C C2F(arl2c);
STR_NO2F C2F(no2f);
#endif
#endif
#endif /* !__COMMOM_STRUCTURE_CACSD_H__ */
