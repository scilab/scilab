/*--------------------------------------------------------------------------*/
/* INRIA */
/*--------------------------------------------------------------------------*/
#include "FTables.h"
/***********************************
* Search Table for fbutn
***********************************/


#define ARGS_fbutn char *,integer*,integer*
typedef integer * (*fbutnf)(ARGS_fbutn);

/***************** fbutn ***************/
extern void haltscicos(ARGS_fbutn);
void C2F(fbutn)(ARGS_fbutn);
void C2F(setfbutn)(char *name, int *rep);

FTAB FTab_fbutn[] =
{
	{"haltscicos", (voidf)  haltscicos},
	{(char *) 0, (voidf) 0}
};




/***********************************
* Search Table for fbutn
**********************************/

/** the current function fixed by setfbutn **/

static fbutnf fbutnfonc ;

/** function call **/

void C2F(fbutn)(char *name, integer *win, integer *entry)
{
	(*fbutnfonc)(name,win,entry);
}

/** fixes the function associated to name **/

void C2F(setfbutn)(char *name, int *rep)
{
	fbutnfonc = (fbutnf) SetFunction(name,rep,FTab_fbutn);
}
