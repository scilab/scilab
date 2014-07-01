/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ModelAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{



template<> property<ModelAdapter>::props_t property<ModelAdapter>::fields = property<ModelAdapter>::props_t();

ModelAdapter::ModelAdapter(const ModelAdapter& o) :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(o) { }

ModelAdapter::ModelAdapter(org_scilab_modules_scicos::model::Block* o) :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(o)
{
}

ModelAdapter::~ModelAdapter()
{
}

bool ModelAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"ModelAdapter.hxx: Dunno what to display there" << std::endl;
    return true;
}

std::wstring ModelAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring ModelAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
