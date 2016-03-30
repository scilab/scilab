/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
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

/*--------------------------------------------------------------------------*/
#include "XcosCellFactory.hxx"
#include "view_scilab/Adapters.hxx"

#include "types.hxx"
#include "int.hxx"
#include "function.hxx"
#include "gw_xcos.hxx"

extern "C" {
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_scicos;
using namespace org_scilab_modules_xcos_graph_model;
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_xcosCellCreated(types::typed_list &in, int /*_iRetCount*/, types::typed_list &/*out*/)
{
    if (in.size() == 1 && in[0]->isUserType())
    {
        const model::BaseObject* o = view_scilab::Adapters::instance().descriptor(in[0]);
        if (o == nullptr || (o->kind() != DIAGRAM && o->kind() != BLOCK && o->kind() != ANNOTATION))
        {
            Scierror(77, _("%s: Wrong type for input argument #%d: ""%s"" expected.\n"), "xcosCellCreated", 1, "diagram or block");
            return types::Function::Error;
        }

        XcosCellFactory::created(getScilabJavaVM(), o->id(), static_cast<int>(o->kind()));
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

