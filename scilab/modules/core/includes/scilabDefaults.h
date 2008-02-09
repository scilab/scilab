/*
 * This file contains default paths for Scilab
 * @copyright INRIA 2007
 * @author Sylvestre LEDRU 
 */
#ifndef __SCILABDEFAULT_H__
#define __SCILABDEFAULT_H__
#include "machine.h"

/* Where is stored the history by default */
#ifdef _MSC_VER
#define DEFAULT_HISTORY_FILE "history.scilab"
#else
#define DEFAULT_HISTORY_FILE ".history.scilab"
#endif

/* Where is the modelicac compiler 
 * @TODO : update for Linux ... it is a wrong path */
#ifdef _MSC_VER
#define ModelicacName "/bin/modelicac.exe"
#else
#define ModelicacName "/modules/scicos/bin/modelicac"
#endif

/* Where the module list file is */
#define basenamemodulesfile "etc/modules.xml" 

/* Where the classpath list file is */
#define XMLCLASSPATH "%s/etc/classpath.xml"

/* Where the library list file is */
#define XMLLIBRARYPATH "%s/etc/librarypath.xml"

/* Where the system properties are */
#define XMLSYSTEMPROPERTIES "%s/modules/jvm/etc/jvm-property.xml"

/* Name of the localisation XML file containing the errors message strings */
#define FILELOCALIZATIONERROR "errors"

/* Name of the localisation XML file containing the message strings */
#define FILELOCALIZATIONMSGS "messages"

/* Name of the localisation XML file containing the menu strings */
#define FILELOCALIZATIONMENUS "menus"

/* Where the localizations files can be found */
#define FILELOCALIZATIONFORMATPATH "%s/modules/%s/locales/%s/%s.xml"

/* Where is the gateway file of a module */
#define FORMATGATEWAYFILENAME "%s/modules/%s/sci_gateway/%s_gateway.xml"

/* Where is the version file of a module */
#define FORMATVERSIONFILENAME  "%s/modules/%s/version.xml" 


/* Where is the Scilab startup script */
#define DEFAULTSCILABSTARTUP "SCI/etc/scilab.start"

/* What is the default Scilab StackSize ? */
#define DEFAULTSTACKSIZE 1000000

//#define DefaultScilabQuit "SCI/etc/scilab.quit"
//static char DefaultSCIenv[]="../..";

/* What is the variable to export for the language */
#define EXPORTENVLOCALE "LC_ALL"

/* Name of the main localization domain */
#define NAMELOCALIZATIONDOMAIN "scilab"

/* Path the locale files */
#define PATHLOCALIZATIONFILE "/locale/"

#endif /*__SCILABDEFAULT_H__ */
