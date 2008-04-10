/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
/*--------------------------------------------------------------------------*/

#define _NEW_TONIO_
extern int C2F(intexp) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_exp) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex;
	
	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}


#else
	C2F(intexp)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
