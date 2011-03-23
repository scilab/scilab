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
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER

/* BUG 3863 */
/* forces to define C2F(dcoeff) only once */

typedef struct {
	double c[41];
	int ndng;
} DCOEFF_struct;

__declspec(dllexport) DCOEFF_struct C2F(dcoeff);

#endif

extern int C2F(intexpm)(int *id);
/*--------------------------------------------------------------------------*/
int sci_expm(char *fname,unsigned long fname_len)
{
	static int id[6];
	C2F(intexpm)(id);
	return 0;
}
/*--------------------------------------------------------------------------*/
