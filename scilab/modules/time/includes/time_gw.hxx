/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __TIME_GW_HXX__
#define __TIME_GW_HXX__

#include "funcmanager.hxx"
#include "context.hxx"
#include "cpp_gateway_prototype.hxx"
#include "dynlib_time_gw.h"

class TimeModule
{
private :
    TimeModule() {};
    ~TimeModule() {};
public :
    EXTERN_TIME_GW static int Load();
    EXTERN_TIME_GW static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_tic);
CPP_GATEWAY_PROTOTYPE(sci_toc);


#endif /* !__TIME_GW_HXX__ */
