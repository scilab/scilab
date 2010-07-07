/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __STRIPBLANKS_HXX__
#define __STRIPBLANKS_HXX__

#include "dynlib_string.h"
#include "types.hxx"
#include "string.hxx"
#include "bool.hxx"

using namespace types;

/**
* remove blank or tab characters in a string matrix
* @param[in] String: input Strings
* @param[in] Bool: remove TAB if bool_get() != 0  
* @return String: output String
*/
STRING_IMPEXP String * stripblanks(String *InputStrings, bool bWithTAB);

#endif /* __STRIPBLANKS_HXX__ */
/*--------------------------------------------------------------------------*/
