
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
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

#ifndef __GSCHUR_H__
#define __GSCHUR_H__

#include "dynlib_linear_algebra.h"
#include "GetFunctionByName.h"
#include "linear_FTables.h"


/***********************************
 * Search Table for gzhsel
 ***********************************/

#define ARGS_gzhsel doublecmplx *,doublecmplx *
typedef int * (*gzhself)(ARGS_gzhsel);

/**************** gzhsel ***************/
LINEAR_ALGEBRA_IMPEXP void C2F(zb02ow)(ARGS_gzhsel);
LINEAR_ALGEBRA_IMPEXP void C2F(zb02ox)(ARGS_gzhsel);

FTAB FTab_gzhsel[] =
{
    {"zb02ow", (voidf)  C2F(zb02ow)},
    {"zb02ox", (voidf)  C2F(zb02ox)},
    {(char *) 0, (voidf) 0}
};


/***********************************
 * Search Table for gshsel
 ***********************************/

#define ARGS_gshsel double *,double*,double*
typedef int * (*gshself)(ARGS_gshsel);


/**************** gshsel ***************/
LINEAR_ALGEBRA_IMPEXP void C2F(sb02ow)(ARGS_gshsel);
LINEAR_ALGEBRA_IMPEXP void C2F(sb02ox)(ARGS_gshsel);

FTAB FTab_gshsel[] =
{
    {"sb02ow", (voidf)  C2F(sb02ow)},
    {"sb02ox", (voidf)  C2F(sb02ox)},
    {(char *) 0, (voidf) 0}
};

#endif /* __GSCHUR_H__ */
