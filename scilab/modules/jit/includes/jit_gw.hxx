/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __JIT_GW_HXX__
#define __JIT_GW_HXX__


#include "dynlib_jit_gw.h"

#include "cpp_gateway_prototype.hxx"

class JITModule
{
private :
    JITModule() {};
    ~JITModule() {};

public :
    JIT_GW_IMPEXP static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_enableJIT);
CPP_GATEWAY_PROTOTYPE(sci_disableJIT);
CPP_GATEWAY_PROTOTYPE(sci_isJIT);

#endif /* !__JIT_GW_HXX__ */
