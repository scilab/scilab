/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "history.h"
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(gethistory) _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_gethistory) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(gethistory)(fname);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(gethistory) _PARAMS((char *fname))
{

	static int l1, m1, n1;	
	int indice=1,GotoLine;
	sci_hist *Parcours = history;


	Rhs=Max(Rhs,0);
	CheckRhs(0,1) ;
	CheckLhs(1,1) ;

	if (!history)   goto empty;

	if (Rhs == 1) {
		GetRhsVar(1,"i",&m1,&n1,&l1);
		GotoLine=Max(1,*istk(l1)); 
	}
	else {
		GotoLine=1; 
	}

	/* looking for the top of the hystory */
	if (Parcours) Parcours=GoFirstKnot(Parcours);

	/* get the first requested record */
	while  ( Parcours->next ) {	
		if ( indice == GotoLine ) break;
		Parcours=GoNextKnot(Parcours);
		indice++;
	}
	if (!Parcours->next) goto empty;

	if(!CreSmatFromHist(fname, Rhs+1, Parcours)) return 0;
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	return 0;

empty:
	m1=0;
	n1=0;
	CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	return 0;
}	
/*-----------------------------------------------------------------------------------*/
