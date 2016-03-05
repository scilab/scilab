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

#ifndef __DISPLAYTREE__
#define __DISPLAYTREE__

#include <vector>
#include <string>
#include <sstream>

#include "dynlib_gui.h"
#include "list.hxx"

#define TREE_REF_NAME L"uitree"

static const std::wstring Label(L"label");
static const std::wstring Icon(L"icon");
static const std::wstring Callback(L"callback");

GUI_IMPEXP bool parseListItem(types::List* pIn, int icountItem, std::vector<std::string> &_pvStructList, const std::string& _szLevel);

#endif /* __DISPLAYTREE__ */
