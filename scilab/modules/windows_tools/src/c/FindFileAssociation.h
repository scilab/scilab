/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

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
char * FindFileAssociation (char *ptrFindStr, char *Extra);

#endif /* __FINDFILEASSOCIATION_H__ */
/*--------------------------------------------------------------------------*/
