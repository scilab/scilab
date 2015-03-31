/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#include <vector>
#include <string>

#include "displaytree.hxx"

#include "internal.hxx"
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "struct.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h" /* wide_string_to_UTF8 */
}

bool bParseListItem(types::List* pIn, int _iItemCount, std::vector<std::string> &_pvStructList, std::string _szLevel)
{
    char* cstr; // Buffer

    std::string szCurLvl = "";

    // Parse item
    for (int i = 2; i < _iItemCount; ++i) // Look for tlists in the passed list pIn
    {
        if (!pIn->get(i)->isTList()) // Potential tree
        {
            // Go up, it is finished for this node
            return true;
        }
        types::TList* tlist = pIn->get(i)->getAs<types::TList>();

        // Retrieve next item
        int iItemCount = tlist->getSize();
        if (iItemCount < 2)
        {
            return false;
        }

        // Get first element as a string
        if (!tlist->get(0)->isString())
        {
            return false;
        }
        types::String* strItem1 = tlist->get(0)->getAs<types::String>();
        if (strItem1->getSize() < 1)
        {
            return false;
        }

        // Check tree structure
        cstr = wide_string_to_UTF8(strItem1->get(0));
        if (strcmp(cstr, TREE_REF_NAME) != 0)
        {
            FREE(cstr);
            return false;
        }
        FREE(cstr);

        // Get the second element as a struct
        if (!tlist->get(1)->isStruct())
        {
            return false;
        }
        types::Struct* node = tlist->get(1)->getAs<types::Struct>();
        types::String* fields = node->get(0)->getFieldNames();
        if (fields->getSize() < 3)
        {
            return false;
        }

        // Add node level
        szCurLvl = _szLevel;
        szCurLvl += ".";
        std::ostringstream out;
        out << i - 1;
        szCurLvl += out.str();

        _pvStructList.push_back(szCurLvl);

        types::InternalType* temp;

        // Get label name
        temp = node->get(0)->get(Label);
        if (!temp->isString())
        {
            return false;
        }
        types::String* strLabel = temp->getAs<types::String>();
        if (strLabel->getSize() != 1)
        {
            return false;
        }
        cstr = wide_string_to_UTF8(strLabel->get(0));
        _pvStructList.push_back(std::string(cstr));
        FREE(cstr);

        // Get icon name
        temp = node->get(0)->get(Icon);
        if (!temp->isString())
        {
            return false;
        }
        types::String* strIcon = temp->getAs<types::String>();
        if (strIcon->getSize() != 1)
        {
            return false;
        }
        cstr = wide_string_to_UTF8(strIcon->get(0));
        _pvStructList.push_back(std::string(cstr));
        FREE(cstr);

        // Get callback name
        temp = node->get(0)->get(Callback);
        if (!temp->isString())
        {
            return false;
        }
        types::String* strCallback = temp->getAs<types::String>();
        if (strCallback->getSize() != 1)
        {
            return false;
        }
        cstr = wide_string_to_UTF8(strCallback->get(0));
        _pvStructList.push_back(std::string(cstr));
        FREE(cstr);

        bParseListItem(tlist, iItemCount, _pvStructList, szCurLvl);
    }

    return true;
}
