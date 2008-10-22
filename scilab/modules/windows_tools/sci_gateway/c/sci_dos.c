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

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "gw_windows_tools.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "MALLOC.h" /* MALLOC */
#include "spawncommand.h"
#include "sciprint.h"
#include "tmpdir.h"
#include "FileExist.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
#define BUFSIZE 4096
/*--------------------------------------------------------------------------*/
pipeinfo pipeOut= {INVALID_HANDLE_VALUE, NULL,0};
pipeinfo pipeErr= {INVALID_HANDLE_VALUE, NULL,0};
/*--------------------------------------------------------------------------*/
static int PrintOuput(char **ouput,int nbrlines);
/*--------------------------------------------------------------------------*/
int sci_dos(char *fname,unsigned long l)
{
	int m1,n1,l1;
	int *Status=NULL;
	char *Param1String=NULL;
	char *Param2String=NULL;
	BOOL ECHOMODE=FALSE;
	BOOL DetachProcessOption=FALSE;

	char **Output=NULL;
	int numberoflines=0;

	CheckRhs(1,2);
	CheckLhs(1,2);

	if (GetType(1)!=sci_strings) 
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
		return 0;
	}

	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	Param1String=cstk(l1);
	DetachProcessOption=DetectDetachProcessInCommandLine(Param1String);

	if (Rhs == 2)
	{
		if (GetType(2)!=sci_strings) 
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n") ,fname,2);
			return 0;
		}
		GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
		Param2String=cstk(l1);

		if (strcmp("-echo",Param2String))
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: '%s' expected.\n"),fname,2,"echo");
			return 0;
		}
		else
		{
			ECHOMODE=TRUE;
		}
	}

	spawncommand(Param1String,DetachProcessOption);

	Status=(int*)MALLOC(sizeof(int));

	if (DetachProcessOption)
	{
		if ( strlen(pipeErr.OutputBuffer) )
		{
			/* StdErr will be "Output" */
			*Status=FALSE;
			Output=CreateOuput(&pipeErr,DetachProcessOption);
			numberoflines=pipeErr.NumberOfLines;
		}
		else
		{
			/* StdOut will be "Output" */
			*Status=TRUE;
			Output=CreateOuput(&pipeOut,DetachProcessOption);
			numberoflines=pipeOut.NumberOfLines;
		}
	}
	else
	{
		char *TMPDir=NULL;
		char FileTMPDir[PATH_MAX];

		TMPDir=getTMPDIR();
		sprintf(FileTMPDir,"%s\\DOS.OK",TMPDir);
		if (TMPDir)
		{
			FREE(TMPDir);
			TMPDir=NULL;
		}

		if (FileExist(FileTMPDir))
		{
			DeleteFile(FileTMPDir);
			/* StdOut will be "Output" */
			*Status=TRUE;
			Output=CreateOuput(&pipeOut,DetachProcessOption);
			numberoflines=pipeOut.NumberOfLines;
		}
		else
		{
			/* StdErr will be "Output" */
			*Status=FALSE;
			Output=CreateOuput(&pipeErr,DetachProcessOption);
			numberoflines=pipeErr.NumberOfLines;
		}
	}

	if (ECHOMODE) PrintOuput(Output,numberoflines);

	if (Lhs == 1)
	{
		m1=1;n1=1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
		LhsVar(1)=Rhs+1;
	}
	else /* Lhs == 2 */
	{
		if (Output[0])
		{
			m1=numberoflines;
			n1=1;
			CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1, &n1, Output);
		}
		else
		{
			m1=0;
			n1=0;
			l1=0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
		}
		
		LhsVar(1)=Rhs+1;

		m1=1;n1=1;
		CreateVarFromPtr(Rhs+2,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
		LhsVar(2)=Rhs+2;
	}

	C2F(putlhsvar)();

	if (Status) {FREE(Status);Status=NULL;}
	if (Output)
	{
		int i=0;
		for(i=0;i<numberoflines;i++) if (Output[i]) {FREE(Output[i]);Output[i]=NULL;}
		FREE(Output);
		Output=NULL;
	}

	ClosePipeInfo (pipeOut);
	ClosePipeInfo (pipeErr);

	return 0;
}
/*--------------------------------------------------------------------------*/
static int PrintOuput(char **ouput,int nbrlines)
{
	if (ouput)
	{
		int i=0;
		for(i=0;i<nbrlines;i++) 
		{
			if (ouput[i]) sciprint("%s\n",ouput[i]);
		}
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
