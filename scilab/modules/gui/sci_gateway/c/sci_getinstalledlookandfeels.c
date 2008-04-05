/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "MALLOC.h"
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
#include "getinstalledlookandfeels.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getinstalledlookandfeels)(char *fname,unsigned long fname_len)
{
	
	char **lookandfeels = NULL;
	int nbElems = 0;
	int nbCol = 0;

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,1);

	lookandfeels = getinstalledlookandfeels(&nbElems);

	nbCol = 1;
	CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &nbElems, &nbCol,lookandfeels );

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
/*--------------------------------------------------------------------------*/
