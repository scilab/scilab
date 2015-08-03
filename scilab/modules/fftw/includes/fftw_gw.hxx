/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
