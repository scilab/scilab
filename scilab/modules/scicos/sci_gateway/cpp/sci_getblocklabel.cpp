/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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
#include "internal.hxx"
#include "double.hxx"
#include "string.hxx"
#include "function.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "import.h" /* getscilabel() */
#include "scicos-def.h"

#include "localization.h"
#include "Scierror.h"

    /*--------------------------------------------------------------------------*/
    // Variable defined in scicos.c
    extern CURBLK_struct C2F(curblk);
    /*--------------------------------------------------------------------------*/
}

static const std::string funname = "getblocklabel";

types::Function::ReturnValue sci_getblocklabel(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), funname.data(), 0, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    // Pick the current simulation block if the user didn't pass a block number
    int blockNumber;
    if (in.size() == 0)
    {
        blockNumber = C2F(curblk).kfun;
    }
    else
    {
        if (!in[0]->isDouble())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 1);
            return types::Function::Error;
        }
        types::Double* BlockNumber = in[0]->getAs<types::Double>();
        if (BlockNumber->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 1);
            return types::Function::Error;
        }

        if (!BlockNumber->isScalar())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), funname.data(), 1);
            return types::Function::Error;
        }
        blockNumber = static_cast<int>(BlockNumber->get(0));
    }

    int labelSize;
    char label[100];
    int ierr = getscilabel(&blockNumber, label, &labelSize);
    if (ierr != 0)
    {
        Scierror(999, _("%s: scicosim is not running.\n"), funname.data());
        return types::Function::Error;
    }
    label[labelSize] = '\0';

    types::String* Label = new types::String(label);

    out.push_back(Label);
    return types::Function::OK;
}
