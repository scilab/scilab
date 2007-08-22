/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __LIBRARYINFO_H__
#define __LIBRARYINFO_H__

/**
* get path of scilab library
* @param[in] name of scilab library
* @return a string : path of library
*/
char *getlibrarypath(char *libraryname);

/**
* get macros list in a scilab's library
* @param[in] name of scilab library
* @param[out] size of array returned
* @return a array of string (macros list)
*/
char **getlistmacrosfromlibrary(char *libraryname,int *sizearray);

#endif /* __LIBRARYINFO_H__ */
/*-----------------------------------------------------------------------------------*/
