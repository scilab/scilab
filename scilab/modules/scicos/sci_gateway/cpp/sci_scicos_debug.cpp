/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
*  Copyright (C) INRIA - Allan CORNET
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/

#include <string>
#include <cmath>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "scicos-def.h"
#include "localization.h"
#include "Scierror.h"
    /*--------------------------------------------------------------------------*/
    // Variable defined in scicos.c
    extern COSDEBUG_struct C2F(cosdebug);
    /*--------------------------------------------------------------------------*/
}

static const std::string funname = "scicos_debug";

types::Function::ReturnValue sci_scicos_debug(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), funname.c_str(), 0, 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }

    if (in.empty())
    {
        types::Double* ret = new types::Double(C2F(cosdebug).cosd);
        out.push_back(ret);
    }
    else
    {
        if (!in[0]->isDouble())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 1);
            return types::Function::Error;
        }
        types::Double* pIn = in[0]->getAs<types::Double>();

        if (!pIn->isScalar())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A real scalar expected.\n"), funname.data(), 1);
            return types::Function::Error;
        }
        if (pIn->get(0) != floor(pIn->get(0)))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d : An integer value expected.\n"), funname.data(), 1);
            return types::Function::Error;
        }

        C2F(cosdebug).cosd = pIn->get(0);
    }

    return types::Function::OK;
}
