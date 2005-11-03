/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intshowalluimenushandles.h"
/*-----------------------------------------------------------------------------------*/
extern int version_flag();
extern sciPointObj *sciGetPointerFromHandle (long handle);
extern sciEntityType sciGetEntityType (sciPointObj * pobj);
extern sciSons *sciGetLastSons (sciPointObj * pobj);
/*-----------------------------------------------------------------------------------*/
int SciShowAllUimenus(sciPointObj* pparent);
/*-----------------------------------------------------------------------------------*/
int intshowalluimenushandles(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);
	
	if (version_flag()!=0)
	{
		Scierror(999,"Not valid under old graphics mode.");
		return 0;
	}

	if (VarType(1) == sci_handles)
	{
		sciPointObj *pParent=NULL;
		int m1,n1,l1;

		GetRhsVar(1,"h",&m1,&n1,&l1);
		pParent=sciGetPointerFromHandle((long)*hstk(l1));

		if (sciGetEntityType (pParent) != SCI_FIGURE ) 
		{
			Scierror(999,"Invalid parameter. must be a Figure handle");
			return 0;
		}
		else
		{
			SciShowAllUimenus(pParent);
		}
	}
	else
	{
		Scierror(999,"Invalid parameter. must be a Figure handle");
		return 0;
	}
	LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int SciShowAllUimenus(sciPointObj* pparent)
{
	sciSons *psonstmp;
	psonstmp = sciGetLastSons(pparent);

	while(psonstmp != (sciSons *) NULL)
	{   
		if(sciGetEntityType (psonstmp->pointobj) == SCI_UIMENU)
		{
			pUIMENU_FEATURE(psonstmp->pointobj)->handle_visible=TRUE;
			SciShowAllUimenus(psonstmp->pointobj);
		}

		psonstmp = psonstmp->pprev;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
