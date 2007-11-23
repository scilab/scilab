/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __FINDFILEEXTENSION_H__
#define __FINDFILEEXTENSION_H__

/**
* Searches a path for an extension
* @param[in] pathname
* @return the address of the "." preceding the extension 
* NULL character otherwise. 
*/
char *FindFileExtension(char *filename);

#endif /* __FINDFILEEXTENSION_H__ */
/*--------------------------------------------------------------------------*/
