/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2012 - DIGITEO - cedric delamarre
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

#ifndef __GSORT_H__
#define __GSORT_H__

#include "double.hxx"
#include "sparse.hxx"
#include "string.hxx"
#include "int.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

ELEMENTARY_FUNCTIONS_IMPEXP types::Double* gsort(types::Double* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);

ELEMENTARY_FUNCTIONS_IMPEXP types::String* gsort(types::String* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);

ELEMENTARY_FUNCTIONS_IMPEXP types::Int8* gsort(types::Int8* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::Int16* gsort(types::Int16* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::Int32* gsort(types::Int32* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::Int64* gsort(types::Int64* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::UInt8* gsort(types::UInt8* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::UInt16* gsort(types::UInt16* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::UInt32* gsort(types::UInt32* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::UInt64* gsort(types::UInt64* pIn, types::Double* pInd, const std::wstring& wstrWay, const std::wstring& wstrProcess);

#endif /* __GSORT_H__ */
