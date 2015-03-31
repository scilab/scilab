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

#ifndef __DISPLAYTREE__
#define __DISPLAYTREE__

#include <vector>
#include <string>
#include <sstream>

#include "list.hxx"

using namespace std;

#define TREE_REF_NAME "uitree"

static const std::wstring Label(L"label");
static const std::wstring Icon(L"icon");
static const std::wstring Callback(L"callback");

bool bParseListItem(types::List* pIn, int icountItem, std::vector<std::string> &_pvStructList, std::string _szLevel);

#endif /* __DISPLAYTREE__ */
