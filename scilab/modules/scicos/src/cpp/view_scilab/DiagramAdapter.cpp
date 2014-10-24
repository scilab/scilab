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
#include "TextAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
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
        // FIXME: get all children of the Diagram and return them as a list
        model::Diagram* adaptee = adaptor.getAdaptee().get();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, children);

        types::List* o = new types::List();

        int link_number = 0;
        Controller newController = Controller();
        for (int i = 0; i < static_cast<int>(children.size()); ++i)
        {
            model::BaseObject* item = newController.getObject(children[i]).get();
            switch (item->kind())
            {
                case ANNOTATION:
                {
                    model::Annotation* annotation = static_cast<model::Annotation*>(item);
                    TextAdapter* localAdaptor = new TextAdapter(std::shared_ptr<model::Annotation>(annotation));
                    o->set(i, localAdaptor);
                    continue;
                }
                case BLOCK:
                {
                    model::Block* block = static_cast<model::Block*>(item);
                    BlockAdapter* localAdaptor = new BlockAdapter(std::shared_ptr<model::Block>(block));
                    o->set(i, localAdaptor);
                    continue;
                }
                case LINK:
                {
                    model::Link* link = static_cast<model::Link*>(item);
                    LinkAdapter* localAdaptor = new LinkAdapter(std::shared_ptr<model::Link>(link));

                    // In case a Link points to a Block that has not been added yet,
                    // retrieve the 'from' and 'to' values from the Diagram Adapter if they have been saved,
                    // without updating the model
                    if (adaptor.getFromSize() != 0)
                    {
                        localAdaptor->setFrom(children[i], adaptor.getFrom(link_number), newController, false);
                        localAdaptor->setTo(children[i], adaptor.getTo(link_number), newController, false);
                        link_number++;
                    }
                    o->set(i, localAdaptor);
                    continue;
                }
                default:
                    return 0;
            }
        }
        return o;
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // FIXME implement, decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee().get();

        types::List* list = v->getAs<types::List>();

        // Clear the children list before the loop to reset the diagram children
        // and clear the old Links information
        std::vector<ScicosID> diagramChildren;
        controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
        adaptor.clearFrom();
        adaptor.clearTo();
        std::vector<LinkAdapter*> linkListView;
        std::vector<ScicosID> linkListModel;
        for (int i = 0; i < list->getSize(); ++i)
        {
            if (list->get(i)->getType() != types::InternalType::ScilabUserType)
            {
                return false;
            }

            // Find the type of the input object through Adapters' mapping.
            const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(list->get(i)->getShortTypeStr());

            // Then, each adapter gets linked to the diagram through its adaptee (PARENT_DIAGRAM)
            // and the diagram's adaptee lists its adaptees (CHILDREN).
            ScicosID id;
            switch (adapter_index)
            {
                case Adapters::BLOCK_ADAPTER:
                {
                    BlockAdapter* modelElement = list->get(i)->getAs<BlockAdapter>();
                    model::Block* subAdaptee = modelElement->getAdaptee().get();

                    id = subAdaptee->id();

                    controller.setObjectProperty(id, BLOCK, PARENT_DIAGRAM, adaptee);
                    controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    diagramChildren.push_back(id);
                    controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    break;
                }
                case Adapters::LINK_ADAPTER:
                {
                    LinkAdapter* modelElement = list->get(i)->getAs<LinkAdapter>();
                    model::Link* subAdaptee = modelElement->getAdaptee().get();

                    id = subAdaptee->id();

                    controller.setObjectProperty(id, LINK, PARENT_DIAGRAM, adaptee);

                    // Hold Links information, to try the linking at model-level once all the elements have been added to the Diagram
                    linkListView.push_back(modelElement);
                    linkListModel.push_back(id);

                    controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    diagramChildren.push_back(id);
                    controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    break;
                }
                case Adapters::TEXT_ADAPTER:
                {
                    TextAdapter* modelElement = list->get(i)->getAs<TextAdapter>();
                    model::Annotation* subAdaptee = modelElement->getAdaptee().get();

                    id = subAdaptee->id();

                    controller.setObjectProperty(id, ANNOTATION, PARENT_DIAGRAM, adaptee->id());
                    controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    diagramChildren.push_back(id);
                    controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    break;
                }
                default:
                    return false;
            }
        }

        // Do the linking at model-level
        for (int i = 0; i < static_cast<int>(linkListView.size()); ++i)
        {
            // Trigger 'from' and 'to' properties
            std::vector<double> from_content = linkListView[i]->getFrom();
            if (!linkListView[i]->setFrom(linkListModel[i], from_content, controller, true))
            {
                return false;
            }
            adaptor.setFrom(from_content);
            std::vector<double> to_content = linkListView[i]->getTo();
            if (!linkListView[i]->setTo(linkListModel[i], to_content, controller, true))
            {
                return false;
            }
            adaptor.setTo(to_content);
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
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(adaptee)
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }

    contrib_content = new types::List();
}

DiagramAdapter::DiagramAdapter(const DiagramAdapter& adapter) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(adapter)
{
    // When cloning a DiagramAdapter, clone its Links information as well
    for (int i = 0; i < static_cast<int>(from_vec.size()); ++i)
    {
        setFrom(adapter.getFrom(i));
        setTo(adapter.getTo(i));
    }
}

DiagramAdapter::~DiagramAdapter()
{
    delete contrib_content;
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
    return contrib_content->clone();
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    delete contrib_content;
    contrib_content = v->clone();
}

std::vector<double> DiagramAdapter::getFrom(int link_number) const
{
    return from_vec[link_number];
}

int DiagramAdapter::getFromSize() const
{
    return static_cast<int>(from_vec.size());
}

void DiagramAdapter::setFrom(const std::vector<double>& from_content)
{
    from_vec.push_back(from_content);
}

void DiagramAdapter::clearFrom()
{
    from_vec.clear();
}

std::vector<double> DiagramAdapter::getTo(int link_number) const
{
    return to_vec[link_number];
}

void DiagramAdapter::setTo(const std::vector<double>& to_content)
{
    to_vec.push_back(to_content);
}

void DiagramAdapter::clearTo()
{
    to_vec.clear();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
