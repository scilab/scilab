/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "ui_data_gw.hxx"
#include "ui_data.h"
#include "function.hxx"
#include "EditVar.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "Scierror.h"
}

using namespace org_scilab_modules_ui_data;

static const std::string fname("closeEditvar");
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_closeEditvar(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname.data(), 0);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    EditVar::closeVariableEditor(getScilabJavaVM());

    return types::Function::OK;
}
