/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "getJvmOptions.h"
#include "GetXmlFileEncoding.h"
#include "MALLOC.h"
#include "localization.h"
#include "machine.h"
#include "stricmp.h"
#include "FileExist.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "strsubst.h"
#include "getos.h"
#include "getshortpathname.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
JavaVMOption * getJvmOptions(char *SCI_PATH,char *filename_xml_conf,int *size_JavaVMOption)
{
	if ( FileExist(filename_xml_conf) )
	{
		JavaVMOption *jvm_options = NULL;
		char *encoding = GetXmlFileEncoding(filename_xml_conf);

		/* Don't care about line return / empty line */
		xmlKeepBlanksDefault(0);
		/* check if the XML file has been encoded with utf8 (unicode) or not */
		if (stricmp("utf-8", encoding)==0)
		{
			xmlDocPtr doc;
			xmlXPathContextPtr xpathCtxt = NULL;
			xmlXPathObjectPtr xpathObj = NULL;
			char *jvm_option_string = NULL;

			int indice = 0;
			{
				BOOL bConvert = FALSE;
				char *shortfilename_xml_conf = getshortpathname(filename_xml_conf,&bConvert);
				if (shortfilename_xml_conf)
				{
					doc = xmlParseFile (shortfilename_xml_conf);
					FREE(shortfilename_xml_conf);
					shortfilename_xml_conf = NULL;
				}
			}

			if (doc == NULL)
			{
				fprintf(stderr,_("Error: Could not parse file %s.\n"), filename_xml_conf);
				if (encoding) {FREE(encoding);encoding=NULL;}
				*size_JavaVMOption = 0;
				return NULL;
			}

			xpathCtxt = xmlXPathNewContext(doc);
			xpathObj = xmlXPathEval((const xmlChar*)"//jvm_options/option | //jvm_options/option[@os='OSNAME']", xpathCtxt);

			if(xpathObj && xpathObj->nodesetval->nodeMax)
			{
				/* the Xpath has been understood and there are node */
				int	i;
				for(i = 0; i < xpathObj->nodesetval->nodeNr; i++)
				{

					xmlAttrPtr attrib=xpathObj->nodesetval->nodeTab[i]->properties;
					/* Get the properties of <option>  */
					while (attrib != NULL)
					{
						/* loop until when have read all the attributes */
						if (xmlStrEqual (attrib->name, (const xmlChar*) "value"))
						{
							/* we found the tag name */
							const char *str=(const char*)attrib->children->content;
							jvm_option_string = strdup(str);
						}
						attrib = attrib->next;
					}

					if ( (jvm_option_string) && (strlen(jvm_option_string) > 0) )
					{
						char *option_string_path_separator = NULL;
						char *option_string_sci_path = NULL;

						option_string_path_separator = strsub(jvm_option_string,"$PATH_SEPARATOR",PATH_SEPARATOR);
						if (jvm_option_string)
						{
							FREE(jvm_option_string);
						}

						option_string_sci_path = strsub(option_string_path_separator,"$SCILAB",SCI_PATH);
						if (option_string_sci_path)
						{
							FREE(option_string_path_separator);
						}

						jvm_options = (JavaVMOption *)REALLOC(jvm_options,sizeof(JavaVMOption)*(indice+1));
						jvm_options[indice].optionString = option_string_sci_path;
						indice++;
					}
				}
			}

			if(xpathObj) xmlXPathFreeObject(xpathObj);
			if(xpathCtxt) xmlXPathFreeContext(xpathCtxt);
			xmlFreeDoc (doc);

			/* xmlCleanupParser is called in
			 * modules/core/src/c/TerminateCore.c 
			 * since it needs to be done only once.
			 */

			if (getenv("SCI_JAVA_ENABLE_HEADLESS")!=NULL) {
				/* When Scilab is built from a virtual machine, it needs
				 * an X11 server / input
				 * This is only called by "make doc" by the SCI/Makefile.am
				 */
				#define HEADLESS "-Djava.awt.headless=true"
				jvm_options = (JavaVMOption *)REALLOC(jvm_options,sizeof(JavaVMOption)*(indice+1));
				jvm_options[indice].optionString = MALLOC((strlen(HEADLESS)+1)*sizeof(char));
				strcpy(jvm_options[indice].optionString,HEADLESS);
				indice++;
				#undef HEADLESS
			}

			if (encoding) {FREE(encoding);encoding=NULL;}

			*size_JavaVMOption = indice;
			return jvm_options;
		}
		else
		{
			fprintf(stderr,_("Error: Not a valid configuration file %s (encoding not '%s') Encoding '%s' found.\n"), filename_xml_conf, "utf-8", encoding);
		}
		if (encoding) {FREE(encoding);encoding=NULL;}
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
