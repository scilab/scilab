/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Antoine ELIAS
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
	#include <hdf5.h>
	#include <string.h>
	#include <stdio.h>
	#include "gw_fileio.h"
	#include "stack-c.h"
	#include "MALLOC.h"
	#include "Scierror.h"
	#include "localization.h"
	#include "sciprint.h"
	#include "variable_api.h"
	#include "../../call_scilab/includes/CallScilab.h"
}

int sci_import_from_hdf5(char *fname,unsigned long fname_len)
{
	PutLhsVar();
	return 0;
}

/*--------------------------------------------------------------------------*/
