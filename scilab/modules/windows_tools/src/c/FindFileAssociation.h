/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __FINDFILEASSOCIATION_H__
#define __FINDFILEASSOCIATION_H__

/**
* returns full name of application associated to a file extension
* example : FindFileAssociation (".sce","print");
* returns "c:\programs files\scilab-5.0\wscilex.exe"
* @param[in] file extension
* @param[in] It is typically set to a Shell verb such as open. Set this parameter to NULL if it is not used.
* @return file associated
*/
char * FindFileAssociation (char *ptrFindStr,char *Extra);

#endif /* __FINDFILEASSOCIATION_H__ */
/*--------------------------------------------------------------------------*/ 
