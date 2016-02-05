/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 * Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
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

bool parseListItem(types::List* pIn, int _iItemCount, std::vector<std::string>& _pvStructList, const std::string& _szLevel)
{
    char* cstr; // Buffer

    std::string szCurLvl = "";

    // Parse item
    for (int i = 2; i < _iItemCount; ++i) // Look for tlists in the passed list pIn
    {
        if (pIn->get(i)->isTList() == false) // Potential tree
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
        if (tlist->get(0)->isString() == false)
        {
            return false;
        }
        types::String* strItem1 = tlist->get(0)->getAs<types::String>();
        if (strItem1->getSize() < 1)
        {
            return false;
        }

        // Check tree structure
        if (wcscmp(strItem1->get(0), TREE_REF_NAME) != 0)
        {
            return false;
        }

        // Get the second element as a struct
        if (tlist->get(1)->isStruct() == false)
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

        types::InternalType* temp = nullptr;

        // Get label name
        temp = node->get(0)->get(Label);
        if (temp->isString() == false)
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
        if (temp->isString() == false)
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
        if (temp->isString() == false)
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

        parseListItem(tlist, iItemCount, _pvStructList, szCurLvl);
    }

    return true;
}
