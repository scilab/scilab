/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
};

CPP_GATEWAY_PROTOTYPE(sci_clc);
CPP_GATEWAY_PROTOTYPE(sci_iswaitingforinput);
CPP_GATEWAY_PROTOTYPE(sci_lines);
CPP_GATEWAY_PROTOTYPE(sci_tohome);
CPP_GATEWAY_PROTOTYPE(sci_prompt);

#endif /* !__SHELL_GW_HXX__ */