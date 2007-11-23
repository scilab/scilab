/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __FINDFILES_H__
#define __FINDFILES_H__

/** 
* Finding all files in a directory with a given filespec
* @param path example : "c:\"
* @param filespec example : "*.sce"
* @param[out] sizeListReturned the size of the files list
* @return all the files except '.' & '..'
*/

char **findfiles(char *path,char *filespec,int *sizeListReturned);

#endif /* __FINDFILES_H__ */
/*--------------------------------------------------------------------------*/ 
