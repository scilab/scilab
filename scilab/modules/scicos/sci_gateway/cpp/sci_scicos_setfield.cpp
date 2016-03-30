/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
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

#include <string>

#include "../../includes/view_scilab/Adapters.hxx"
#include "gw_scicos.hxx"

#include "types.hxx"
#include "string.hxx"
#include "list.hxx"
#include "function.hxx"

#include "view_scilab/BaseAdapter.hxx"
#include "view_scilab/BlockAdapter.hxx"
#include "view_scilab/CprAdapter.hxx"
#include "view_scilab/DiagramAdapter.hxx"
#include "view_scilab/GraphicsAdapter.hxx"
#include "view_scilab/LinkAdapter.hxx"
#include "view_scilab/ModelAdapter.hxx"
#include "view_scilab/ParamsAdapter.hxx"
#include "view_scilab/ScsAdapter.hxx"
#include "view_scilab/StateAdapter.hxx"
#include "view_scilab/TextAdapter.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "model/Annotation.hxx"
#include "model/Block.hxx"
#include "model/Diagram.hxx"
#include "model/Link.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace org_scilab_modules_scicos;

static const std::string funame = "scicos_setfield";

template<class Adaptor, class Adaptee>
types::InternalType* set(types::InternalType* adaptor_type, const std::wstring& field, types::InternalType* value)
{
    Adaptor* adaptor = adaptor_type->getAs<Adaptor>();

    if (!adaptor->setProperty(field, value, Controller()))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: unable to set \"%ls\".\n"), funame.c_str(), 2, field.c_str());
        return adaptor_type;
    }

    return adaptor_type->clone();
}

types::Function::ReturnValue sci_scicos_setfield(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), funame.data(), 3);
        return types::Function::Error;
    }
    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    types::InternalType* field_type = in[0];
    if (field_type->getType() != types::InternalType::ScilabString)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    types::String* field_name = field_type->getAs<types::String>();
    if (field_name->getSize() > 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    std::wstring field = field_name->get(0);
    types::InternalType* value = in[1];
    types::InternalType* adaptor = in[2];

    /*
     * allocate the right adapter then try to set fields values
     */

    const view_scilab::Adapters::adapters_index_t adapter_index = view_scilab::Adapters::instance().lookup_by_typename(adaptor->getShortTypeStr());
    types::InternalType* returnType;
    switch (adapter_index)
    {
        case view_scilab::Adapters::BLOCK_ADAPTER:
            returnType = set<view_scilab::BlockAdapter, model::Block>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::CPR_ADAPTER:
            returnType = set<view_scilab::CprAdapter, model::Diagram>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::DIAGRAM_ADAPTER:
            returnType = set<view_scilab::DiagramAdapter, model::Diagram>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::GRAPHIC_ADAPTER:
            returnType = set<view_scilab::GraphicsAdapter, model::Block>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::LINK_ADAPTER:
            returnType = set<view_scilab::LinkAdapter, model::Link>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::MODEL_ADAPTER:
            returnType = set<view_scilab::ModelAdapter, model::Block>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::PARAMS_ADAPTER:
            returnType = set<view_scilab::ParamsAdapter, model::Diagram>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::SCS_ADAPTER:
            returnType = set<view_scilab::ScsAdapter, model::Diagram>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::STATE_ADAPTER:
            returnType = set<view_scilab::StateAdapter, model::Diagram>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::TEXT_ADAPTER:
            returnType = set<view_scilab::TextAdapter, model::Annotation>(adaptor, field, value);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        default:
            Scierror(999, _("%s: Wrong value for input argument #%d:  \"%ls\" type is not managed.\n"), funame.data(), 2, adaptor->getTypeStr().c_str());
            return types::Function::Error;
            break;
    }

    return types::Function::OK;
}
