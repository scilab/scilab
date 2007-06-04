/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "InitializeJVM.h"
#include "loadClasspath.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "JVM.h"
#include "createMainScilabObject.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeJVM(void)
{
	BOOL bOK=FALSE;
	char *SCIPATH=NULL;

	SCIPATH=getSCIpath();

	bOK=startJVM(SCIPATH);

	if (!bOK)
	{
#ifdef _MSC_VER
		MessageBox(NULL,"Scilab cannot open JVM library.","Error",MB_ICONEXCLAMATION|MB_OK);
#else
		printf("\nError : Scilab cannot open JVM library.\n");
#endif
	}
	else

	{
		#define XMLCLASSPATH "%s/etc/classpath.xml"
		char *classpathfile = NULL;
		classpathfile = (char*)MALLOC(sizeof(char)*(strlen(SCIPATH)+strlen(XMLCLASSPATH)+1));
		sprintf(classpathfile,XMLCLASSPATH,SCIPATH);
		LoadClasspath(classpathfile);
		if (classpathfile) {FREE(classpathfile); classpathfile = NULL;}

		bOK = createMainScilabObject();

		if (!bOK)
		{
#ifdef _MSC_VER
			MessageBox(NULL,"Scilab cannot create Scilab Java Main-Class. (we have not been able to find the main Scilab class. Check if the Scilab and thirdparty packages are available).","Error",MB_ICONEXCLAMATION|MB_OK);
#else
			printf("\nError : Scilab cannot create Scilab Java Main-Class. (we have not been able to find the main Scilab class. Check if the Scilab and thirdparty packages are available).\n");
#endif
		}
	}

	if (SCIPATH) {FREE(SCIPATH);SCIPATH=NULL;}

	if (!bOK) exit(1);

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

