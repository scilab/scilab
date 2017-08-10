/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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

#ifndef __GETMATLABVARIABLE_HXX__
#define __GETMATLABVARIABLE_HXX__

#include "gatewaystruct.hxx"
#include "double.hxx"
#include "int.hxx"
#include "struct.hxx"
#include "context.hxx"
#include "string.hxx"
#include "sparse.hxx"
#include "cell.hxx"
#include "mlist.hxx"


extern "C"
{
#include "GetMatlabVariable.h"
#include "dynlib_matio.h"
}


matvar_t* GetDoubleMatVar(types::Double* pDblIn, const char* name, int matfile_version);

matvar_t* GetIntegerMatVar(types::InternalType* pITIn, const char* name);

matvar_t* GetCharMatVar(types::String* pStr, const char* name);

matvar_t* GetSparseMatVar(types::Sparse* pSparse, const char* name);

matvar_t* GetStructMatVar(types::Struct* pStruct, const char* name, int matfile_version);

matvar_t* GetCellMatVar(types::Cell* pCell, const char* name, int matfile_version);

matvar_t* GetMListMatVar(types::MList* pMist, const char* name, int matfile_version);

#endif /* !__GETMATLABVARIABLE_HXX__ */
