/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __FFTW_GW_HXX__
#define __FFTW_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "dynlib_fftw.h"

class FFTWModule
{
private:
    FFTWModule() {};
    ~FFTWModule() {};
public:
    FTTW_SCILAB_IMPEXP static int Load();
    FTTW_SCILAB_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_dct, FTTW_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_dst, FTTW_SCILAB_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_fftw, FTTW_SCILAB_IMPEXP);

#endif /* __FFTW_GW_HXX__ */
