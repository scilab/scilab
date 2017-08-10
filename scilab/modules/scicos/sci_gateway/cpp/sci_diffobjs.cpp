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
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
/* Testing if two Scilab objects are equal (same types, sizes and values).
*
* bool = diffobjs(o1, o2)
*
* rhs 1 o1   : Scilab object
* rhs 2 o2   : Scilab object
* lhs 1 bool : double worth 0 if the objects are identical, 1 otherwise
*/
/*--------------------------------------------------------------------------*/

static const std::string funname = "diffobjs";

types::Function::ReturnValue sci_diffobjs(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    types::Double* ret = new types::Double(1);

    if (*in[0] == *in[1])
    {
        ret->set(0, 0);
    }

    out.push_back(ret);
    return types::Function::OK;
}
