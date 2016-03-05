/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#include "machine.h"
/*--------------------------------------------------------------------------*/
/* only for f2c version */
/* only for Visual studio express version */
/* We force commons definition here */
/*--------------------------------------------------------------------------*/
typedef struct
{
    double tsw; /**< @TODO : add comment */
    double rowns2[20]; /**< @TODO : add comment */
    double pdnorm; /**< @TODO : add comment */
    int insufr; /**< @TODO : add comment */
    int insufi; /**< @TODO : add comment */
    int ixpr; /**< @TODO : add comment */
    int iowns2[2]; /**< @TODO : add comment */
    int jtyp; /**< @TODO : add comment */
    int mused; /**< @TODO : add comment */
    int mxordn; /**< @TODO : add comment */
    int mxords; /**< @TODO : add comment */
} LSA001_struct;

LSA001_struct C2F(lsa001);
/*--------------------------------------------------------------------------*/
typedef struct
{
    int nd, iflag;
} ODECD_struct;

ODECD_struct C2F(odecd);
/*--------------------------------------------------------------------------*/
typedef struct
{
    int mesflg, lunit;
} EH0001_struct;

EH0001_struct C2F(eh0001);
/*--------------------------------------------------------------------------*/
typedef struct
{
    char namjac[25];
} CJAC_struct;

CJAC_struct C2F(cjac);
/*--------------------------------------------------------------------------*/
typedef struct
{
    char namef[25];
} CYDOT_struct;

CYDOT_struct C2F(cydot);
/*--------------------------------------------------------------------------*/
typedef  struct
{
    char names[25];
} CSURF_struct;

CSURF_struct C2F(csurf);
/*--------------------------------------------------------------------------*/
