/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------------------*/
#ifndef __DYNAMIC_LINK_H__
#define __DYNAMIC_LINK_H__

#include "dynlib_dynamic_link.h"
#include "BOOL.h"
#include "machine.h"            /* C2F */

#define ENTRYMAX 500

/**
* Initialize tables 
*/
DYNAMIC_LINK_IMPEXP void initializeLink(void);

/**
* check that a routine name is a loaded
* if *ilib == -1 
*    checks if routinename is a loaded
*    entry point 
*    the result is -1 if false 
*    or the number in the function table 
* 
* @param[in] routinename
* @param[in/out] ilib number in the function table (-1 if FALSE)
* @return a BOOL
*/
DYNAMIC_LINK_IMPEXP BOOL c_link(char *routinename, int *ilib);

/**
* OBSOLETE 
* if *ilib == -1 
*    checks if routinename is a loaded
*    entry point 
*    the result is -1 if false 
*               or the number in the function table 
* @param[in] routinename
* @param[in/out] ilib
*/
DYNAMIC_LINK_IMPEXP void C2F(iislink) (char *routinename, int *ilib);

/**
* returns the ii functions 
* @param ii
* @param ptr on functions
*/
DYNAMIC_LINK_IMPEXP void GetDynFunc(int ii, void (**realop) ());

/**
* Search a function in the table 
* Search from end to top 
* @param 
* @param 
*/
DYNAMIC_LINK_IMPEXP int SearchInDynLinks(char *op, void (**realop) ());

/**
* Show the linked files 
*/
DYNAMIC_LINK_IMPEXP void ShowDynLinks(void);

/**
* unlink all linked files 
*/
DYNAMIC_LINK_IMPEXP void unlinkallsharedlib(void);

/**
* Unlink a shared lib 
* @param i (number of shared lib)
*/
DYNAMIC_LINK_IMPEXP void unlinksharedlib(int *i);

/**
* load a shared archive and call LoadDynLibrary
* the shared lib handler is stored in a Table 
* return value is == -1 if the LoadDynLibrary failed 
* @param loaded_file
*/
DYNAMIC_LINK_IMPEXP int Sci_dlopen(char *loaded_file);

/**
* This routine load the entryname ename 
* from shared lib ishared 
* @return TRUE or FALSE
*/
DYNAMIC_LINK_IMPEXP int Sci_dlsym(char *ename, int ishared, char *strf);

/**
* Delete entry points associated with shared lib ishared
* then delete the shared lib 
@param ishared
*/
DYNAMIC_LINK_IMPEXP void Sci_Delsym(int ishared);

/**
* @param sizearray returns size of string array returned
* @return string array with functions names loaded
*/
DYNAMIC_LINK_IMPEXP char **getNamesOfFunctionsInSharedLibraries(int *sizearray);

/**
* call link for scilab
* @param idsharedlibrary
* @param filename (dynamic library name)
* @param subnamesarray (list of functions name in dynamic library)
* @param sizesubnamesarray
* @param fflag
* @param ierr (last error)
* @return id 
*/
DYNAMIC_LINK_IMPEXP int scilabLink(int idsharedlibrary, char *filename, char **subnamesarray, int sizesubnamesarray, BOOL fflag, int *ierr);

/**
* get list of all Id of shared lib
* @param size of returned list
* @return list of Id
*/
DYNAMIC_LINK_IMPEXP int *getAllIdSharedLib(int *sizeList);

#endif /* __DYNAMIC_LINK_H__ */
/*-----------------------------------------------------------------------------------*/
