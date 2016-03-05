/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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
    DATA_STRUCTURES_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_getfield);
CPP_GATEWAY_PROTOTYPE(sci_setfield);
CPP_GATEWAY_PROTOTYPE(sci_rlist);
CPP_GATEWAY_PROTOTYPE(sci_definedfields);

#endif /* !__DATA_STRUCTURES_GW_HXX__ */
