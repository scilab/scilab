/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "error_internal.h"
#include "sciprint.h"
#include "msgstore.h"
#include "BOOL.h"
#include "stack-def.h"
#include "errmsg.h"
#include "errmds.h"
/*--------------------------------------------------------------------------*/ 
extern int C2F(errloc)(); /* fortran */
extern int C2F(errmgr)(); /* fortran */
extern int C2F(errcontext)(); /* fortran */

/*--------------------------------------------------------------------------*/ 
int error_internal(int *n,char *buffer,int mode)
{
	int num = 0;
	int lct1 = 0;
	int imode = 0;
	int imess = 0;
	int errtyp = 0;

	/* extract error modes out of errct variable */
	C2F(errmds)(&num, &imess, &imode);

	/* de-activate output control */
	lct1 = C2F(iop).lct[0];
	C2F(iop).lct[0] = 0;

	/* errors are recoverable */
	errtyp = 0;

	if (C2F(errgst).err1 == 0) 
	{
		BOOL trace = ! ((num < 0 || num == *n) && imess != 0);
		/* locate the error in the current statement */
		if (trace) 
		{
			C2F(errloc)(n);
		}
		else
		{
			/* output error message */
			C2F(iop).lct[0] = -1;
		}

		if (mode == ERROR_FROM_FORTRAN)
		{
			C2F(errmsg)(n, &errtyp);
		}
		else /* ERROR_FROM_C */
		{
			int len = (int) strlen(buffer);

			/* free message table */
			C2F(freemsgtable)();

			/* store error number */
			C2F(errstore)(n);

			/* store message */
			C2F(msgstore)(buffer,&len);

			/* display error */
			if (C2F(iop).lct[0] != -1) sciprint(buffer);
		}
		C2F(iop).lct[0] = 0;
	}
	C2F(errcontext)(); 
	/* handle the error */
	C2F(errmgr)(n, &errtyp);

	/* re-activate output control */
	C2F(iop).lct[0] = lct1;

	return 0;
}
/*--------------------------------------------------------------------------*/ 
