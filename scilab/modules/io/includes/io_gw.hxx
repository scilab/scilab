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

#ifndef __IO_GW_HXX__
#define __IO_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_io_gw.h"
}

class IoModule
{
private :
    IoModule() {};
    ~IoModule() {};
public :
    IO_GW_IMPEXP static int Load();
    IO_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

#endif /* __IO_GW_HXX__ */

CPP_GATEWAY_PROTOTYPE(sci_genlib);
CPP_GATEWAY_PROTOTYPE(sci_file);
CPP_GATEWAY_PROTOTYPE(sci_host);
CPP_GATEWAY_PROTOTYPE(sci_unix);
CPP_GATEWAY_PROTOTYPE(sci_lib);
CPP_GATEWAY_PROTOTYPE(sci_write);
CPP_GATEWAY_PROTOTYPE(sci_read);
