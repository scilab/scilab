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

#define _NEW_TONIO_

/*--------------------------------------------------------------------------*/
extern int C2F(intisequal) (int *id);
int C2F(intisequalvar)(char * fname, int *job, long int fl); /* the gateway */
/*--------------------------------------------------------------------------*/
int C2F(sci_isequal) (char *fname,unsigned long fname_len)
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iJob = 1;
	C2F(intisequalvar)(fname, &iJob, fname_len );
#else //_NEW_TONIO_
	C2F(intisequal)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
