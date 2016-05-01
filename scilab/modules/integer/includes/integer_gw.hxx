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

#ifndef __INTEGER_GW_HXX__
#define __INTEGER_GW_HXX__

#include "cpp_gateway_prototype.hxx"

#include "dynlib_integer_gw.h"

class IntegerModule
{
private :
    IntegerModule() {};
    ~IntegerModule() {};
public :
    INTEGER_GW_IMPEXP static int Load();
    INTEGER_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_inttype);
CPP_GATEWAY_PROTOTYPE(sci_double);
CPP_GATEWAY_PROTOTYPE(sci_iconvert);
//Scilab 6
CPP_GATEWAY_PROTOTYPE(sci_integer8);
CPP_GATEWAY_PROTOTYPE(sci_uinteger8);
CPP_GATEWAY_PROTOTYPE(sci_integer16);
CPP_GATEWAY_PROTOTYPE(sci_uinteger16);
CPP_GATEWAY_PROTOTYPE(sci_integer32);
CPP_GATEWAY_PROTOTYPE(sci_uinteger32);
CPP_GATEWAY_PROTOTYPE(sci_integer64);
CPP_GATEWAY_PROTOTYPE(sci_uinteger64);

#endif /* __INTEGER_GW_HXX__ */
