/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DATA_STRUCTURES_GW_HXX__
#define __DATA_STRUCTURES_GW_HXX__

#include "dynlib_data_structures.h"

#include "cpp_gateway_prototype.hxx"

class DataStructuresModule
{
private:
    DataStructuresModule() {};
    ~DataStructuresModule() {};

public:
    DATA_STRUCTURES_IMPEXP static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_getfield);
CPP_GATEWAY_PROTOTYPE(sci_setfield);
CPP_GATEWAY_PROTOTYPE(sci_rlist);

#endif /* !__DATA_STRUCTURES_GW_HXX__ */
