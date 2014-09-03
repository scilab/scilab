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

#include "Controller.hxx"
#include "TextAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

} /* namespace */

template<> property<TextAdapter>::props_t property<TextAdapter>::fields = property<TextAdapter>::props_t();

TextAdapter::TextAdapter(const TextAdapter& o) :
    BaseAdapter<TextAdapter, org_scilab_modules_scicos::model::Annotation>(o) {}

TextAdapter::TextAdapter(org_scilab_modules_scicos::model::Annotation* o) :
    BaseAdapter<TextAdapter, org_scilab_modules_scicos::model::Annotation>(o)
{
    if (property<TextAdapter>::properties_have_not_been_set())
    {
        // FIXME: add some properties
    }
}

TextAdapter::~TextAdapter()
{
}

std::wstring TextAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring TextAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
