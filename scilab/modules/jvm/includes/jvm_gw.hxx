/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __JVM_GW_HXX__
#define __JVM_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_jvm_gw.h"
}

class JvmModule
{
private :
    JvmModule() {};
    ~JvmModule() {};

public :
    JVM_GW_IMPEXP static int Load();
};
CPP_GATEWAY_PROTOTYPE(sci_system_getproperty);

#endif /* !__JVM_GW_HXX__ */
