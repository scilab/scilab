/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GETMATLABVARIABLE_HXX__
#define __GETMATLABVARIABLE_HXX__

#include "gatewaystruct.hxx"
#include "double.hxx"
#include "int.hxx"
#include "struct.hxx"
#include "context.hxx"
#include "string.hxx"
#include "sparse.hxx"

extern "C"
{
#include "GetMatlabVariable.h"
#include "dynlib_matio.h"
}


matvar_t* GetDoubleMatVar(types::Double* pDblIn, const char* name, int matfile_version);

matvar_t* GetIntegerMatVar(types::InternalType* pITIn, const char* name);

matvar_t* GetCharMatVar(types::String* pStr, const char* name);

matvar_t* GetSparseMatVar(types::Sparse* pSparse, const char* name);

#endif /* !__GETMATLABVARIABLE_HXX__ */
