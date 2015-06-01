/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TIType.hxx"

namespace analysis
{
    const std::wstring TIType::_boolean_ = L"boolean",
	TIType::_ce_ = L"ce",
	TIType::_constant_ = L"constant",
	TIType::_fptr_ = L"fptr",
	TIType::_function_ = L"function",
	TIType::_int16_ = L"int16",
	TIType::_int32_ = L"int32",
	TIType::_int64_ = L"int64",
	TIType::_int8_ = L"int8",
	TIType::_library_ = L"library",
	TIType::_list_ = L"list",
	TIType::_mlist_ = L"mlist",
	TIType::_polynomial_ = L"polynomial",
	TIType::_sparse_ = L"sparse",
	TIType::_st_ = L"st",
	TIType::_string_ = L"string",
	TIType::_tlist_ = L"tlist",
	TIType::_uint16_ = L"uint16",
	TIType::_uint32_ = L"uint32",
	TIType::_uint64_ = L"uint64",
	TIType::_uint8_ = L"uint8",
    	TIType::_unknown_ = L"";
}
