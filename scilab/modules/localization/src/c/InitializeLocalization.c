/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdlib.h>
#include "machine.h"

#ifndef _MSC_VER
 #ifdef HAVE_LIBINTL_H
  #include <libintl.h>
  #ifdef HAVE_LOCALE_H
   #include <locale.h>
  #else
   #error "Cannot find locale.h despite that libintl.h is available"
  #endif
 #endif
#endif

#include "MALLOC.h"
#include "InitializeLocalization.h"
#include "localization.h"
#include "setgetSCIpath.h"
#include "inisci-c.h"
#include "scilabDefaults.h"
#include "setgetlanguage.h"

/*--------------------------------------------------------------------------*/ 

BOOL InitializeLocalization(void)
{
#ifdef HAVE_LIBINTL_H

	char *SCIpath=getSCIpath();
	char *pathLocales=NULL;
	char *ret=NULL;

	/* set directory containing message catalogs */
	pathLocales=(char *)MALLOC(sizeof(char)*(strlen(SCIpath)+strlen(PATHLOCALIZATIONFILE)+1));

	strcpy(pathLocales, SCIpath);
	strcat(pathLocales, PATHLOCALIZATIONFILE);

	if (bindtextdomain(NAMELOCALIZATIONDOMAIN,pathLocales)==NULL){
		fprintf(stderr, "Localization: Error while binding the domain from %s\n", pathLocales);
		FREE(pathLocales);
		return FALSE;
	}
	FREE(pathLocales);

	/* set domain for future gettext() calls */
	ret=textdomain(NAMELOCALIZATIONDOMAIN);
	if (ret==NULL){
		fprintf(stderr, "Localization: Error while declaring the text domain %s\n", NAMELOCALIZATIONDOMAIN);
		return FALSE;
	}

	/* Here, the "" means that we will try to use the language of the system
	 * first. If it doesn't work, we switch back to default (English) */
	setlanguage("", FALSE, FALSE); /* Booleans are : BOOL updateHelpIndex, BOOL updateMenus */

	return TRUE;
#else
	fprintf(stderr, "Localization: setlocale didn't exist on the computer used to compile Scilab ! This is abnormal ! No localization will be working for this distribution of Scilab.\n");
	return FALSE;
#endif
}
/*--------------------------------------------------------------------------*/ 

