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
#include <stdio.h>
#include "gw_sound.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static int BeepON=1;
/*--------------------------------------------------------------------------*/
void BeepLinuxWindows(void);
/*--------------------------------------------------------------------------*/
/* SCILAB function : beep */
/*--------------------------------------------------------------------------*/
int sci_Beep(char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;
	char *output=NULL ;

	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 1)
	{
		char *param1=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		if ( strcmp(param1,"on") == 0 )
		{
			BeepON=1;
		}
		else
		{
			if ( strcmp(param1,"off") == 0 )
			{
				BeepON=0;
			}
			else Scierror(999,_("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"),fname,1,"on","off");
		}
	}
	else
	{
		if (BeepON) BeepLinuxWindows();
	}

	output=(char*)MALLOC(6*sizeof(char));
	n1=1;

	if (BeepON) strcpy(output,"on");
	else strcpy(output,"off");

	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
	FREE(output);
	output=NULL;

	LhsVar(1) = Rhs+1;

	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
void BeepLinuxWindows(void)
{
	#ifdef _MSC_VER
		MessageBeep((UINT)-1);
	#else
		puts("\a");
	#endif
}
/*--------------------------------------------------------------------------*/
