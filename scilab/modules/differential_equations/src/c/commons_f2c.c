/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
/* only for f2c version */
/* only for Visual studio express version */
/* We force commons definition here */
/*--------------------------------------------------------------------------*/ 
typedef struct {
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
typedef struct {
    int nd, iflag;
} ODECD_struct;

ODECD_struct C2F(odecd);
/*--------------------------------------------------------------------------*/
typedef struct {
    int mesflg, lunit;
} EH0001_struct;

EH0001_struct C2F(eh0001);
/*--------------------------------------------------------------------------*/
typedef struct {
    char namjac[25];
} CJAC_struct;

CJAC_struct C2F(cjac);
/*--------------------------------------------------------------------------*/
typedef struct {
    char namef[25];
} CYDOT_struct;

CYDOT_struct C2F(cydot);
/*--------------------------------------------------------------------------*/
typedef  struct {
    char names[25];
} CSURF_struct;

CSURF_struct C2F(csurf);
/*--------------------------------------------------------------------------*/
