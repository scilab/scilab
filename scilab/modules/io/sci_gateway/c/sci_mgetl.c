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

#include "gw_io.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "IsAScalar.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_mgetl)(char *fname,unsigned long fname_len)
{
	if (Rhs == 2)
	{
		if (VarType(2) != sci_matrix)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: An int scalar expected.\n"),fname,2); 
			return 0;
		}
		else
		{
			if ( !IsAScalar(2) )
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: An int scalar expected.\n"),fname,2); 
				return 0;
			}
		}
	}

	C2F(intmgetl)();
	return 0;
}
/*--------------------------------------------------------------------------*/
