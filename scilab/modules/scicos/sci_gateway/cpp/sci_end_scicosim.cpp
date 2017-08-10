/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) INRIA - Allan CORNET
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

#include <string>

#include "gw_scicos.hxx"

#include "internal.hxx"
#include "types.hxx"
#include "function.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "scicos.h" /* end_scicos_sim() */
#include "scicos-def.h"
#include "localization.h"
#include "Scierror.h"

    /*--------------------------------------------------------------------------*/
    // Variable defined in sci_scicosim.cpp
    extern COSIM_struct C2F(cosim);
    /*--------------------------------------------------------------------------*/
}

static const std::string funname = "end_scicosim";

types::Function::ReturnValue sci_end_scicosim(types::typed_list &in, int _iRetCount, types::typed_list &/*out*/)
{
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 0);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    const int isrun = C2F(cosim).isrun;
    if (!isrun)
    {
        Scierror(999, _("%s: scicosim is not running.\n"), funname.data());
        return types::Function::Error;
    }
    end_scicos_sim();

    return types::Function::OK;
}
