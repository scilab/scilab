/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2016-2016 - Clément DAVID
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
#include "Controller.hxx"
#include "view_scilab/Adapters.hxx"
#include "XMIResource.hxx"

#include "types.hxx"
#include "function.hxx"
#include "string.hxx"
#include "user.hxx"
#include "int.hxx"

#include "gw_scicos.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_scicos;
/*--------------------------------------------------------------------------*/
static char funname[] = "scicosDiagramToScilab";
types::InternalType* importFile(char const* file);
bool exportFile(int index, char const* file, types::InternalType* uid);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_scicosDiagramToScilab(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    /*
     * to import some files
     * scs_m = scicosDiagramToScilab("/path/to/a/file")
     * [scs_m1, scs_m2] = scicosDiagramToScilab(["/path/to/a/file", "/path/to/files"])
     *
     * to export a file
     * scicosDiagramToScilab("/path/to/a/file", scs_m)
     * scicosDiagramToScilab(["/path/to/a/file", "/path/to/files"], scs_m1, scs_m2)
     */

    if (in.size() < 1)
    {
        Scierror(77, _("%s: Wrong number of input arguments: at least %d expected.\n"), funname, 1);
        return types::Function::Error;
    }

    if (!in[0]->isString())
    {
        Scierror(77, _("%s: Wrong type for input argument #%d: string expected.\n"), funname, 1);
        return types::Function::Error;
    }

    types::String* files = in[0]->getAs<types::String>();
    if (in.size() == 1 && files->getSize() == _iRetCount)
    {
        // the user asked to import
        out.resize(_iRetCount);
        for (int i = 0; i < _iRetCount; i++)
        {
            char* f = wide_string_to_UTF8(files->get(i));
            out[i] = importFile(f);
            FREE(f);
            if (out[i] == nullptr)
            {
                return types::Function::Error;
            }
        }
    }
    else if (in.size() == 1 + static_cast<size_t>(files->getSize()) && _iRetCount <= 1)
    {
        // the user asked to export
        for (int i = 0; i < _iRetCount; i++)
        {
            if (!in[1 + i]->isUserType())
            {
                Scierror(77, _("%s: Wrong type for input argument #%d: ""%s"" expected.\n"), funname, "diagram");
                return types::Function::Error;
            }
        }
        for (int i = 0; i < _iRetCount; i++)
        {
            char* f = wide_string_to_UTF8(files->get(i));
            bool success = exportFile(1 + i, f, in[1 + i]);
            FREE(f);
            if (!success)
            {
                return types::Function::Error;
            }
        }
    }
    else
    {
        // report a comprehensible error message
        if (in.size() == 1)
        {
            Scierror(77, _("%s: Wrong number of output arguments: %d expected.\n"), funname, files->getSize());
        }
        else if (in.size() == 1 + static_cast<size_t>(files->getSize()))
        {
            Scierror(77, _("%s: Wrong number of output arguments: %d expected.\n"), funname, 0);
        }
        else
        {
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), funname, 1);
        }
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
types::InternalType* importFile(char const* file)
{
    // create a diagram
    org_scilab_modules_scicos::Controller controller;
    ScicosID uid = controller.createObject(DIAGRAM);

    // load it
    if (XMIResource(uid).load(file) != 0)
    {
        Scierror(999, _("%s: Unable to load \"%s\" .\n"), funname, file);
        return nullptr;
    }

    return view_scilab::Adapters::instance().allocate_view(uid, DIAGRAM);
}
/*--------------------------------------------------------------------------*/
bool exportFile(int index, char const* file, types::InternalType* type)
{
    // check that the passed argument is a diagram
    const model::BaseObject* o = view_scilab::Adapters::instance().descriptor(type);
    if (o == nullptr || o->kind() != DIAGRAM)
    {
        Scierror(77, _("%s: Wrong type for input argument #%d: ""%s"" expected.\n"), funname, index, "diagram");
        return false;
    }

    if (XMIResource(o->id()).save(file) < 0)
    {
        Scierror(999, _("%s: Unable to save \"%s\" .\n"), funname, file);
        return false;

    }

    return true;
}
/*--------------------------------------------------------------------------*/
