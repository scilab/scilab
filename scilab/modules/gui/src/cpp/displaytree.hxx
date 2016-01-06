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

#include "dynlib_gui.h"
#include "list.hxx"

#define TREE_REF_NAME "uitree"

static const std::string Label("label");
static const std::string Icon("icon");
static const std::string Callback("callback");

GUI_IMPEXP bool parseListItem(types::List* pIn, int icountItem, std::vector<std::string> &_pvStructList, const std::string& _szLevel);

#endif /* __DISPLAYTREE__ */
