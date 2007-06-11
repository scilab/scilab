/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
#include "getinstalledlookandfeels.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getinstalledlookandfeels) _PARAMS((char *fname,unsigned long fname_len))
{
	
	char **lookandfeels = NULL;
	int nbElems = 0;
	int nbCol = 0;

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,1);

	lookandfeels = getinstalledlookandfeels(&nbElems);

	nbCol = 1;
	CreateVarFromPtr( Rhs+1, "S", &nbElems, &nbCol,lookandfeels ) ;

	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();

	if (lookandfeels)
	{
		int i = 0;
		for (i=0;i<nbElems;i++)
		{
			if (lookandfeels[i])
			{
				FREE(lookandfeels[i]);
				lookandfeels[i]=NULL;
			}
		}
		FREE(lookandfeels);
		lookandfeels=NULL;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
