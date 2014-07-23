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

#include <string>
#include <algorithm>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "string.hxx"
#include "list.hxx"
#include "function.hxx"

#include "view_scilab/Adapters.hxx"
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

static const std::string funame = "scicos_new";

template<class Adaptor, class Adaptee>
Adaptor* alloc_and_set(kind_t k, types::String* type_name, types::typed_list &in)
{
    Controller* controller = Controller::get_instance();
    ScicosID o = controller->createObject(k);
    Adaptor* adaptor = new Adaptor(static_cast<Adaptee*>(controller->getObject(o)));

    // the first header entry is the type
    for (int i = 1; i < in.size(); i++)
    {
        std::wstring name = type_name->get(i);
        if (!adaptor->setProperty(name, in[i]))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: unable to set \"%ls\".\n"), funame.data(), i, name.data());
            delete adaptor;
            return 0;
        }
    }

    return adaptor;
}

types::Function::ReturnValue sci_scicos_new(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }
    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    types::InternalType* type = in[0];
    if (type->getType() != types::InternalType::ScilabString)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    types::String* type_name = type->getAs<types::String>();
    if (type_name->getRows() > 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Row vector expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    if (type_name->getCols() != in.size())
    {
        Scierror(999, _("%s: Wrong number of input argument: %d expected.\n"), funame.data(), type_name->getCols());
        return types::Function::Error;
    }

    /*
     * allocate the right adapter then try to set fields values
     */

    const view_scilab::Adapters::adapters_index_t adapter_index = view_scilab::Adapters::instance().lookup_by_typename(type_name->get(0));
    types::InternalType* returnType;
    switch (adapter_index)
    {
        case view_scilab::Adapters::BLOCK_ADAPTER:
            returnType = alloc_and_set<view_scilab::BlockAdapter, model::Block>(BLOCK, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::CPR_ADAPTER:
            returnType = alloc_and_set<view_scilab::CprAdapter, model::Diagram>(DIAGRAM, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::DELETED_ADAPTER:
            returnType = new types::List();
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::DIAGRAM_ADAPTER:
            returnType = alloc_and_set<view_scilab::DiagramAdapter, model::Diagram>(DIAGRAM, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::GRAPHIC_ADAPTER:
            returnType = alloc_and_set<view_scilab::GraphicsAdapter, model::Block>(BLOCK, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::LINK_ADAPTER:
            returnType = alloc_and_set<view_scilab::LinkAdapter, model::Link>(LINK, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::MODEL_ADAPTER:
            returnType = alloc_and_set<view_scilab::ModelAdapter, model::Block>(BLOCK, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::PARAMS_ADAPTER:
            returnType = alloc_and_set<view_scilab::ParamsAdapter, model::Diagram>(DIAGRAM, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::SCS_ADAPTER:
            returnType = alloc_and_set<view_scilab::ScsAdapter, model::Diagram>(DIAGRAM, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::STATE_ADAPTER:
            returnType = alloc_and_set<view_scilab::StateAdapter, model::Diagram>(DIAGRAM, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        case view_scilab::Adapters::TEXT_ADAPTER:
            returnType = alloc_and_set<view_scilab::TextAdapter, model::Annotation>(ANNOTATION, type_name, in);
            if (returnType == 0)
            {
                return types::Function::Error;
            }
            out.push_back(returnType);
            break;
        default:
            Scierror(999, _("%s: Wrong value for input argument #%d:  \"%ls\" type is not managed.\n"), funame.data(), 1, type_name->get(0));
            return types::Function::Error;
            break;
    }

    return types::Function::OK;
}
