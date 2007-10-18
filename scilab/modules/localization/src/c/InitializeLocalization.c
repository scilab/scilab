/** 
* INRIA 2007 
* Rewrite using gettext by Sylvestre Ledru <sylvestre.ledru@inria.fr>
*/
#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "InitializeLocalization.h"
#include "localization.h"
#include "setgetSCIpath.h"
#include "inisci-c.h"
#include "machine.h"
#include "scilabDefaults.h"
#ifdef _MSC_VER
#define _putenv putenv
#endif
/*-----------------------------------------------------------------------------------*/ 

/**
 * Export the variable LC_ALL to the system
 *
 * @param locale the locale (ex : fr_FR or en_US)
 */
static void putEnvLC_ALL(char *locale){

	char *localeDeclared=NULL;	

	localeDeclared=(char*)MALLOC(sizeof(char)*strlen(EXPORTENVLOCALE)+ strlen("=")+ strlen(locale)+1);
	strcat(localeDeclared,EXPORTENVLOCALE);
	strcat(localeDeclared,"=");
	strcat(localeDeclared,locale);
	/* It will put in the env something like LC_ALL=fr_FR */

	if (putenv (localeDeclared)){
		fprintf(stderr,"Failed to declare the system variable LC_ALL\n");
	}
	FREE(localeDeclared);
}


BOOL InitializeLocalization(void)
{
#ifdef HAVE_LIBINTL_H

	char *pathLocales=getSCIpath();

	char *ret=NULL;
	ret=setlocale(LC_ALL,"");
    if (ret==NULL){
   		fprintf(stderr, "I18N: Doesn't support your locale.\n" );
		return FALSE;
	}
	putEnvLC_ALL(ret);

	strcat(pathLocales, PATHLOCALIZATIONFILE);

	if (bindtextdomain(NAMELOCALIZATIONDOMAIN,pathLocales)==NULL){
		fprintf(stderr, "Error while binding the domain\n");
		return FALSE;
	}

	if (textdomain(NAMELOCALIZATIONDOMAIN)==NULL){
		fprintf(stderr, "Error while declaring the text domain\n");
		return FALSE;
	}

	return TRUE;
#else
	fprintf(stderr, "setlocale didn't exist on the computer used to compile Scilab ! This is abnormal ! No localization will be working for this distribution of Scilab.\n");
	return FALSE;
#endif
}
/*-----------------------------------------------------------------------------------*/ 

