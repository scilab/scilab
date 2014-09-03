/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>

#include "internal.hxx"
#include "list.hxx"
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "ScsAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

} /* namespace */

template<> property<ScsAdapter>::props_t property<ScsAdapter>::fields = property<ScsAdapter>::props_t();

ScsAdapter::ScsAdapter(const ScsAdapter& o) :
    BaseAdapter<ScsAdapter, org_scilab_modules_scicos::model::Diagram>(o) {}

ScsAdapter::ScsAdapter(org_scilab_modules_scicos::model::Diagram* o) :
    BaseAdapter<ScsAdapter, org_scilab_modules_scicos::model::Diagram>(o)
{
    if (property<ScsAdapter>::properties_have_not_been_set())
    {
        // FIXME: add some properties
    }
}

ScsAdapter::~ScsAdapter()
{
}

std::wstring ScsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring ScsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
