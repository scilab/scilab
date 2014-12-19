/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Paul Bignier
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef RECURSIVE_CLONING_HXX_
#define RECURSIVE_CLONING_HXX_

#include <vector>

#include "types.hxx"
#include "list.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

/*
 * Recursively create a List of Adapters for all objects contained in 'diagramChildren'.
 * At each call, do the linking at model-level
 */
types::List* deepCreateAdapters(const std::vector<ScicosID>& diagramChildren, types::List* oldList_objects, const std::vector<link_t>& from, const std::vector<link_t>& to, Controller controller);

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* RECURSIVE_CLONING_HXX_ */
