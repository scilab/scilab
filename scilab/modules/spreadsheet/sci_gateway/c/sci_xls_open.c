
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
 * Copyright (C) 2005-2008 - INRIA - Pierrick MODE
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack-c.h"
#include "gw_spreadsheet.h"
#include "xls.h"
#include "ole.h"
#include "ripole.h"
#include "expandPathVariable.h"
#include "MALLOC.h"
#include "../../../fileio/includes/mopen.h"
#include "Scierror.h"
#include "localization.h"
#include "tmpdir.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
static char *xls_basename (char *name);
/*--------------------------------------------------------------------------*/
int sci_xls_open(char *fname,unsigned long fname_len)
{
#undef IN
	#define max_char_xls_open 256
	int i = 0,k = 0,m1 = 0,n1 = 0,l1 = 0,l2 = 0,one = 1,fd = 0,f_swap = 0;
	int ierr = 0,ns = 0,result = 0;
	double res;
	char **sst = NULL;
	char **Sheetnames = NULL;
	int *Abspos = NULL;
	int nsheets = 0;
	char *filename_IN = NULL;
	char TMP[max_char_xls_open];

	char sep[2];
	char *TMPDIR = NULL;
#ifdef _MSC_VER
	sep[0]='\\';
#else
	sep[0]='/';
#endif
	sep[1]='\0';

	CheckLhs(4,4);
	CheckRhs(1,1);

	if (VarType(1) != sci_strings)
	{
		Scierror(999,"%s: Invalid type of input argument: String expected.", fname);
		return 0;
	}

	/*  checking variable file */
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

	filename_IN = expandPathVariable(cstk(l1));

	TMPDIR = getTMPDIR();
	strcpy(TMP, TMPDIR);
	if (TMPDIR){FREE(TMPDIR);TMPDIR=NULL;}

	strcat(TMP,sep);
	strcat(TMP,xls_basename(filename_IN));
	result = ripole(filename_IN, TMP, 0, 0);
	if (result != OLE_OK) 
	{
		if (result == OLEER_NO_INPUT_FILE) 
		{
			Scierror(999,_("The file %s does not exist.\n"), filename_IN);
		}
		else if(result == OLEER_NOT_OLE_FILE || 
			result == OLEER_INSANE_OLE_FILE || 
			result == OLEER_LOADFAT_BAD_BOUNDARY || 
			result == OLEER_MINIFAT_READ_FAIL || 
			result == OLEER_PROPERTIES_READ_FAIL)
		{
			Scierror(999,_("%s: File %s is not an ole2 file.\n"),fname, filename_IN);
			if (filename_IN) {FREE(filename_IN); filename_IN = NULL;}
		}
		else if(result == -1)
		{
			Scierror(999,_("%s: Cannot open file %s.\n"),fname, filename_IN);
			if (filename_IN) {FREE(filename_IN); filename_IN = NULL;}
		}
		return 0;
	}
	strcat(TMP,sep);
	strcat(TMP,"Workbook");
	C2F(mopen)(&fd, TMP,"rb", &f_swap, &res, &ierr);
	if (ierr != 0)
	{
		Scierror(999,_("%s: There is no xls stream in the ole2 file %s.\n"),fname, filename_IN);
		if (filename_IN) {FREE(filename_IN); filename_IN = NULL;}
		return 0;
	}

	if (filename_IN) {FREE(filename_IN); filename_IN = NULL;}

	CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&one,&one,&l2);
	*istk(l2)=fd; /* logical unit */

	xls_open(&ierr, &fd, &sst ,&ns, &Sheetnames, &Abspos,&nsheets);
	/*return *err:
	0 = OK
	1 = not an OLE file
	2 = no Workbook included
	3 = memory allocation problem
	4 = incorrect file
	5 = not a BIFF8 xls file
	*/
	switch (ierr)
	{
		case 0:
			/* OK */
			break;

		case 1:
			Scierror(999,_("%s: Not an ole2 file.\n"),fname);
			return 0;
			break;

		case 2:
			Scierror(999,_("%s: The file has no Workbook directory.\n"),fname);
			return 0;
			break;

		case 3:
			Scierror(999,_("%s: No more memory.\n"),fname);
			return 0;
			break;

		case 4:
			Scierror(990,_("%s: Incorrect or corrupted file.\n"),fname);
			return 0;
			break;

		case 5:
			Scierror(999,_("%s: Only BIFF8 file format is handled.\n"),fname);
			return 0;
			break;
		default :
			break;

	}

	if (ns != 0) 
	{
		/* Create a typed list to return the properties */
		CreateVarFromPtr(Rhs+2,MATRIX_OF_STRING_DATATYPE, &one, &ns, sst);
		freeArrayOfString(sst, ns);
	}
	else CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&ns,&ns,&l2);

	if (nsheets != 0) 
	{
		/* Create a typed list to return the properties */
		CreateVarFromPtr(Rhs+3,MATRIX_OF_STRING_DATATYPE, &one, &nsheets, Sheetnames);
		freeArrayOfString(Sheetnames, nsheets);

		CreateVar(Rhs+4,MATRIX_OF_DOUBLE_DATATYPE, &one, &nsheets, &l2);
		for (i = 0;i < nsheets;i++) *stk(l2+i) = Abspos[i];
		if (Abspos)
		{
			FREE(Abspos);
			Abspos = NULL;
		}
	}
	else 
	{
		CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,&nsheets,&nsheets,&l2);
		CreateVar(Rhs+4,MATRIX_OF_DOUBLE_DATATYPE,&nsheets,&nsheets,&l2);
	}

	LhsVar(1)= Rhs+1;
	LhsVar(2)= Rhs+2;
	LhsVar(3)= Rhs+3;
	LhsVar(4)= Rhs+4;

	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
static char *xls_basename (char *name)
{
	char *base=NULL;
#ifdef _MSC_VER
	base = strrchr (name, '\\');
#else
	base = strrchr (name, '/');
#endif
	return base ? base + 1 : name;
}
/*--------------------------------------------------------------------------*/
