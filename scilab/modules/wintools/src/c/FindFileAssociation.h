/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __FINDFILEASSOCIATION_H__
#define __FINDFILEASSOCIATION_H__

/**
* returns full name of application associated to a file extension
* example : FindFileAssociation (".sce","print");
* returns "c:\programs files\scilab-5.0\wscilex.exe"
*/
char * FindFileAssociation (char *ptrFindStr,char *Extra);

#endif /* __FINDFILEASSOCIATION_H__ */
/*-----------------------------------------------------------------------------------*/ 
