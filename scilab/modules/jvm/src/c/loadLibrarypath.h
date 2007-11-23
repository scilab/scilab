/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __LOADLIBRARYPATH_H__
#define __LOADLIBRARYPATH_H__

#include "machine.h" /* BOOL */

/**
* add paths to java.library.path
* @param a filename SCI/etc/librarypath.xml
* @return TRUE or FALSE
*/
BOOL LoadLibrarypath(char *xmlfilename);

#endif /* __LOADLIBRARYPATH_H__ */
/*--------------------------------------------------------------------------*/ 
