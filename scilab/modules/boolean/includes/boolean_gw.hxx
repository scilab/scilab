/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __BOOLEAN_GW_HXX__
#define __BOOLEAN_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_boolean_gw.h"
}

class BooleanModule
{
private :
    BooleanModule() {};
    ~BooleanModule() {};
public :
    BOOLEAN_GW_IMPEXP static int Load();
    BOOLEAN_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_bool2s);
CPP_GATEWAY_PROTOTYPE(sci_find);
CPP_GATEWAY_PROTOTYPE(sci_and);
CPP_GATEWAY_PROTOTYPE(sci_or);

#endif /* __BOOLEAN_GW_HXX__ */
