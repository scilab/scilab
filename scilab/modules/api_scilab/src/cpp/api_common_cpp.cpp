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

#include "api_common.hxx"

namespace api_scilab
{
types::InternalType::ScilabType getType(types::InternalType* _pIT)
{
    if (_pIT)
    {
        return _pIT->getType();
    }

    return types::InternalType::ScilabNull;
}

types::InternalType* getReturnVariable(Variable* _pVar)
{
    _pVar->setReturnVariable();
    return (types::InternalType*)_pVar->getReturnVariable();
}

Variable* getAsVariable(types::InternalType* _pIT)
{
    Variable* pOut = NULL;

    switch (_pIT->getType())
    {
        case types::InternalType::ScilabDouble :
        {
            return getAsDouble(_pIT);
        }
        case types::InternalType::ScilabString :
        {
            return getAsString(_pIT);
        }
        case types::InternalType::ScilabBool :
        {
            return getAsBoolean(_pIT);
        }
        case types::InternalType::ScilabSparse :
        {
            return getAsSparse(_pIT);
        }
        case types::InternalType::ScilabSparseBool :
        {
            //return getAsSparseBool(_pIT);
            return NULL;
        }
        case types::InternalType::ScilabHandle :
        {
            return getAsHandle(_pIT);
        }
        case types::InternalType::ScilabUserType :
        {
            return getAsPointer(_pIT);
        }
        case types::InternalType::ScilabList :
        {
            return getAsList(_pIT);
        }
        case types::InternalType::ScilabMList :
        {
            return getAsMList(_pIT);
        }
        case types::InternalType::ScilabTList :
        {
            return getAsTList(_pIT);
        }
        case types::InternalType::ScilabPolynom :
        {
            //return getAsPolynom(_pIT);
            return NULL;
        }
        case types::InternalType::ScilabStruct :
        {
            //return getAsStruct(_pIT);
            return NULL;
        }
        default:
        {
            return NULL;
        }
    }

    return NULL;
}

bool isMatrix(types::InternalType* _pIT)
{
    switch (_pIT->getType())
    {
        case types::InternalType::ScilabDouble :
        case types::InternalType::ScilabString :
        case types::InternalType::ScilabBool :
        case types::InternalType::ScilabInt8 :
        case types::InternalType::ScilabUInt8 :
        case types::InternalType::ScilabInt16 :
        case types::InternalType::ScilabUInt16 :
        case types::InternalType::ScilabInt32 :
        case types::InternalType::ScilabUInt32 :
        case types::InternalType::ScilabInt64 :
        case types::InternalType::ScilabUInt64 :
        case types::InternalType::ScilabSparse :
        case types::InternalType::ScilabSparseBool :
        case types::InternalType::ScilabHandle :
        {
            return true;
        }
        default :
        {
            return false;
        }
    }
}

Matrix* getAsMatrix(types::InternalType* _pIT)
{
    if (isMatrix(_pIT))
    {
        return dynamic_cast<Matrix*>(getAsVariable(_pIT));
    }

    return NULL;
}

/* Double */
bool isDouble(types::InternalType* _pIT)
{
    return _pIT->isDouble();
}

Double* getAsDouble(types::InternalType* _pIT)
{
    if (_pIT->isDouble())
    {
        return Double::getAsDouble(_pIT);
    }

    return NULL;
}

/* String */
bool isString(types::InternalType* _pIT)
{
    return _pIT->isString();
}

String* getAsString(types::InternalType* _pIT)
{
    if (_pIT->isString())
    {
        return String::getAsString(_pIT);
    }

    return NULL;
}

/* Boolean */
bool isBoolean(types::InternalType* _pIT)
{
    return _pIT->isBool();
}

bool isBool(types::InternalType* _pIT)
{
    return _pIT->isBool();
}

Bool* getAsBoolean(types::InternalType* _pIT)
{
    if (_pIT->isBool())
    {
        return Bool::getAsBool(_pIT);
    }

    return NULL;
}

Bool* getAsBool(types::InternalType* _pIT)
{
    if (_pIT->isBool())
    {
        return Bool::getAsBool(_pIT);
    }

    return NULL;
}

/* Int */
bool isInt(types::InternalType* _pIT)
{
    return _pIT->isInt();
}

bool isInt8(types::InternalType* _pIT)
{
    return _pIT->isInt8();
}

bool isUInt8(types::InternalType* _pIT)
{
    return _pIT->isUInt8();
}

bool isInt16(types::InternalType* _pIT)
{
    return _pIT->isInt16();
}

bool isUInt16(types::InternalType* _pIT)
{
    return _pIT->isUInt16();
}

bool isInt32(types::InternalType* _pIT)
{
    return _pIT->isInt32();
}

bool isUInt32(types::InternalType* _pIT)
{
    return _pIT->isUInt32();
}

bool isInt64(types::InternalType* _pIT)
{
    return _pIT->isInt64();
}

bool isUInt64(types::InternalType* _pIT)
{
    return _pIT->isUInt64();
}

Int8* getAsInt8(types::InternalType* _pIT)
{
    if (_pIT->isInt8())
    {
        return Int8::getAsInt8(_pIT);
    }

    return NULL;
}

UInt8* getAsUInt8(types::InternalType* _pIT)
{
    if (_pIT->isUInt8())
    {
        return UInt8::getAsUInt8(_pIT);
    }

    return NULL;
}

Int16* getAsInt16(types::InternalType* _pIT)
{
    if (_pIT->isInt16())
    {
        return Int16::getAsInt16(_pIT);
    }

    return NULL;
}

UInt16* getAsUInt16(types::InternalType* _pIT)
{
    if (_pIT->isUInt16())
    {
        return UInt16::getAsUInt16(_pIT);
    }

    return NULL;
}

Int32* getAsInt32(types::InternalType* _pIT)
{
    if (_pIT->isInt32())
    {
        return Int32::getAsInt32(_pIT);
    }

    return NULL;
}

UInt32* getAsUInt32(types::InternalType* _pIT)
{
    if (_pIT->isUInt32())
    {
        return UInt32::getAsUInt32(_pIT);
    }

    return NULL;
}

Int64* getAsInt64(types::InternalType* _pIT)
{
    if (_pIT->isInt64())
    {
        return Int64::getAsInt64(_pIT);
    }

    return NULL;
}

UInt64* getAsUInt64(types::InternalType* _pIT)
{
    if (_pIT->isUInt64())
    {
        return UInt64::getAsUInt64(_pIT);
    }

    return NULL;
}

/* Sparse */
bool isSparse(types::InternalType* _pIT)
{
    return _pIT->isSparse();
}

Sparse* getAsSparse(types::InternalType* _pIT)
{
    if (_pIT->isSparse())
    {
        return Sparse::getAsSparse(_pIT);
    }

    return NULL;
}

bool isHandle(types::InternalType* _pIT)
{
    return _pIT->isHandle();
}

bool isGraphicHandle(types::InternalType* _pIT)
{
    return _pIT->isHandle();
}

GraphicHandle* getAsHandle(types::InternalType* _pIT)
{
    if (_pIT->isHandle())
    {
        return GraphicHandle::getAsGraphicHandle(_pIT);
    }

    return NULL;
}

GraphicHandle* getAsGraphicHandle(types::InternalType* _pIT)
{
    if (_pIT->isHandle())
    {
        return GraphicHandle::getAsGraphicHandle(_pIT);
    }

    return NULL;
}

bool isPointer(types::InternalType* _pIT)
{
    return _pIT->isPointer();
}

Pointer* getAsPointer(types::InternalType* _pIT)
{
    if (_pIT->isPointer())
    {
        return Pointer::getAsPointer(_pIT);
    }

    return NULL;
}

bool isList(types::InternalType* _pIT)
{
    return _pIT->isList();
}

List* getAsList(types::InternalType* _pIT)
{
    if (_pIT->isList())
    {
        return List::getAsList(_pIT);
    }

    return NULL;
}

bool isTList(types::InternalType* _pIT)
{
    return _pIT->isTList();
}

TList* getAsTList(types::InternalType* _pIT)
{
    if (_pIT->isTList())
    {
        return TList::getAsTList(_pIT);
    }

    return NULL;
}

bool isMList(types::InternalType* _pIT)
{
    return _pIT->isMList();
}

MList* getAsMList(types::InternalType* _pIT)
{
    if (_pIT->isMList())
    {
        return MList::getAsMList(_pIT);
    }

    return NULL;
}

}
