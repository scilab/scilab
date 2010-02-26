/*--------------------------------------------------------------------------*/ 
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#ifndef __GETFULLFILENAME_HXX__
#define __GETFULLFILENAME_HXX__

#include <string>

/**
* get full filename with path of a file
* @param[in] std::wstring input filename
* @return std::wstring full filename
*/
std::wstring getFullFilename(std::wstring _wfilename);


std::wstring getUniqueFilename(std::wstring _wfilename);

#endif /* __GETFULLFILENAME_HXX__ */
/*--------------------------------------------------------------------------*/ 
