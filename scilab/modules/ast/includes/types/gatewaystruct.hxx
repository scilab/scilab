/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

    GatewayStruct() {};
    ~GatewayStruct() {};
};

struct EXTERN_AST GatewayCStruct
{
    std::string name;
    std::string lasterror;
};
}
#endif /* !__GATEWAYSTRUCT_HXX__ */