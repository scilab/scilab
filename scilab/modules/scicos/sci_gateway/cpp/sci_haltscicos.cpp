/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
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

#include "types.hxx"
#include "function.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "scicos-def.h"

#include "localization.h"
#include "Scierror.h"
    /*--------------------------------------------------------------------------*/
    // Variable defined in scicos.c
    extern COSHLT_struct C2F(coshlt);
    /*--------------------------------------------------------------------------*/
}

static const std::string funname = "sci_haltscicos";

types::Function::ReturnValue sci_haltscicos(types::typed_list &in, int _iRetCount, types::typed_list &/*out*/)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), funname.c_str(), 0);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }

    // MAGIC VALUE: 0 is used to continue the simulation
    // MAGIC VALUE: 1 is used to halt the simulator
    // MAGIC VALUE: 2 is used to switch to the final time
    //                        then halt the simulator
    C2F(coshlt).halt = 2;

    return types::Function::OK;
}
