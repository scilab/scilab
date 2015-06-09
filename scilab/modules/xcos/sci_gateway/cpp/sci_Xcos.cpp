/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "Xcos.hxx"
#include "loadStatus.hxx"

#include "gw_xcos.hxx"

#include "string.hxx"
#include "mlist.hxx"
#include "user.hxx"
#include "function.hxx"
#include "overload.hxx"

extern "C" {
#include "sci_malloc.h"
#include "getFullFilename.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace org_scilab_modules_xcos;

static int callXcos(char* fname, char* file, char* var);

/*--------------------------------------------------------------------------*/
static char funname[] = "xcos";

types::Function::ReturnValue sci_Xcos(types::typed_list &in, int _iRetCount, types::typed_list &/*out*/)
{
    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), funname, 0, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname, 1);
        return types::Function::Error;
    }

    /*
     * xcos() call
     */
    if (in.empty())
    {
        callXcos(funname, nullptr, nullptr);
        return types::Function::OK;
    }

    /*
     * xcos("file.zcos") or xcos(["file.zcos" "foo.zcos"]) call
     */
    if (in.size() == 1 && in[0]->isString())
    {
        types::String* arg1 = in[0]->getAs<types::String>();

        for (int i = 0; i < arg1->getSize(); ++i)
        {
            char* c_str = wide_string_to_UTF8(arg1->get(0));
            char* file = getFullFilename(c_str);
            FREE(c_str);
            if (file == nullptr)
            {
                return types::Function::Error;
            }
            if (callXcos(funname, file, nullptr))
            {
                return types::Function::Error;
            }
        }

        return types::Function::OK;
    }

    /*
     * xcos(scs_m) call
     */
    if (in.size() == 1 && (in[0]->isUserType() || in[0]->isMList())) // Kept MList for compatibility with Scilab 5
    {
        if (in[0]->getShortTypeStr() != L"diagram")
        {
            Scierror(77, _("%s: Wrong type for input argument #%d: ""%s"" expected.\n"), funname, 1, "diagram");
            return types::Function::Error;
        }

        // overloaded by %diagram_xcos.sci
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_xcos";
        types::typed_list out;
        types::Function::ReturnValue ret = Overload::call(wstFuncName, in, _iRetCount, out);

        return types::Function::OK;
    }

    /*
     * xcos(scs_m, "scs_m") call (usually from the overload macro)
     */
    if (in.size() == 2 && in[0]->isMList()) // For compatibility with Scilab 5
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), funname, 2);
            return types::Function::Error;
        }

        types::String* arg2 = in[1]->getAs<types::String>();
        if (arg2->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A single string expected.\n"), funname, 2);
            return types::Function::Error;
        }

        char* c_str = wide_string_to_UTF8(arg2->get(0));
        int ret = callXcos(funname, nullptr, c_str);
        FREE(c_str);
        if (ret == 1)
        {
            return types::Function::Error;
        }
        else
        {
            return types::Function::OK;
        }
    }
    if (in.size() == 2 && in[0]->isUserType())
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), funname, 2);
            return types::Function::Error;
        }

        types::String* arg2 = in[1]->getAs<types::String>();
        if (arg2->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A single string expected.\n"), funname, 2);
            return types::Function::Error;
        }

        char* c_str = wide_string_to_UTF8(arg2->get(0));
        int ret = callXcos(funname, nullptr, c_str);
        FREE(c_str);
        if (ret == 1)
        {
            return types::Function::Error;
        }
        else
        {
            return types::Function::OK;
        }
    }

    /*
     * If not returned yet, display the error message.
     */
    Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), funname, 1);
    return types::Function::Error;
}
/*--------------------------------------------------------------------------*/

static int callXcos(char *fname, char* file, char* var)
{
    set_loaded_status(XCOS_CALLED);

    try
    {
        Xcos::xcos(getScilabJavaVM(), file, var);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        std::cerr << exception.getJavaExceptionName() << std::endl;
        std::cerr << exception.getJavaDescription() << std::endl;
        std::cerr << exception.getJavaStackTrace() << std::endl;

        Scierror(999, "%s: %s\n", fname,
                 exception.getJavaDescription().c_str());

        if (file)
        {
            FREE(file);
        }
        if (var)
        {
            FREE(var);
        }
        return 1;
    }
    catch (GiwsException::JniException &exception)
    {
        std::cerr << exception.getJavaExceptionName() << std::endl;
        std::cerr << exception.getJavaDescription() << std::endl;
        std::cerr << exception.getJavaStackTrace() << std::endl;

        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());

        if (file)
        {
            FREE(file);
        }
        if (var)
        {
            FREE(var);
        }
        return 1;
    }

    if (file)
    {
        FREE(file);
    }
    if (var)
    {
        FREE(var);
    }
    return 0;
}
