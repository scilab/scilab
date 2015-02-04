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
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "CprAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

struct dummy_property
{

    static types::InternalType* get(const CprAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        // Return a default empty matrix.
        return types::Double::Empty();
    }

    static bool set(CprAdapter& /*adaptor*/, types::InternalType* /*v*/, Controller& /*controller*/)
    {
        // Everything should be right as the properties mapped using this adapter do not perform anything
        return true;
    }
};

} /* namespace */

template<> property<CprAdapter>::props_t property<CprAdapter>::fields = property<CprAdapter>::props_t();
static void initialize_fields()
{
    if (property<CprAdapter>::properties_have_not_been_set())
    {
        property<CprAdapter>::fields.reserve(4);
        property<CprAdapter>::add_property(L"state", &dummy_property::get, &dummy_property::set);
        property<CprAdapter>::add_property(L"sim", &dummy_property::get, &dummy_property::set);
        property<CprAdapter>::add_property(L"cor", &dummy_property::get, &dummy_property::set);
        property<CprAdapter>::add_property(L"corinv", &dummy_property::get, &dummy_property::set);
    }
}

CprAdapter::CprAdapter() :
    BaseAdapter<CprAdapter, org_scilab_modules_scicos::model::Diagram>()
{
    initialize_fields();
}

CprAdapter::CprAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<CprAdapter, org_scilab_modules_scicos::model::Diagram>(c, adaptee)
{
    initialize_fields();
}

CprAdapter::~CprAdapter()
{
}

std::wstring CprAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring CprAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
