/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __ADDGATEWAYINCONTEXT_H__
#define __ADDGATEWAYINCONTEXT_H__
#include "export_symbol.h"
#include "c_gateway_prototype.h"

#ifdef __cplusplus
extern "C"
{
#endif
    EXTERN_SYMBOL void addGatewayInContext(wchar_t* _pwstName, OLDGW_FUNC _pFunc, wchar_t* _pwstModule);
#ifdef __cplusplus
}
#endif

#endif /* !__ADDGATEWAYINCONTEXT_H__ */