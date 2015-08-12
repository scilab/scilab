/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#ifndef __DISPLAY_TREE__
#define __DISPLAY_TREE__

#include <vector>
#include <sstream>
#include <string>

using namespace std;

#define TREE_REF_NAME "uitree"

bool bParseListItem(void* _pvApiCtx, int *_piCurrentItem, int icountItem, std::vector<std::string> *_pvStructList, std::string _szLevel);

#endif /* __DISPLAY_TREE__ */
