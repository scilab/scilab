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

#include "ParamsAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

} /* namespace */

template<> property<ParamsAdapter>::props_t property<ParamsAdapter>::fields = property<ParamsAdapter>::props_t();

ParamsAdapter::ParamsAdapter(const ParamsAdapter& o) :
    BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>(o) {}

ParamsAdapter::ParamsAdapter(org_scilab_modules_scicos::model::Diagram* o) :
    BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>(o)
{
    if (property<ParamsAdapter>::properties_has_not_been_set())
    {
        // FIXME: add some properties
    }
}

ParamsAdapter::~ParamsAdapter()
{
}

std::wstring ParamsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring ParamsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
