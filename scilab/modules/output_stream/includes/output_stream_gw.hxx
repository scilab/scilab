/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
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

#ifndef __OUTPUTSTREAM_GW_HXX__
#define __OUTPUTSTREAM_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_output_stream_gw.h"
}

class OutputStreamModule
{
private :
    OutputStreamModule() {};
    ~OutputStreamModule() {};

public :
    OUTPUT_STREAM_IMPEXP_GW static int Load();
    OUTPUT_STREAM_IMPEXP_GW static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_disp);
CPP_GATEWAY_PROTOTYPE(sci_msprintf);
CPP_GATEWAY_PROTOTYPE(sci_mprintf);

#endif /* !__OUTPUTSTREAM_GW_HXX__ */
