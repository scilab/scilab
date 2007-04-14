/**************************************
 * Sylvestre Ledru - INRIA 2007
 **************************************/
#ifndef __SCHUR_H__
#define __SCHUR_H__

#include "machine.h"
#include "linear_FTables.h"

/***********************************
 * Search Table for schsel
 ***********************************/

#define ARGS_schsel double *,double*
typedef integer * (*schself)(ARGS_schsel);


/**************** schsel ***************/
extern void C2F(sb02mv)(ARGS_schsel);
extern void C2F(sb02mw)(ARGS_schsel);
 
FTAB FTab_schsel[] ={
{"sb02mv", (voidf)  C2F(sb02mv)},
{"sb02mw", (voidf)  C2F(sb02mw)},
{(char *) 0, (voidf) 0}};


/***********************************
 * Search Table for zchsel
 ***********************************/

#define ARGS_zchsel doublecmplx *
typedef integer * (*zchself)(ARGS_zchsel);


/**************** zchsel ***************/
extern void C2F(zb02mv)(ARGS_zchsel);
extern void C2F(zb02mw)(ARGS_zchsel);
 
FTAB FTab_zchsel[] ={
{"zb02mv", (voidf)  C2F(zb02mv)},
{"zb02mw", (voidf)  C2F(zb02mw)},
{(char *) 0, (voidf) 0}};

#endif /* __SCHUR_H__ */