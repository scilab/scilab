
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GSCHUR_H__
#define __GSCHUR_H__

#include "AddFunctionInTable.h"
#include "linear_FTables.h"


/***********************************
 * Search Table for gzhsel
 ***********************************/

#define ARGS_gzhsel doublecmplx *,doublecmplx *
typedef int * (*gzhself)(ARGS_gzhsel);

/**************** gzhsel ***************/
extern void C2F(zb02ow)(ARGS_gzhsel);
extern void C2F(zb02ox)(ARGS_gzhsel);
 
FTAB FTab_gzhsel[] ={
{"zb02ow", (voidf)  C2F(zb02ow)},
{"zb02ox", (voidf)  C2F(zb02ox)},
{(char *) 0, (voidf) 0}};


/***********************************
 * Search Table for gshsel
 ***********************************/

#define ARGS_gshsel double *,double*,double*
typedef int * (*gshself)(ARGS_gshsel);


/**************** gshsel ***************/
extern void C2F(sb02ow)(ARGS_gshsel);
extern void C2F(sb02ox)(ARGS_gshsel);
 
FTAB FTab_gshsel[] ={
{"sb02ow", (voidf)  C2F(sb02ow)},
{"sb02ox", (voidf)  C2F(sb02ox)},
{(char *) 0, (voidf) 0}};

#endif /* __GSCHUR_H__ */
