/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "inittypenames.h"
#include "typename.h"
#include "Scierror.h"
#include "do_error_number.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern TYPE_struct C2F(typnams);
/*--------------------------------------------------------------------------*/
static int callError(int ierr);
/*--------------------------------------------------------------------------*/
int C2F(inittypenames)()
{
	int ierr = 0;
	int i = 0;

	/* initialize COMMON (struct) */
	C2F(typnams).ptmax = 1;
	for (i = 0; i < MAX_SCILAB_DEFINED_TYPE; i++)
	{
		C2F(typnams).tp[i] = i;
		C2F(typnams).ln[i] = 0;
		C2F(typnams).ptr[i] = 0;
	}

	/* add types */
	ierr = addNamedType("s",sci_matrix);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("p",sci_poly);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("b",sci_boolean);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("sp",sci_sparse);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("spb",sci_boolean_sparse);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("msp",sci_matlab_sparse);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("i",sci_ints);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("h",sci_handles);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("c",sci_strings);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("m",sci_u_function);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("mc",sci_c_function);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("f",sci_lib);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("l",sci_list);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("tl",sci_tlist);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("ml",sci_mlist);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("ptr",sci_lufact_pointer);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("ip",sci_implicit_poly);
	if (ierr) {callError(ierr); return 1;}

	ierr = addNamedType("fptr",sci_intrinsic_function);
	if (ierr) {callError(ierr); return 1;}

	return 0;
}
/*--------------------------------------------------------------------------*/
int callError(int ierr)
{
	switch (ierr)
	{
	case 1: case 3:
		SciError(224);
		break;
	case 2:
		SciError(225);
		break;
	default:
		/* never here */
		break;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
