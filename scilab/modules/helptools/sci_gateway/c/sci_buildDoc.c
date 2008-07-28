/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA/DIGITEO - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "gw_helptools.h"
#include "Scierror.h"
#include "stack-c.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
int sci_buildDoc(char *fname,unsigned long l)
{
	static int l1,n1,m1;
	static int l2,n2,m2;
	char *exportFormat=NULL;
	
	CheckRhs(1,2);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *sourceDoc="/home/sylvestre/dev/scilab5/modules/helptools/master_en_US_help.xml";
		org_scilab_modules_helptools::BuildDocObject *doc = new org_scilab_modules_helptools::BuildDocObject(getScilabJavaVM());
		doc->process(sourceDoc, styleDoc, outputDirectory, styleSheet);

		//		buildDoc(,"/usr/share/xml/docbook/stylesheet/nwalsh/javahelp/javahelp.xsl","/tmp/","/home/sylvestre/dev/scilab5/modules/helptools/css/javahelp.css");
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		exportFormat=cstk(l1);

		/*		if (getTclInterp() == NULL)
		{
		 releaseTclInterp();
		 Scierror(999,_("%s: Error main TCL interpreter not initialized.\n"),fname);
			return 0;
		}
		releaseTclInterp();
		*/
		if (Rhs==2)
		{
			/* two arguments given - get a pointer on the slave interpreter */
			if (GetType(2) == sci_strings)
			{
				GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
				/*				TCLinterpreter=Tcl_GetSlave(getTclInterp(),cstk(l2));
				releaseTclInterp();
				if (TCLinterpreter==NULL)
				{
					Scierror(999,_("%s: No such slave interpreter.\n"),fname);
					return 0;
					}*/
			}
			else
			{
				Scierror(999,_("%s: Wrong input argument: String expected.\n"),fname);
				return 0;
			}
		}
		else
		{
			/* only one argument given - use the main interpreter */
			/*			TCLinterpreter=getTclInterp();
						releaseTclInterp();*/
		}

		n1=1;
		/*		if ( Tcl_GetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY) )
				{*/
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
			/*		}
		else
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
			}*/

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong input argument: String expected.\n"),fname);
		return 0;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
