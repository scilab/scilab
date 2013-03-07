/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) ENPC
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*
 * This file contains default paths for Scilab
 */
#ifndef __SCILABDEFAULT_H__
#define __SCILABDEFAULT_H__

/* Where is stored the history by default */
#define DEFAULT_HISTORY_FILE "history"

#ifdef _MSC_VER
#define ModelicacName "modelicac.exe"
#else
#define ModelicacName "modelicac"
#endif

/* Where the module list file is */
#define basenamemodulesfile "etc/modules.xml"

/* Where the classpath list file is */
#define XMLCLASSPATH "%s/etc/classpath.xml"

/* Where the library list file is */
#define XMLLIBRARYPATH "%s/etc/librarypath.xml"

/* Where the system properties are */
#define XMLSYSTEMPROPERTIES "%s/modules/jvm/etc/jvm-property.xml"

/* Where is the gateway file of a module */
#define FORMATGATEWAYFILENAME "%s/modules/%s/sci_gateway/%s_gateway.xml"

/* Where is the version file of a module */
#define FORMATVERSIONFILENAME  "%s/modules/%s/version.xml"


/* Where is the Scilab startup script */
#define DEFAULTSCILABSTARTUP "SCI/etc/scilab.start"

/* What is the default Scilab StackSize ? */
#define DEFAULTSTACKSIZE 1000000

/* default Scilab global StackSize  */
#if defined(_MSC_VER) && defined(_WIN64)
/* On Windows x64 initial global stacksize must be bigger :( */
#define DEFAULTGSTACKSIZE DEFAULTSTACKSIZE
#else
#define DEFAULTGSTACKSIZE 1000
#endif


//#define DefaultScilabQuit "SCI/etc/scilab.quit"
//static char DefaultSCIenv[]="../..";

#ifndef _MSC_VER
/* What is the variable to export for the language */
#define EXPORTENVLOCALE LC_MESSAGES
#else
/* MS VS (setlocale) doesn't know LC_MESSAGES */
/* http://msdn2.microsoft.com/en-us/library/x99tb11d(vs.71).aspx */
#define EXPORTENVLOCALE LC_CTYPE
#endif
#define EXPORTENVLOCALESTR "LC_MESSAGES"

/* The portable locale. Should be english for numerical values */
#define LCNUMERICVALUE "C"

/* Name of the main localization domain */
#define NAMELOCALIZATIONDOMAIN "scilab"

/* Path the locale files */
#define PATHLOCALIZATIONFILE "/locale/"

#endif /*__SCILABDEFAULT_H__ */
