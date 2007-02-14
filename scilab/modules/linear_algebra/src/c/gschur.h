/**************************************
 * Sylvestre Ledru - INRIA 2007
 **************************************/

#include "linear_FTables.h"


/***********************************
 * Search Table for gzhsel
 ***********************************/

#if defined(__STDC__)
#define ARGS_gzhsel doublecmplx *,doublecmplx *
typedef integer * (*gzhself)(ARGS_gzhsel);
#else
#define ARGS_gzhsel /**/
typedef integer * (*gzhself)();
#endif 

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

#if defined(__STDC__)
#define ARGS_gshsel double *,double*,double*
typedef integer * (*gshself)(ARGS_gshsel);
#else
#define ARGS_gshsel /**/
typedef integer * (*gshself)();
#endif 


/**************** gshsel ***************/
extern void C2F(sb02ow)(ARGS_gshsel);
extern void C2F(sb02ox)(ARGS_gshsel);
 
FTAB FTab_gshsel[] ={
{"sb02ow", (voidf)  C2F(sb02ow)},
{"sb02ox", (voidf)  C2F(sb02ox)},
{(char *) 0, (voidf) 0}};
