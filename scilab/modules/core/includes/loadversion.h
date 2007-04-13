/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"

#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

/*-----------------------------------------------------------------------------------*/ 
/**
 * Retrieve the version of the module from the XML description file 
 *
 * @param[in] modulename name of the module
 * @param[out] sci_version_major 
 * @param[out] sci_version_minor 
 * @param[out] sci_version_maintenance   
 * @param[out] sci_version_string    
 * @param[out] sci_version_revision  
 * @return if the function has been able to load the informations
 */
BOOL getversionmodule(char *modulename,
					  int *sci_version_major,
					  int *sci_version_minor,
					  int *sci_version_maintenance,
					  char *sci_version_string,
					  int *sci_version_revision);
/*-----------------------------------------------------------------------------------*/ 
