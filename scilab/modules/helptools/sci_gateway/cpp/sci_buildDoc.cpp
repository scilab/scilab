/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "BuildDocObject.hxx"

extern "C"
{
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "stack-c.h"
#include "gw_helptools.h"
#include "Scierror.h"
#include "setgetSCIpath.h"
#include "localization.h"
#include "setgetlanguage.h"
#include "getScilabJavaVM.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "ConvertSlash.h"
#endif
/*--------------------------------------------------------------------------*/
#define PATHTOCSS "/modules/helptools/css/javahelp.css"
#define PATHTOBUILDDOC "/modules/helptools/build/doc/scilab_%s_help/"
#define PATHTOMASTERXML "/modules/helptools/master_%s_help.xml"
#define DEFAULTEXPORT "JH"
/*--------------------------------------------------------------------------*/
int sci_buildDoc(char *fname,unsigned long l)
{
	static int l1 = 0,n1 = 0,m1 = 0;
	static int l2 = 0,n2 = 0,m2 = 0;
	char * exportFormat = NULL;
	char * SciPath = getSCIpath(); /* Scilab path */
	char * masterXML = NULL; /* Which file contains all the doc stuff */
	char * masterXMLTMP = NULL;
	char * outputDirectory = NULL; /* Working directory */
	char * outputDirectoryTMP = NULL;
	char * currentLanguage = getlanguage();
	char * styleSheet = (char*)MALLOC((strlen(SciPath)+strlen(PATHTOCSS)+1)*sizeof(char));; /* the CSS */

	if ( styleSheet == NULL )
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
		return 0;
	}

	CheckRhs(0,2);
	CheckLhs(1,1);

	strcpy(styleSheet,SciPath);
	strcat(styleSheet,PATHTOCSS);

	/* Update the path with the localization */
	outputDirectoryTMP = (char*) MALLOC ((strlen(PATHTOBUILDDOC)-strlen("%s")+strlen(currentLanguage)+1)*sizeof(char)); /* - strlen(%s) because it is going to be be replaced by currentLanguage */
	if ( outputDirectoryTMP == NULL )
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
		return 0;
	}
	sprintf(outputDirectoryTMP, PATHTOBUILDDOC, currentLanguage);

	outputDirectory = (char*)MALLOC((strlen(SciPath)+strlen(outputDirectoryTMP)+1)*sizeof(char)); 
	if ( outputDirectory == NULL )
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
		return 0;
	}

	strcpy(outputDirectory, SciPath);
	strcat(outputDirectory, outputDirectoryTMP);
	FREE(outputDirectoryTMP);


	if (Rhs < 1) 
	{
		exportFormat = (char*)MALLOC((strlen(DEFAULTEXPORT)+1)*sizeof(char));
		if ( exportFormat == NULL )
		{
			Scierror(999,_("%s: No more memory.\n"),fname);
			return 0;
		}

		strcpy(exportFormat,DEFAULTEXPORT);
	}
	else
	{
		if (GetType(1) != sci_strings) 
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
			return 0;
			// Wrong type string
		}

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		exportFormat = cstk(l1);
	}


	if (Rhs < 2) 
	{
		/* Update the path with the localization */
		masterXMLTMP = (char*) MALLOC ((strlen(PATHTOMASTERXML)-strlen("%s")+strlen(currentLanguage)+1)*sizeof(char));
		if ( masterXMLTMP == NULL )
		{
			Scierror(999,_("%s: No more memory.\n"),fname);
			return 0;
		}
		
		sprintf(masterXMLTMP, PATHTOMASTERXML, currentLanguage);

		masterXML = (char*)MALLOC((strlen(SciPath)+strlen(masterXMLTMP)+1)*sizeof(char));
		if ( masterXML == NULL )
		{
			Scierror(999,_("%s: No more memory.\n"),fname);
			return 0;
		}
		strcpy(masterXML,SciPath);
		strcat(masterXML,masterXMLTMP);
		FREE(masterXMLTMP);
	}
	else 
	{
		if (GetType(2) != sci_strings)
		{
			// Wrong type string
			Scierror(999,_("%s: Wrong for type for input argument #%d: String expected.\n"),fname,2);
			return 0;
		}

		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		masterXML = cstk(l2);
	}

	org_scilab_modules_helptools::BuildDocObject *doc = new org_scilab_modules_helptools::BuildDocObject(getScilabJavaVM());

	#ifdef _MSC_VER
	slashToAntislash(outputDirectory,outputDirectory);
	slashToAntislash(styleSheet,styleSheet);
	slashToAntislash(masterXML,masterXML);
	#endif

	if (doc->setOutputDirectory(outputDirectory)) 
	{
		doc->setWorkingLanguage(currentLanguage);
		doc->setExportFormat(exportFormat);
		doc->process(masterXML, styleSheet);
		delete doc;
		FREE(masterXML); masterXML = NULL;
		FREE(outputDirectory); outputDirectory = NULL;
		FREE(styleSheet); styleSheet = NULL;
	}
	else
	{
		Scierror(999,_("%s: Could find or create the working directory %s.\n"),fname, outputDirectory);
		delete doc;
		FREE(masterXML); masterXML = NULL;
		FREE(outputDirectory); outputDirectory = NULL;
		FREE(styleSheet); styleSheet = NULL;
		return 0;
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
}
