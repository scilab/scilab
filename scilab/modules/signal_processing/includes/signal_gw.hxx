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

#ifndef __SIGNAL_GW_HXX__
#define __SIGNAL_GW_HXX__

#include "cpp_gateway_prototype.hxx"
extern "C"
{
#include "dynlib_signal_processing_gw.h"
}

class SignalProcessingModule
{
private :
    SignalProcessingModule() {};
    ~SignalProcessingModule() {};
public :
    EXTERN_SIGNAL_PROCESSING_GW static int Load();
    EXTERN_SIGNAL_PROCESSING_GW static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_rpem, EXTERN_SIGNAL_PROCESSING_GW);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_corr, EXTERN_SIGNAL_PROCESSING_GW);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_delip, EXTERN_SIGNAL_PROCESSING_GW);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_fft, EXTERN_SIGNAL_PROCESSING_GW);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_syredi, EXTERN_SIGNAL_PROCESSING_GW);

#endif /* !__SIGNAL_GW_HXX__ */
