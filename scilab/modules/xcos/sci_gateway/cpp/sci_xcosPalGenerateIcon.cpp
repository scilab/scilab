/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
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

#include "gw_xcos.hxx"
#include "Palette.hxx"
#include "model/BaseObject.hxx"
#include "view_scilab/Adapters.hxx"

#include "types.hxx"
#include "function.hxx"
#include "string.hxx"
#include "UTF8.hxx"

#include "GiwsException.hxx"
#include "xcosUtilities.hxx"
#include "loadStatus.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos_palette;
using namespace org_scilab_modules_scicos;

static char funname[] = "xcosPalGenerateIcon";

types::Function::ReturnValue sci_xcosPalGenerateIcon(types::typed_list &in, int _iRetCount, types::typed_list &/*out*/)
{
    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), funname, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), funname, 1);
        return types::Function::Error;
    }

    const model::BaseObject* o = view_scilab::Adapters::instance().descriptor(in[0]);
    if (o == nullptr || o->kind() != BLOCK)
    {
        Scierror(999, _("%s: Argument #%d: ""%s"" expected.\n"), funname, 1, "Block");
        return types::Function::Error;
    }

    if (!in[1]->isString())
    {
        Scierror(999, _("%s: Argument #%d: ""%s"" expected.\n"), funname, 2, "String");
        return types::Function::Error;
    }
    types::String* path = in[1]->getAs<types::String>();
    if (path->getSize() != 1)
    {
        Scierror(999, _("%s: Argument #%d: Scalar (1 element) expected.\n"), funname, 2);
        return types::Function::Error;
    }
    std::string iconPath = scilab::UTF8::toUTF8(path->get(0));

    /* Call the java implementation */
    set_loaded_status(XCOS_CALLED);
    try
    {
        Palette::generatePaletteIcon(getScilabJavaVM(), o->id(), iconPath.c_str());
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        Scierror(999, "%s: %s\n", funname, exception.getJavaDescription().c_str());
        return types::Function::Error;
    }
    catch (GiwsException::JniException &exception)
    {
        Scierror(999, "%s: %s\n", funname, exception.whatStr().c_str());
        return types::Function::Error;
    }

    return types::Function::OK;
}

