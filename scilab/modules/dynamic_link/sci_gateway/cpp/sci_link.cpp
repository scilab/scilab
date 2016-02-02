/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
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

/*-----------------------------------------------------------------------------------*/
#include "dynamic_link_gw.hxx"
#include "configvariable.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "dynamic_link.h"
#include "ilib_verbose.h"
#include "sci_malloc.h"
#include "localization.h"
#include "dl_genErrorMessage.h"
#include "freeArrayOfString.h"
#include "sciprint.h"
}

void displayDynLibInfo(void);
types::Double* getLibraryIDs(void);
/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_link(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iSizeSubNames       = 0;
    wchar_t** pwstSubNames  = NULL;
    wchar_t* pwstLibName    = NULL;
    BOOL bFortran           = TRUE;
    int iIDSharedLib        = -1;

    if (in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "c_link", 0, 3);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        std::vector<std::wstring> FunctionsList = ConfigVariable::getEntryPointNameList();
        if (FunctionsList.size() == 0)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        types::String* pSFunctionNames = new types::String(1, (int)FunctionsList.size());
        for (int i = 0 ; i < FunctionsList.size(); i++)
        {
            pSFunctionNames->set(FunctionsList.size() - i - 1, FunctionsList[i].c_str());
        }

        out.push_back(pSFunctionNames);
        return types::Function::OK;
    }

    if (in.size() == 3)
    {
        //flag
        if (in[2]->isString() == false || in[2]->getAs<types::String>()->isScalar() == false)
        {
            Scierror(999 , _("%s : Wrong type for input argument #%d: string expected.\n"), "link", 3);
            return types::Function::Error;
        }

        types::String* pSFlag = in[2]->getAs<types::String>();
        wchar_t* pwstFlag = pSFlag->get(0);
        if (wcscmp(pwstFlag, L"f") == 0 || wcscmp(pwstFlag, L"c") == 0)
        {
            if (wcscmp(pwstFlag, L"c") == 0)
            {
                bFortran = FALSE;
            }
        }
        else
        {
            Scierror(999, _("%ls Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "link", 3, "f", "c");
            return types::Function::Error;
        }

    }

    if (in.size() >= 2)
    {
        //sub names
        if (in[1]->isString() == false || ( in[1]->getAs<types::String>()->isVector() == false && in[1]->getAs<types::String>()->isScalar() == false))
        {
            Scierror(999, _("%s Wrong type for input argument #%d: string or string vector expected.\n"), "link", 2);
            return types::Function::Error;
        }

        types::String* pSSubNames = in[1]->getAs<types::String>();
        iSizeSubNames = pSSubNames->getSize();
        pwstSubNames = pSSubNames->get();
    }

    if (in.size() >= 1)
    {
        if (in[0]->isDouble())
        {
            types::Double* pD = in[0]->getAs<types::Double>();
            if (pD->isScalar() == false)
            {
                Scierror(999, _("%s : Wrong value for argument #%d: %s\n"), "link", 1, _("Unique id of a shared library expected."));
                return types::Function::Error;
            }

            iIDSharedLib = (int)pD->get(0);
        }
        else if (in[0]->isString())
        {
            types::String* pS = in[0]->getAs<types::String>();
            if (pS->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "link", 1);
                return types::Function::Error;
            }

            if (wcscmp(pS->get(0), L"show") == 0)
            {
                //show option
                displayDynLibInfo();
                out.push_back(getLibraryIDs());
                return types::Function::OK;
            }

            //library name
            pwstLibName = pS->get(0);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"), "link", 1);
            return types::Function::Error;
        }
    }

    int iErr    = 0;
    int iRetID  = scilabLink(iIDSharedLib, pwstLibName, pwstSubNames, iSizeSubNames, bFortran, &iErr);

    if (iErr)
    {
        dl_genErrorMessage(L"link", iErr, pwstLibName);

        /* release lib if it is a new link */
        if ((iIDSharedLib == -1) && (iRetID != -1))
        {
            ConfigVariable::removeDynamicLibrary(iRetID);
        }
        return types::Function::Error;
    }

    out.push_back(new types::Double(iRetID));
    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/
void displayDynLibInfo(void)
{
    std::list<ConfigVariable::EntryPointStr*>* pEPList = ConfigVariable::getEntryPointList();
    std::vector<ConfigVariable::DynamicLibraryStr*>* pDLList = ConfigVariable::getDynamicLibraryList();

    if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
    {
        sciprint(_("Number of entry points %d.\nShared libraries :\n"), pEPList->size());
    }

    if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
    {
        sciprint("[ ");
    }

    int iLibCount = 0;
    for (int i = 0 ; i < pDLList->size() ; i++)
    {
        if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
        {
            if ((*pDLList)[i] != NULL)
            {
                sciprint("%d ", i);
                iLibCount++;
            }
        }
    }

    if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
    {
        if (iLibCount < 2)
        {
            sciprint(_("] : %d library.\n"), iLibCount);
        }
        else
        {
            sciprint(_("] : %d libraries.\n"), iLibCount);
        }
    }

    std::list<ConfigVariable::EntryPointStr*>::const_reverse_iterator it;
    for (it = pEPList->rbegin() ; it != pEPList->rend() ; it++)
    {
        if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
        {
            sciprint(_("Entry point %ls in shared library %d.\n"), (*it)->pwstEntryPointName, (*it)->iLibIndex);
        }
    }
}
/*-----------------------------------------------------------------------------------*/
types::Double* getLibraryIDs(void)
{
    std::vector<ConfigVariable::DynamicLibraryStr*>* pDLList = ConfigVariable::getDynamicLibraryList();

    int iLibCount = 0;
    for (int i = 0 ; i < pDLList->size() ; i++)
    {
        if ((*pDLList)[i] != NULL)
        {
            iLibCount++;
        }
    }

    if (iLibCount == 0)
    {
        return types::Double::Empty();
    }

    types::Double* pOut = new types::Double(1, iLibCount);
    iLibCount = 0;
    for (int i = 0 ; i < pDLList->size() ; i++)
    {
        if ((*pDLList)[i] != NULL)
        {
            pOut->set(iLibCount++, (double)i);
        }
    }
    return pOut;
}
/*-----------------------------------------------------------------------------------*/
