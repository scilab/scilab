/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __URIFILETOFILENAME_H__
#define __URIFILETOFILENAME_H__

#include "machine.h"
/**
* convert URI File (file://path/filename.ext) to standard filename
* @param[in] uri : a string URI file
* @return a filename
*/
char *URIFileToFilename(char *uri);

/**
* check if it is a URIFile
* @param [in] uri : a string URI file
* @return TRUE or FALSE;
*/
BOOL isURIFile(char *uri);

#endif /* __URIFILETOFILENAME_H__ */
/*-----------------------------------------------------------------------------------*/
