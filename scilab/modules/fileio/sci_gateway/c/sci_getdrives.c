/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "getdrives.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_getdrives(char *fname,unsigned long l)
{
	static int l1=0,n1=0,m1=0;
	char **Drives=NULL;
	int nbDrives=0;

	Rhs=Max(Rhs,0);
	CheckRhs(0,0);
	CheckLhs(0,1);

	Drives=getdrives(&nbDrives);

	if (Drives)
	{
		m1=1;
		n1=nbDrives;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1,Drives);
	}
	else
	{
		m1=0; n1=0; l1=0;
		/* returns a [] */
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
	}

	LhsVar(1) = Rhs+1;
	
	freeArrayOfString(Drives, nbDrives);
	nbDrives = 0;

    PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
