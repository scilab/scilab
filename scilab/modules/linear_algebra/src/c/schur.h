/**************************************
 * Sylvestre Ledru - INRIA 2007
 **************************************/

#include "machine.h"
#include "linear_FTables.h"

/***********************************
 * Search Table for schsel
 ***********************************/

#if defined(__STDC__)
#define ARGS_schsel double *,double*
typedef integer * (*schself)(ARGS_schsel);
#else
#define ARGS_schsel /**/
typedef integer * (*schself)();
#endif 


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

#if defined(__STDC__)
#define ARGS_zchsel doublecmplx *
typedef integer * (*zchself)(ARGS_zchsel);
#else
#define ARGS_zchsel /**/
typedef integer * (*zchself)();
#endif 


/**************** zchsel ***************/
extern void C2F(zb02mv)(ARGS_zchsel);
extern void C2F(zb02mw)(ARGS_zchsel);
 
FTAB FTab_zchsel[] ={
{"zb02mv", (voidf)  C2F(zb02mv)},
{"zb02mw", (voidf)  C2F(zb02mw)},
{(char *) 0, (voidf) 0}};
