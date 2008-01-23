/**************************************
 * Sylvestre Ledru - INRIA 2007
 **************************************/
#ifndef __GSCHUR_H__
#define __GSCHUR_H__

#include "AddFunctionInTable.h"
#include "linear_FTables.h"


/***********************************
 * Search Table for gzhsel
 ***********************************/

#define ARGS_gzhsel doublecmplx *,doublecmplx *
typedef integer * (*gzhself)(ARGS_gzhsel);

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
typedef integer * (*gshself)(ARGS_gshsel);


/**************** gshsel ***************/
extern void C2F(sb02ow)(ARGS_gshsel);
extern void C2F(sb02ox)(ARGS_gshsel);
 
FTAB FTab_gshsel[] ={
{"sb02ow", (voidf)  C2F(sb02ow)},
{"sb02ox", (voidf)  C2F(sb02ox)},
{(char *) 0, (voidf) 0}};

#endif /* __GSCHUR_H__ */