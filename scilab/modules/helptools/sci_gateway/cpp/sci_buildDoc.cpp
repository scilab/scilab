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
#include "GiwsException.hxx"

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
#ifdef _MSC_VER
static void __slashToAntislash(std::string *in)
{
  size_t found = in->rfind("/");

  while (found != std::string::npos)
    {
      in->replace (found, 1, "\\");
      found = in->rfind("/");
    }
}
#endif
/*--------------------------------------------------------------------------*/
  int sci_buildDoc(char *fname,unsigned long l)
  {
    static int l1 = 0,n1 = 0,m1 = 0;
    static int l2 = 0,n2 = 0,m2 = 0;
    static int l3 = 0,n3 = 0,m3 = 0;
    static int l4 = 0,n4 = 0,m4 = 0;
	//	static int Row_Output = 0, Col_Output = 0;
    std::string exportFormat;
    std::string SciPath = getSCIpath(); /* Scilab path */
    std::string masterXML; /* Which file contains all the doc stuff */
    std::string masterXMLTMP;
    std::string outputDirectory; /* Working directory */
    std::string outputDirectoryTMP;
    std::string language;
    std::string styleSheet; /* the CSS */
	//	std::string pathToGenerated;
    org_scilab_modules_helptools::BuildDocObject *doc = NULL;

    CheckRhs(0,4);
    CheckLhs(1,1);

    styleSheet = SciPath+PATHTOCSS;

    if (Rhs < 1)
      {
	exportFormat = DEFAULTEXPORT;
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

    if ( Rhs < 3) /* Language not provided */
      {
		  language = getlanguage();
      }
    else
      {
	GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
        if (m3*n3 == 0)
          {
            language = getlanguage();
          }
        else
          {
            language = std::string(cstk(l3));
          }
      }

    if (Rhs < 2)
      {
		  /* Update the path with the localization */
		  masterXMLTMP = std::string("/modules/helptools/master_")+language+std::string("_help.xml");
		  masterXML = SciPath + masterXMLTMP;
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

    if (Rhs == 4)
      {
        if (GetType(4) == sci_strings)
          {
            GetRhsVar(4,STRING_DATATYPE,&m4,&n4,&l4);
            outputDirectory = std::string(cstk(l4))+std::string("/scilab_")+language+std::string("_help/");
          }
        else
          {
			  Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,4);
			  return FALSE;
          }
      }
    else /* Scilab help */
      {
        /* Update the path with the localization */
        outputDirectoryTMP = std::string("/modules/helptools/")+std::string(exportFormat)+std::string("/scilab_")+language+std::string("_help/");

        outputDirectory = SciPath+outputDirectoryTMP;
      }

    try
      {
	doc = new org_scilab_modules_helptools::BuildDocObject(getScilabJavaVM());

#ifdef _MSC_VER
	__slashToAntislash(&outputDirectory);
	__slashToAntislash(&styleSheet);
	__slashToAntislash(&masterXML);
#endif


	if (doc->setOutputDirectory((char *) outputDirectory.c_str()))
	  {
	    doc->setWorkingLanguage((char *) language.c_str());
	    doc->setExportFormat((char *) exportFormat.c_str());

		doc->process((char *) masterXML.c_str(), (char *) styleSheet.c_str());
		/*		Row_Output = 1;
		Col_Output = 1;
		CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &Row_Output, &Col_Output, &pathToGenerated.c_str() );
		*/

	  }
	else
	  {
		  Scierror(999,_("%s: Could find or create the working directory %s.\n"), fname, outputDirectory.c_str());
		  return FALSE;
	  }
      }
    catch(GiwsException::JniException ex)
      {
		  Scierror(999,_("%s: Error while building documentation: %s.\n"), fname, ex.getJavaDescription().c_str());
		  return FALSE;
      }

    if (doc != NULL) delete doc;
	LhsVar(1) = 0 ;
    C2F(putlhsvar)();
    return 0;
  }
  /*--------------------------------------------------------------------------*/
}
