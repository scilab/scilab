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
#include "setgetlanguage.h"
#include "../../../io/includes/setenvc.h"
/*-----------------------------------------------------------------------------------*/ 

/**
 * Export the variable LC_ALL to the system
 *
 * @param locale the locale (ex : fr_FR or en_US)
 */
static void putEnvLC_ALL(char *locale){

	/* It will put in the env something like LC_ALL=fr_FR */
	if ( !setenvc(EXPORTENVLOCALE,locale))
	{
		fprintf(stderr,"Failed to declare the system variable LC_ALL\n");
	}
}


BOOL InitializeLocalization(void)
{
#ifdef HAVE_LIBINTL_H

	char *SCIpath=getSCIpath();
	char *pathLocales=NULL;
	char *ret=NULL;

	ret=setlocale(LC_MESSAGES,"");
	if (ret==NULL){
   		fprintf(stderr, "I18N: Doesn't support your locale.\n" );
		return FALSE;
	}

	putEnvLC_ALL(ret);
	setlanguage(ret, FALSE, FALSE);

	pathLocales=(char *)MALLOC(sizeof(char)*(strlen(SCIpath)+strlen(PATHLOCALIZATIONFILE)+1));

	strcpy(pathLocales, SCIpath);
	strcat(pathLocales, PATHLOCALIZATIONFILE);

	if (bindtextdomain(NAMELOCALIZATIONDOMAIN,pathLocales)==NULL){
		fprintf(stderr, "Error while binding the domain from %s\n", pathLocales);
		FREE(pathLocales);
		return FALSE;
	}
	FREE(pathLocales);

	if (textdomain(NAMELOCALIZATIONDOMAIN)==NULL){
		fprintf(stderr, "Error while declaring the text domain %s\n", NAMELOCALIZATIONDOMAIN);
		return FALSE;
	}

	return TRUE;
#else
	fprintf(stderr, "setlocale didn't exist on the computer used to compile Scilab ! This is abnormal ! No localization will be working for this distribution of Scilab.\n");
	return FALSE;
#endif
}
/*-----------------------------------------------------------------------------------*/ 

