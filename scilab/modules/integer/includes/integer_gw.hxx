/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
