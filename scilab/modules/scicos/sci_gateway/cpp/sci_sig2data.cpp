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
#include "internal.hxx"
#include "string.hxx"
#include "struct.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
/* [x[,t]]=sig2data(B)
*
* Extract data from a 'scicos' signal structure.
*  x will be the field B.values
*  t will be the field B.time
*
* Usage :
* -->a(:,:,4)=([7 8 9;10 11 12]);
* -->b=data2sig(a,0.1)
* b  =
*   values: [2x3x4 hypermat]
*   time: [4x1 constant]
* -->b.time
* ans  =
*    0.
*    0.1.
*    0.2.
*    0.3.
*
* -->[x,t]=sig2data(b)
* t  =
*  0.
*   0.1
*   0.2
*   0.3
* x  =
* (:,:,1)
*
*  0.    0.    0.
*  0.    0.    0.
*
* Inputs
*   B    : a scilab structure that defines a signal :
*          B.values :  B data,
*          B.time   :  the samples time in the form of
*                      a columun vector.
*
* Outputs
*   x    : the field B.values
*   t    : the field B.time
*
* initial rev 08/11/07, Alan
*
*/
/*--------------------------------------------------------------------------*/

static const std::string funname = "sig2data";

static const std::wstring Values(L"values");
static const std::wstring Time(L"time");

types::Function::ReturnValue sci_sig2data(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), funname.data(), 1, 2);
        return types::Function::Error;
    }

    if (!in[0]->isStruct())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A struct expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::Struct* B = in[0]->getAs<types::Struct>();

    if (B->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A single struct expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::String* fields = B->get(0)->getFieldNames();
    if (fields->getSize() != 2)
    {
        Scierror(999, _("%s: Wrong fields for input argument #%d : \"%s\" and \"%s\" expected.\n"), funname.data(), 1, "values", "time");
        return types::Function::Error;
    }
    if (fields->get(0) != Values)
    {
        Scierror(999, _("%s: Wrong fields for input argument #%d : \"%s\" and \"%s\" expected.\n"), funname.data(), 1, "values", "time");
        return types::Function::Error;
    }
    if (fields->get(1) != Time)
    {
        Scierror(999, _("%s: Wrong fields for input argument #%d : \"%s\" and \"%s\" expected.\n"), funname.data(), 1, "values", "time");
        return types::Function::Error;
    }

    // Values
    types::InternalType* A = B->get(0)->get(Values);
    types::InternalType* timeValues = B->get(0)->get(Time);

    out.push_back(A);
    if (_iRetCount == 2)
    {
        out.push_back(timeValues);
    }
    return types::Function::OK;
}
