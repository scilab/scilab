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
