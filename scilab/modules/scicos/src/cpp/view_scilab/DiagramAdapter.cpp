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
#include <vector>
#include <sstream>
#include <memory>

#include "internal.hxx"
#include "double.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "DiagramAdapter.hxx"
#include "Adapters.hxx"
#include "ParamsAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
#include "TextAdapter.hxx"
#include "model/BaseObject.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

struct props
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(adaptor.getAdaptee());
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(adaptor.getAdaptee());
        return localAdaptor.setAsTList(v, controller);
    }
};

struct objs
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        return adaptor.getListObjects();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // Decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee().get();

        types::List* list = v->getAs<types::List>();

        // Clear the children list before the loop to reset the diagram children
        // and clear the old Links information
        std::vector<ScicosID> diagramChildren;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
        for (ScicosID id : diagramChildren)
        {
            auto o = controller.getObject(id);
            controller.setObjectProperty(id, o->kind(), PARENT_DIAGRAM, 0ll);
        }
        diagramChildren.clear();
        adaptor.getFrom().clear();
        adaptor.getTo().clear();

        // Set the children to the right IDs
        std::vector<LinkAdapter*> linkListView;
        for (int i = 0; i < list->getSize(); ++i)
        {
            if (list->get(i)->getType() != types::InternalType::ScilabUserType)
            {
                return false;
            }

            const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(list->get(i)->getShortTypeStr());

            // Then, each adapter gets linked to the diagram through its adaptee (PARENT_DIAGRAM)
            // and the diagram's adaptee lists its adaptees (CHILDREN).
            ScicosID id;
            switch (adapter_index)
            {
                case Adapters::BLOCK_ADAPTER:
                {
                    BlockAdapter* modelElement = list->get(i)->getAs<BlockAdapter>();

                    id = modelElement->getAdaptee()->id();

                    controller.setObjectProperty(id, BLOCK, PARENT_DIAGRAM, adaptee->id());
                    diagramChildren.push_back(id);
                    break;
                }
                case Adapters::LINK_ADAPTER:
                {
                    LinkAdapter* modelElement = list->get(i)->getAs<LinkAdapter>();

                    id = modelElement->getAdaptee()->id();

                    controller.setObjectProperty(id, LINK, PARENT_DIAGRAM, adaptee->id());

                    // Hold Links information, to try the linking at model-level once all the elements have been added to the Diagram
                    linkListView.push_back(modelElement);

                    diagramChildren.push_back(id);
                    break;
                }
                case Adapters::TEXT_ADAPTER:
                {
                    TextAdapter* modelElement = list->get(i)->getAs<TextAdapter>();

                    id = modelElement->getAdaptee()->id();

                    controller.setObjectProperty(id, ANNOTATION, PARENT_DIAGRAM, adaptee->id());
                    diagramChildren.push_back(id);
                    break;
                }
                default:
                    return false;
            }
        }
        controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);

        adaptor.setListObjects(v);

        // Do the linking at model-level
        for (int i = 0; i < static_cast<int>(linkListView.size()); ++i)
        {
            // Trigger 'from' and 'to' properties
            link_t from_content = linkListView[i]->getFrom();
            if (!linkListView[i]->setFrom(from_content, controller))
            {
                return false;
            }
            adaptor.getFrom().push_back(from_content);
            link_t to_content = linkListView[i]->getTo();
            if (!linkListView[i]->setTo(to_content, controller))
            {
                return false;
            }
            adaptor.getTo().push_back(to_content);
        }
        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee().get();

        std::string version;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, VERSION_NUMBER, version);

        return new types::String(version.data());
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee().get();

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string version (c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee->id(), DIAGRAM, VERSION_NUMBER, version);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee().get();

            std::string version;
            controller.setObjectProperty(adaptee->id(), DIAGRAM, VERSION_NUMBER, version);
            return true;
        }

        return false;
    }
};

struct contrib
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        return adaptor.getContribContent();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) v;
        (void) controller;

        adaptor.setContribContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Diagram> adaptee) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(adaptee),
    list_objects(new types::List()),
    from_vec(),
    to_vec(),
    contrib_content(new types::List())
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }
}

DiagramAdapter::DiagramAdapter(const DiagramAdapter& adapter) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(adapter),
    list_objects(adapter.getListObjects()),
    from_vec(adapter.from_vec),
    to_vec(adapter.to_vec),
    contrib_content(adapter.getContribContent())
{
}

DiagramAdapter::~DiagramAdapter()
{
    // Unlink the diagram's children if necessary
    Controller controller;
    std::vector<ScicosID> diagramChildren;
    if (getAdaptee() != 0)
    {
        controller.getObjectProperty(getAdaptee()->id(), DIAGRAM, CHILDREN, diagramChildren);
        for (ScicosID id : diagramChildren)
        {
            auto o = controller.getObject(id);
            controller.setObjectProperty(id, o->kind(), PARENT_DIAGRAM, 0ll);
        }
    }

    list_objects->DecreaseRef();
    list_objects->killMe();

    contrib_content->DecreaseRef();
    contrib_content->killMe();
}

std::wstring DiagramAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* DiagramAdapter::getContribContent() const
{
    contrib_content->IncreaseRef();
    return contrib_content;
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    contrib_content->DecreaseRef();
    contrib_content->killMe();

    v->IncreaseRef();
    contrib_content = v;
}

types::InternalType* DiagramAdapter::getListObjects() const
{
    return list_objects;
}

void DiagramAdapter::setListObjects(types::InternalType* v)
{
    // The old 'list_objects' needs to be freed after setting it to 'v'
    types::InternalType* temp = list_objects;

    v->IncreaseRef();
    list_objects = v;

    temp->DecreaseRef();
    temp->killMe();
}

std::vector<link_t>& DiagramAdapter::getFrom()
{
    return from_vec;
}

std::vector<link_t>& DiagramAdapter::getTo()
{
    return to_vec;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
