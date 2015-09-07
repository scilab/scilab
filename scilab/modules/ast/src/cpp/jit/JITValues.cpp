/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sstream>
#include <string>

#include "JITValues.hxx"
#include "JITVisitor.hxx"

#include "alltypes.hxx"

namespace jit
{

JITVal * JITVal::get(JITVisitor & visitor, types::InternalType * const pIT, const bool alloc, const std::string & name)
{
    if (pIT->isGenericType())
    {
        types::GenericType * pGT = static_cast<types::GenericType *>(pIT);
        if (pGT->isScalar())
        {
            switch (pGT->getType())
            {
                case types::InternalType::ScilabInt8:
                    return new JITScalarVal<char>(visitor, static_cast<types::Int8 *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabUInt8:
                    return new JITScalarVal<unsigned char>(visitor, static_cast<types::UInt8 *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabInt16:
                    return new JITScalarVal<short>(visitor, static_cast<types::Int16 *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabUInt16:
                    return new JITScalarVal<unsigned short>(visitor, static_cast<types::UInt16 *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabInt32:
                    return new JITScalarVal<int>(visitor, static_cast<types::Int32 *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabUInt32:
                    return new JITScalarVal<unsigned int>(visitor, static_cast<types::UInt32 *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabInt64:
                    return new JITScalarVal<long long>(visitor, static_cast<types::Int64 *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabUInt64:
                    return new JITScalarVal<unsigned long long>(visitor, static_cast<types::UInt64 *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabDouble:
                    return new JITScalarVal<double>(visitor, static_cast<types::Double *>(pGT)->get(0), alloc, name);
                case types::InternalType::ScilabBool:
                    return new JITScalarVal<bool>(visitor, static_cast<types::Bool *>(pGT)->get(0), alloc, name);
                default:
                    throw ast::InternalError(std::wstring(L"Type not handled by JIT compiler: ") + pGT->getTypeStr());
            }
        }
        else
        {
            switch (pGT->getType())
            {
                case types::InternalType::ScilabInt8:
                {
                    types::Int8 * p = static_cast<types::Int8 *>(pGT);
                    return new JITMatrixVal<char>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabUInt8:
                {
                    types::UInt8 * p = static_cast<types::UInt8 *>(pGT);
                    return new JITMatrixVal<unsigned char>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabInt16:
                {
                    types::Int16 * p = static_cast<types::Int16 *>(pGT);
                    return new JITMatrixVal<short>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabUInt16:
                {
                    types::UInt16 * p = static_cast<types::UInt16 *>(pGT);
                    return new JITMatrixVal<unsigned short>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabInt32:
                {
                    types::Int32 * p = static_cast<types::Int32 *>(pGT);
                    return new JITMatrixVal<int>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabUInt32:
                {
                    types::UInt32 * p = static_cast<types::UInt32 *>(pGT);
                    return new JITMatrixVal<unsigned int>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabInt64:
                {
                    types::Int64 * p = static_cast<types::Int64 *>(pGT);
                    return new JITMatrixVal<long long>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabUInt64:
                {
                    types::UInt64 * p = static_cast<types::UInt64 *>(pGT);
                    return new JITMatrixVal<unsigned long long>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabDouble:
                {
                    types::Double * p = static_cast<types::Double *>(pGT);
                    return new JITMatrixVal<double>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                case types::InternalType::ScilabBool:
                {
                    types::Bool * p = static_cast<types::Bool *>(pGT);
                    return new JITMatrixVal<int>(visitor, p->getRows(), p->getCols(), p->get(), alloc, name);
                }
                default:
                    throw ast::InternalError(std::wstring(L"Type not handled by JIT compiler: ") + pGT->getTypeStr());
            }
        }
    }

    throw ast::InternalError(std::wstring(L"Type not handled by JIT compiler: ") + pIT->getTypeStr());
}

JITVal * JITVal::get(JITVisitor & visitor, const analysis::TIType & t, const bool alloc, const std::string & name)
{
    if (t.isknown())
    {
        if (t.isscalar())
        {
            switch (t.type)
            {
                case analysis::TIType::BOOLEAN:
                    return new JITScalarVal<bool>(visitor, 0, alloc, name);
                case analysis::TIType::DOUBLE:
                    return new JITScalarVal<double>(visitor, 0, alloc, name);
                case analysis::TIType::INT16:
                    return new JITScalarVal<short>(visitor, 0, alloc, name);
                case analysis::TIType::INT32:
                    return new JITScalarVal<int>(visitor, 0, alloc, name);
                case analysis::TIType::INT64:
                    return new JITScalarVal<long long>(visitor, 0, alloc, name);
                case analysis::TIType::INT8:
                    return new JITScalarVal<char>(visitor, 0, alloc, name);
                case analysis::TIType::UINT16:
                    return new JITScalarVal<unsigned short>(visitor, 0, alloc, name);
                case analysis::TIType::UINT32:
                    return new JITScalarVal<unsigned int>(visitor, 0, alloc, name);
                case analysis::TIType::UINT64:
                    return new JITScalarVal<unsigned long long>(visitor, 0, alloc, name);
                case analysis::TIType::UINT8:
                    return new JITScalarVal<unsigned char>(visitor, 0, alloc, name);
                default :
                    std::wostringstream ostr;
                    ostr << t;
                    throw ast::InternalError(std::wstring(L"Type not handled by JIT compiler: ") + ostr.str());
            }
        }
        else
        {
            /*            switch (t.type)
                        {
                            case analysis::TIType::EMPTY:
                                return new JITMatrixVal<double>(visitor, 0, 0, nullptr, alloc, name);
                            case analysis::TIType::BOOLEAN:
                                return new JITMatrixVal<bool>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::DOUBLE:
                                return new JITMatrixVal<double>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::INT16:
                                return new JITMatrixVal<short>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::INT32:
                                return new JITMatrixVal<int>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::INT64:
                                return new JITMatrixVal<long long>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::INT8:
                                return new JITMatrixVal<char>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::UINT16:
                                return new JITMatrixVal<unsigned short>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::UINT32:
                                return new JITMatrixVal<unsigned int>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::UINT64:
                                return new JITMatrixVal<unsigned long long>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            case analysis::TIType::UINT8:
                                return new JITMatrixVal<unsigned char>(visitor, t.rows, t.cols, nullptr, alloc, name);
                            default :
                                std::wostringstream ostr;
                                ostr << t;
                                throw ast::ScilabError(std::wstring(L"Type not handled by JIT compiler: ") + ostr.str());
                        }
            */
        }
    }

    std::wostringstream ostr;
    ostr << t;
    throw ast::InternalError(std::wstring(L"Type not handled by JIT compiler: ") + ostr.str());
}
}
