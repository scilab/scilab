/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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
    JVM_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};
CPP_GATEWAY_PROTOTYPE(sci_system_getproperty);
CPP_GATEWAY_PROTOTYPE(sci_system_setproperty);

#endif /* !__JVM_GW_HXX__ */
