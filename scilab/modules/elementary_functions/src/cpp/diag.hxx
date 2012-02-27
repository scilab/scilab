/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - DIGITEO - cedric delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __DIAG_H__
#define __DIAG_H__

#include "double.hxx"
#include "polynom.hxx"
#include "string.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

ELEMENTARY_FUNCTIONS_IMPEXP types::Double* diag(types::Double* pIn,  int iStartPos);
ELEMENTARY_FUNCTIONS_IMPEXP types::Polynom* diag(types::Polynom* pIn,  int iStartPos);
ELEMENTARY_FUNCTIONS_IMPEXP types::String* diag(types::String* pIn,  int iStartPos);

#endif /* __DIAG_H__ */
