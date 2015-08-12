/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __SEARCHMACROINLIBRARIES_H__
#define __SEARCHMACROINLIBRARIES_H__

/**
* search a macro in all scilab libraries
* and returns all libraries names
* @param[in] macro name
* @param[out] size of returned array
* @return array of char where macro founded
*/

char **searchmacroinlibraries(char *macro, int *sizeReturnedArray);

#endif /* __SEARCHMACROINLIBRARIES_H__ */
/*--------------------------------------------------------------------------*/