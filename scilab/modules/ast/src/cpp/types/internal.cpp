/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#include "exp.hxx" // for invoke
#include "callexp.hxx"

#include "internal.hxx"

namespace types
{

void InternalType::whoAmI(void)
{
    std::cout << "types::Internal";
}

bool InternalType::isAssignable(void)
{
    return false;
}

bool InternalType::hasToString()
{
    return true;
}

std::wstring InternalType::toStringInLine()
{
    return getTypeStr();
}

ast::Exp * InternalType::getExp(const Location& /*loc*/)
{
    return nullptr;
}

bool InternalType::isTrue()
{
    return false;
}

bool InternalType::neg(InternalType *& /*out*/)
{
    return false;
}

bool InternalType::transpose(InternalType *& /*out*/)
{
    return false;
}

bool InternalType::adjoint(InternalType *& out)
{
    return transpose(out);
}

bool InternalType::isFieldExtractionOverloadable() const
{
    return false;
}

bool InternalType::invoke(typed_list & /*in*/, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & /*out*/, const ast::Exp & /*e*/)
{
    return false;
}

bool InternalType::isInvokable() const
{
    return false;
}

bool InternalType::hasInvokeOption() const
{
    return false;
}

int InternalType::getInvokeNbIn()
{
    return -1;
}

int InternalType::getInvokeNbOut()
{
    return -1;
}

bool InternalType::operator==(const InternalType& it)
{
    return (getType() == (const_cast<InternalType *>(&it))->getType());
}

bool InternalType::operator!=(const InternalType& it)
{
    return !(*this == it);
}

bool InternalType::isGenericType(void)
{
    return false;
}

bool InternalType::isArrayOf(void)
{
    return false;
}

bool InternalType::isString(void)
{
    return false;
}

bool InternalType::isDouble(void)
{
    return false;
}

bool InternalType::isSparse(void)
{
    return false;
}

bool InternalType::isSparseBool(void)
{
    return false;
}

bool InternalType::isFloat(void)
{
    return false;
}

bool InternalType::isInt(void)
{
    return false;
}
bool InternalType::isInt8(void)
{
    return false;
}

bool InternalType::isUInt8(void)
{
    return false;
}

bool InternalType::isInt16(void)
{
    return false;
}

bool InternalType::isUInt16(void)
{
    return false;
}

bool InternalType::isInt32(void)
{
    return false;
}

bool InternalType::isUInt32(void)
{
    return false;
}

bool InternalType::isInt64(void)
{
    return false;
}

bool InternalType::isUInt64(void)
{
    return false;
}

bool InternalType::isBool(void)
{
    return false;
}

bool InternalType::isPoly(void)
{
    return false;
}

bool InternalType::isSinglePoly(void)
{
    return false;
}

bool InternalType::isCallable(void)
{
    return false;
}

bool InternalType::isFunction(void)
{
    return false;
}

bool InternalType::isMacro(void)
{
    return false;
}

bool InternalType::isMacroFile(void)
{
    return false;
}

bool InternalType::isContainer(void)
{
    return false;
}

bool InternalType::isList(void)
{
    return false;
}

bool InternalType::isStruct(void)
{
    return false;
}

bool InternalType::isSingleStruct(void)
{
    return false;
}

bool InternalType::isCell(void)
{
    return false;
}

bool InternalType::isTList(void)
{
    return false;
}

bool InternalType::isMList(void)
{
    return false;
}

bool InternalType::isImplicitList(void)
{
    return false;
}

bool InternalType::isColon(void)
{
    return false;
}

bool InternalType::isDollar(void)
{
    return false;
}

bool InternalType::isFile(void)
{
    return false;
}

bool InternalType::isHandle(void)
{
    return false;
}

bool InternalType::isSingleHandle(void)
{
    return false;
}

bool InternalType::isThreadId(void)
{
    return false;
}

bool InternalType::isListOperation(void)
{
    return false;
}

bool InternalType::isListDelete(void)
{
    return false;
}

bool InternalType::isListInsert(void)
{
    return false;
}

bool InternalType::isListUndefined(void)
{
    return false;
}

bool InternalType::isPointer(void)
{
    return false;
}

bool InternalType::isLibrary(void)
{
    return false;
}

bool InternalType::isUserType(void)
{
    return false;
}

void InternalType::clearPrintState()
{
    m_bPrintFromStart = true;
    m_iSavePrintState = 0;
    m_iRows1PrintState = 0;
    m_iCols1PrintState = 0;
    m_iRows2PrintState = 0;
    m_iCols2PrintState = 0;
}

}
