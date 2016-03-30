/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
wchar_t* FindFileAssociation(const wchar_t* ptrFindStr, const wchar_t* Extra);

#endif /* __FINDFILEASSOCIATION_H__ */
/*--------------------------------------------------------------------------*/
