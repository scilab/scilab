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
#include <vector>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "scicos.h" /* set_pointer_xproperty() */
#include "scicos-def.h"

#include "localization.h"
#include "Scierror.h"
    /*--------------------------------------------------------------------------*/
    // Variable defined in sci_scicosim.cpp
    extern COSIM_struct C2F(cosim);
    /*--------------------------------------------------------------------------*/
}

static const std::string funname = "set_xproperty";

types::Function::ReturnValue sci_set_xproperty(types::typed_list &in, int _iRetCount, types::typed_list &/*out*/)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    int isrun = C2F(cosim).isrun;
    if (!isrun)
    {
        Scierror(999, _("%s: scicosim is not running.\n"), funname.c_str());
        return types::Function::Error;
    }

    if (!in[0]->isDouble())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::Double* pIn = in[0]->getAs<types::Double>();

    // Input can only be a vector composed of -1s and 1s, to indicate algebraic or differential continuous state components of a block
    if (!pIn->isVector())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A vector expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    for (int i = 0; i < pIn->getSize(); ++i)
    {
        if (pIn->get(i) != -1 && pIn->get(i) != 1)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d : %d or %d expected.\n"), funname.data(), 1, -1, 1);
            return types::Function::Error;
        }
    }

    // Using a vector to take advantage of the converting constructor (double -> int)
    std::vector<int> pInteger(pIn->get(), pIn->get() + pIn->getSize());
    set_pointer_xproperty(&pInteger[0]);

    return types::Function::OK;
}
