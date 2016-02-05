/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#ifndef __SHELL_GW_HXX__
#define __SHELL_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_console_gw.h"
}

class ConsoleModule
{
private :
    ConsoleModule() {};
    ~ConsoleModule() {};
public :
    EXTERN_CONSOLE_GW static int Load();
    EXTERN_CONSOLE_GW static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_clc);
CPP_GATEWAY_PROTOTYPE(sci_iswaitingforinput);
CPP_GATEWAY_PROTOTYPE(sci_lines);
CPP_GATEWAY_PROTOTYPE(sci_tohome);
CPP_GATEWAY_PROTOTYPE(sci_prompt);

#endif /* !__SHELL_GW_HXX__ */