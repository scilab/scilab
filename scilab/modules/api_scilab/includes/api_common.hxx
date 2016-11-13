/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifdef _MSC_VER
#pragma warning( disable : 4275 )
#endif

#ifndef __API_COMMON_HXX__
#define __API_COMMON_HXX__

#include "api_scilab.hxx"
#include "types.hxx"
#include "function.hxx"

namespace api_scilab
{
const types::Function::ReturnValue OK = types::Function::OK;
const types::Function::ReturnValue Error = types::Function::Error;
typedef types::Function::ReturnValue Status;
typedef types::typed_list input;
typedef types::typed_list output;
typedef types::typed_list optional;
API_SCILAB_IMPEXP types::InternalType::ScilabType getType(types::InternalType* _pIT);
API_SCILAB_IMPEXP types::InternalType* getReturnVariable(Variable* _pVar);

API_SCILAB_IMPEXP Variable* getAsVariable(types::InternalType* _pIT);

API_SCILAB_IMPEXP bool isMatrix(types::InternalType* _pIT);
API_SCILAB_IMPEXP Matrix* getAsMatrix(types::InternalType* _pIT);

/*Double*/
API_SCILAB_IMPEXP bool isDouble(types::InternalType* _pIT);
API_SCILAB_IMPEXP Double* getAsDouble(types::InternalType* _pIT);

/*String*/
API_SCILAB_IMPEXP bool isString(types::InternalType* _pIT);
API_SCILAB_IMPEXP String* getAsString(types::InternalType* _pIT);

/*Boolean*/
API_SCILAB_IMPEXP bool isBoolean(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isBool(types::InternalType* _pIT);
API_SCILAB_IMPEXP Bool* getAsBool(types::InternalType* _pIT);
API_SCILAB_IMPEXP Bool* getAsBoolean(types::InternalType* _pIT);

/*Int*/
API_SCILAB_IMPEXP bool isInt(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isInt8(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isUInt8(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isInt16(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isUInt16(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isInt32(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isUInt32(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isInt64(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isUInt64(types::InternalType* _pIT);

API_SCILAB_IMPEXP Int8* getAsInt8(types::InternalType* _pIT);
API_SCILAB_IMPEXP UInt8* getAsUInt8(types::InternalType* _pIT);
API_SCILAB_IMPEXP Int16* getAsInt16(types::InternalType* _pIT);
API_SCILAB_IMPEXP UInt16* getAsUInt16(types::InternalType* _pIT);
API_SCILAB_IMPEXP Int32* getAsInt32(types::InternalType* _pIT);
API_SCILAB_IMPEXP UInt32* getAsUInt32(types::InternalType* _pIT);
API_SCILAB_IMPEXP Int64* getAsInt64(types::InternalType* _pIT);
API_SCILAB_IMPEXP UInt64* getAsUInt64(types::InternalType* _pIT);

API_SCILAB_IMPEXP bool isSparse(types::InternalType* _pIT);
API_SCILAB_IMPEXP Sparse* getAsSparse(types::InternalType* _pIT);

API_SCILAB_IMPEXP bool isHandle(types::InternalType* _pIT);
API_SCILAB_IMPEXP bool isGraphicHandle(types::InternalType* _pIT);
API_SCILAB_IMPEXP GraphicHandle* getAsHandle(types::InternalType* _pIT);
API_SCILAB_IMPEXP GraphicHandle* getAsGraphicHandle(types::InternalType* _pIT);

API_SCILAB_IMPEXP bool isPointer(types::InternalType* _pIT);
API_SCILAB_IMPEXP Pointer* getAsPointer(types::InternalType* _pIT);

API_SCILAB_IMPEXP bool isList(types::InternalType* _pIT);
API_SCILAB_IMPEXP List* getAsList(types::InternalType* _pIT);

API_SCILAB_IMPEXP bool isTList(types::InternalType* _pIT);
API_SCILAB_IMPEXP TList* getAsTList(types::InternalType* _pIT);

API_SCILAB_IMPEXP bool isMList(types::InternalType* _pIT);
API_SCILAB_IMPEXP MList* getAsMList(types::InternalType* _pIT);
}

#endif /* ! __API_COMMON_HXX__ */

