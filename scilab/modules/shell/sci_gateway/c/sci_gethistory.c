/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "history.h"
#include "gw_core.h"
#include "gw_shell.h"
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_gethistory) _PARAMS((char *fname,unsigned long fname_len))
{
/*	static int l1 = 0, m1 = 0, n1 = 0;	
	int indice=1,GotoLine = 0;
	sci_hist *Parcours = history;
	
	CheckRhs(0,1);
	CheckLhs(1,1);

	if (!history)
	{
		m1=0; n1=0;
		CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		return 0;
	}

	if (Rhs == 1) 
	{
		GetRhsVar(1,"i",&m1,&n1,&l1);
		GotoLine=Max(1,*istk(l1)); 
	}
	else 
	{
		GotoLine=1; 
	}

	/* looking for the top of the hystory */
/*	if (Parcours) Parcours=GoFirstKnot(Parcours);

	/* get the first requested record */
/*	while  ( Parcours->next ) 
	{
		if ( indice == GotoLine ) break;
		Parcours=GoNextKnot(Parcours);
		indice++;
	}

	if (!Parcours->next)
	{
		m1=0; n1=0;
		CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		return 0;
	}

	if (!CreSmatFromHist(fname, Rhs+1, Parcours)) return 0;
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
*/
static int l1 = 0, m1 = 0, n1 = 0;
		m1=0; n1=0;
		CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();

	return 0;

}
/*-----------------------------------------------------------------------------------*/
