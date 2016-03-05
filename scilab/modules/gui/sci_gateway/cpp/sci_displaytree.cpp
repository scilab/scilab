/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
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

#include <cstring>
#include <string>
#include <vector>

#include "gui_gw.hxx"

#include "displaytree.hxx"
#include "ScilabDisplayTree.hxx"
#include "GiwsException.hxx"

#include "internal.hxx"
#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "struct.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "sci_malloc.h"
#include "charEncoding.h" /* wide_string_to_UTF8 */

#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
}

static const std::string funname = "uiDisplayTree";

types::Function::ReturnValue sci_displaytree(types::typed_list &in, int _iRetCount, types::typed_list& /*out*/)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s:  Wrong number of output argument(s): %d to %d expected."), "helpbrowser", 0, 1);
        return types::Function::Error;
    }

    if (in[0]->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A list expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    types::List* pIn = in[0]->getAs<types::List>();
    int iItemCount = pIn->getSize();
    if (iItemCount < 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : At least %d elements expected.\n"), funname.data(), 1, 2);
        return types::Function::Error;
    }

    // Get first element as a string
    if (pIn->get(0)->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of input argument #%d : string expected.\n"), funname.data(), 1, 1);
        return types::Function::Error;
    }

    types::String* strItem1 = pIn->get(0)->getAs<types::String>();
    if (strItem1->getSize() < 1)
    {
        Scierror(999, _("%s: Wrong size for element #%d of input argument #%d : At least %d element expected.\n"), funname.data(), 1, 1, 1);
        return types::Function::Error;
    }

    // Check tree structure
    if (wcscmp(strItem1->get(0), TREE_REF_NAME) != 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Tree expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    // Get the second element as a struct
    if (pIn->get(1)->isStruct() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of input argument #%d : A struct expected.\n"), funname.data(), 2, 1);
        return types::Function::Error;
    }

    types::Struct* node = pIn->get(1)->getAs<types::Struct>();
    types::String* fields = node->get(0)->getFieldNames();
    if (fields->getSize() < 3)
    {
        Scierror(999, _("%s: Wrong size for element #%d of input argument #%d : At least %d element expected.\n"), funname.data(), 2, 1, 3);
        return types::Function::Error;
    }

    if (fields->get(0) != Label || fields->get(1) != Icon || fields->get(2) != Callback)
    {
        Scierror(999, _("%s: Wrong fields for element #%d of input argument #%d : \"%s\" and \"%s\" expected.\n"), funname.data(), 2, 1, "label", "icon", "callback");
        return types::Function::Error;
    }

    std::string szCurLevel = "";
    std::vector<std::string> StructList;

    // Add node level
    szCurLevel += "1";
    StructList.push_back(szCurLevel);

    types::InternalType* temp = nullptr;

    // Get label name
    temp = node->get(0)->get(Label);
    if (temp->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of element #%d of input argument #%d : string expected.\n"), funname.data(), 1, 2, 1);
        return types::Function::Error;
    }

    types::String* strLabel = temp->getAs<types::String>();
    if (strLabel->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for element #%d of element #%d of input argument #%d : A single string expected.\n"), funname.data(), 1, 2, 1);
        return types::Function::Error;
    }

    char* cstr = wide_string_to_UTF8(strLabel->get(0));
    StructList.push_back(std::string(cstr));
    FREE(cstr);

    // Get icon name
    temp = node->get(0)->get(Icon);
    if (temp->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of element #%d of input argument #%d : string expected.\n"), funname.data(), 2, 2, 1);
        return types::Function::Error;
    }

    types::String* strIcon = temp->getAs<types::String>();
    if (strIcon->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for element #%d of element #%d of input argument #%d : A single string expected.\n"), funname.data(), 2, 2, 1);
        return types::Function::Error;
    }

    cstr = wide_string_to_UTF8(strIcon->get(0));
    StructList.push_back(std::string(cstr));
    FREE(cstr);

    // Get callback name
    temp = node->get(0)->get(Callback);
    if (temp->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of element #%d of input argument #%d : string expected.\n"), funname.data(), 3, 2, 1);
        return types::Function::Error;
    }

    types::String* strCallback = temp->getAs<types::String>();
    if (strCallback->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for element #%d of element #%d of input argument #%d : A single string expected.\n"), funname.data(), 3, 2, 1);
        return types::Function::Error;
    }

    cstr = wide_string_to_UTF8(strCallback->get(0));
    StructList.push_back(std::string(cstr));
    FREE(cstr);

    if (parseListItem(pIn, iItemCount, StructList, szCurLevel) == false)
    {
        Scierror(999, _("%s: Error in the tree parsing.\n"), funname.data());
        return types::Function::Error;
    }

    // Convert vector<string> to char **
    int struct_size = static_cast<int>(StructList.size());
    const char** tab = new const char *[struct_size];

    for (int i = 0; i < struct_size; ++i)
    {
        tab[i] = StructList.at(i).c_str();
    }

    try
    {
        // Java
        org_scilab_modules_gui_tree::ScilabDisplayTree::scilabDisplayTree(getScilabJavaVM(), tab, struct_size);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception has arisen:\n%s"), funname.data(), e.whatStr().c_str());
        return types::Function::Error;
    }

    delete[] tab;
    return types::Function::OK;
}
