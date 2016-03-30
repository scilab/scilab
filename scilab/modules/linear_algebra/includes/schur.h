
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

#ifndef __SCHUR_H__
#define __SCHUR_H__

#include "dynlib_linear_algebra.h"
#include "machine.h"
#include "linear_FTables.h"

/***********************************
 * Search Table for schsel
 ***********************************/

#define ARGS_schsel double *,double*
typedef int * (*schself)(ARGS_schsel);

/**
 * function call : schsel
 * @param alpha
 * @param beta
 * @return
 */
LINEAR_ALGEBRA_IMPEXP int *C2F(schsel)(double *alpha, double *beta);

/**
 * fixes the function associated to name
 * @param len
 * @param name
 * @param rep
 */
LINEAR_ALGEBRA_IMPEXP void C2F(setschsel)(int *len, char *name, int *rep);

/**************** schsel ***************/
LINEAR_ALGEBRA_IMPEXP void C2F(sb02mv)(ARGS_schsel);
LINEAR_ALGEBRA_IMPEXP void C2F(sb02mw)(ARGS_schsel);

FTAB FTab_schsel[] =
{
    {"sb02mv", (voidf)  C2F(sb02mv)},
    {"sb02mw", (voidf)  C2F(sb02mw)},
    {(char *) 0, (voidf) 0}
};


/***********************************
* Search Table for gschur uses : gshsel
***********************************/
/**
 * @TODO : add comment
 *
 * @param alphar
 * @param alphai
 * @param beta
 * @return <ReturnValue>
 */
LINEAR_ALGEBRA_IMPEXP int *C2F(gshsel)(double *alphar, double *alphai, double *beta);

/**
 *  fixes the function associated to name
 *
 * @param len
 * @param name
 * @param rep
 */
LINEAR_ALGEBRA_IMPEXP void C2F(setgshsel)(int *len, char *name, int *rep);


/***********************************
* Search Table for schur uses : fschur
***********************************/

/**
 * @TODO : add comment
 *
 * @param alpha
 * @param beta
 * @return <ReturnValue>
 */
LINEAR_ALGEBRA_IMPEXP int *C2F(gzhsel)(doublecmplx *alpha, doublecmplx *beta);

/**
 *  fixes the function associated to name
 *
 * @param len
 * @param name
 * @param rep
 */
LINEAR_ALGEBRA_IMPEXP void C2F(setgzhsel)(int *len, char *name, int *rep);

/***********************************
* Search Table for gschur uses : gzhsel
***********************************/
/**
 * @TODO : add comment
 *
 * @param alpha
 * @param beta
 * @return <ReturnValue>
 */
LINEAR_ALGEBRA_IMPEXP int *C2F(gzhsel)(doublecmplx *alpha, doublecmplx *beta);

/**
 *  fixes the function associated to name
 *
 * @param len
 * @param name
 * @param rep
 */
LINEAR_ALGEBRA_IMPEXP void C2F(setgzhsel)(int *len, char *name, int *rep);


/***********************************
* Search Table for schur uses : zchsel
***********************************/
/**
 * @TODO : add comment
 *
 * @param alpha
 * @return <ReturnValue>
 */
LINEAR_ALGEBRA_IMPEXP int *C2F(zchsel)(doublecmplx *alpha);

/**
 *  fixes the function associated to name
 *
 * @param len
 * @param name
 * @param rep
 */
LINEAR_ALGEBRA_IMPEXP void C2F(setzchsel)(int *len, char *name, int *rep);

/***********************************
* Search Table for schur uses : fschur
***********************************/

/**
 * @TODO add comment
 *
 * @param lsize
 * @param alpha
 * @param beta
 * @param s
 * @param p
 * @return <ReturnValue>
 */
LINEAR_ALGEBRA_IMPEXP int *C2F(fschur)(int *lsize, double *alpha, double *beta, double *s, double *p);

/**
 *  fixes the function associated to name
 *
 * @param name
 * @param rep
 */
LINEAR_ALGEBRA_IMPEXP void C2F(setfschur)(char *name, int *rep);



/***********************************
 * Search Table for zchsel
 ***********************************/

#define ARGS_zchsel doublecmplx *
typedef int * (*zchself)(ARGS_zchsel);


/**************** zchsel ***************/
LINEAR_ALGEBRA_IMPEXP void C2F(zb02mv)(ARGS_zchsel);
LINEAR_ALGEBRA_IMPEXP void C2F(zb02mw)(ARGS_zchsel);

FTAB FTab_zchsel[] =
{
    {"zb02mv", (voidf)  C2F(zb02mv)},
    {"zb02mw", (voidf)  C2F(zb02mw)},
    {(char *) 0, (voidf) 0}
};

#endif /* __SCHUR_H__ */
