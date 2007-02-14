#ifndef __FTABLES__
#define __FTABLES__
#include "machine.h"
/******* Please do not edit *************/










/**************** fbutn ***************/
extern void haltscicos(ARGS_fbutn);
void C2F(fbutn)(ARGS_fbutn);
void C2F(setfbutn)(char *name, int *rep);

FTAB FTab_fbutn[] ={
{"haltscicos", (voidf)  haltscicos},
{(char *) 0, (voidf) 0}};
/**************** interf ***************/
extern void C2F(foubare)(ARGS_interf);
extern void C2F(foobar1)(ARGS_interf);
extern void C2F(icomp)(ARGS_interf);
extern void C2F(loset)(ARGS_interf);
extern void C2F(arset)(ARGS_interf);
extern void C2F(icse0)(ARGS_interf);
extern void C2F(icse)(ARGS_interf);
extern void C2F(icscof)(ARGS_interf);
void C2F(interf)(ARGS_interf);
void C2F(interf1)(ARGS_interf1);
void C2F(setinterf)(char *name, int *rep);

FTAB FTab_interf[] ={
{"arset", (voidf)  C2F(arset)},
{"foobar1", (voidf)  C2F(foobar1)},
{"foubare", (voidf)  C2F(foubare)},
{"icomp", (voidf)  C2F(icomp)},
{"icscof", (voidf)  C2F(icscof)},
{"icse", (voidf)  C2F(icse)},
{"icse0", (voidf)  C2F(icse0)},
{"loset", (voidf)  C2F(loset)},
{(char *) 0, (voidf) 0}};

/***********************************/
#endif /*__FTABLES__*/
