/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
};

CPP_GATEWAY_PROTOTYPE(sci_tic);
CPP_GATEWAY_PROTOTYPE(sci_toc);


#endif /* !__TIME_GW_HXX__ */
