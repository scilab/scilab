/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
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
#ifndef __GATEWAYSTRUCT_HXX__
#define __GATEWAYSTRUCT_HXX__

#include "internal.hxx"
#include "visitor.hxx"
#include <string>

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

namespace types
{
class EXTERN_AST GatewayStruct
{
public :
    types::typed_list*      m_pIn;
    optional_list*          m_pOpt;
    types::InternalType**   m_pOut;
    int                     m_iIn;
    int                     m_iOut;
    int*                    m_piRetCount;
    const char*             m_pstName;
    int*                    m_pOutOrder;

    GatewayStruct(): m_pIn(nullptr), m_pOpt(nullptr), m_pOut(nullptr), m_iIn(0), m_iOut(0), m_piRetCount(nullptr), m_pOutOrder(nullptr) {};
    ~GatewayStruct() {};
};

struct EXTERN_AST GatewayCStruct
{
    std::string     name;
    std::wstring    lasterror;
};
}
#endif /* !__GATEWAYSTRUCT_HXX__ */
