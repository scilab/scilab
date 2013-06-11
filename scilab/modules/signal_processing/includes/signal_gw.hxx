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
};

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_rpem, EXTERN_SIGNAL_PROCESSING_GW);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_corr, EXTERN_SIGNAL_PROCESSING_GW);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_delip, EXTERN_SIGNAL_PROCESSING_GW);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_fft, EXTERN_SIGNAL_PROCESSING_GW);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_syredi, EXTERN_SIGNAL_PROCESSING_GW);

#endif /* !__SIGNAL_GW_HXX__ */
