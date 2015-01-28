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
#include "mlist.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "DiagramAdapter.hxx"
#include "Adapters.hxx"
#include "ParamsAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
#include "TextAdapter.hxx"
#include "model/BaseObject.hxx"
#include "recursive_cloning.hxx"

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

const std::wstring Deleted (L"Deleted");
const std::wstring TextSharedTypeStr (L"Text");

struct props
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        ParamsAdapter localAdaptor(adaptor.getAdaptee());
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor(adaptor.getAdaptee());
        return localAdaptor.setAsTList(v, controller);
    }
};

struct objs
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee().get();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, children);

        types::List* o = new types::List();

        std::vector<link_t> from = adaptor.getFrom();
        std::vector<link_t> to = adaptor.getTo();

        int link_number = 0;
        Controller newController;
        for (int i = 0; i < static_cast<int>(children.size()); ++i)
        {
            if (children[i] == 0)
            {
                types::MList* deletedObject = new types::MList();
                types::String* header = new types::String(Deleted.data());
                deletedObject->append(header);
                o->append(deletedObject);
                continue;
            }

            std::shared_ptr<model::BaseObject> item = newController.getObject(children[i]);

            switch (item->kind())
            {
                case ANNOTATION:
                {
                    std::shared_ptr<model::Annotation> annotation = std::static_pointer_cast<model::Annotation>(item);
                    TextAdapter* localAdaptor = new TextAdapter(annotation);
                    o->append(localAdaptor);
                    continue;
                }
                case BLOCK:
                {
                    std::shared_ptr<model::Block> block = std::static_pointer_cast<model::Block>(item);
                    BlockAdapter* localAdaptor = new BlockAdapter(block);

                    types::List* list_objects = adaptor.getListObjects()->getAs<types::List>();
                    if (i < list_objects->getSize())
                    {
                        BlockAdapter* oldBlock = list_objects->get(i)->getAs<BlockAdapter>();
                        DiagramAdapter* oldBlockDiagram = oldBlock->getDiagram();
                        if (oldBlockDiagram != nullptr)
                        {
                            oldBlockDiagram->IncreaseRef();
                        }
                        localAdaptor->setDiagram(oldBlockDiagram);
                        types::InternalType* oldDoc = oldBlock->getDocContent();
                        localAdaptor->setDocContent(oldDoc);
                    }

                    o->append(localAdaptor);
                    continue;
                }
                case LINK:
                {
                    std::shared_ptr<model::Link> link = std::static_pointer_cast<model::Link>(item);
                    LinkAdapter* localAdaptor = new LinkAdapter(link);

                    // In case a Link points to a Block that has not been added yet,
                    // retrieve the 'from' and 'to' values from the Diagram Adapter if they have been saved
                    if (link_number < static_cast<int>(from.size()))
                    {
                        localAdaptor->setFrom(from[link_number]);
                        localAdaptor->setTo(to[link_number]);
                        link_number++;
                    }
                    o->append(localAdaptor);
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
        // Decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee().get();

        types::List* list = v->getAs<types::List>();

        // Clear the children list before the loop to reset the diagram children
        // and clear the old Links information
        std::vector<ScicosID> oldDiagramChildren;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, oldDiagramChildren);
for (ScicosID id : oldDiagramChildren)
        {
            if (id != 0)
            {
                auto o = controller.getObject(id);
                controller.setObjectProperty(id, o->kind(), PARENT_DIAGRAM, 0ll);
            }
            else
            {
                // Deleted Block
            }
        }
        std::vector<ScicosID> diagramChildren;

        // Set the children to the right IDs
        std::vector<LinkAdapter*> linkListView;
        for (int i = 0; i < list->getSize(); ++i)
        {
            if (list->get(i)->getType() == types::InternalType::ScilabUserType)
            {

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

                        // Do the linking in the next loop, in case the Link points to a Block that has not been added yet
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
            else if (list->get(i)->getType() == types::InternalType::ScilabMList)
            {
                // Allow to pass mlists to 'objs', representing Text blocks
                types::MList* modelElement = list->get(i)->getAs<types::MList>();
                types::String* header = modelElement->getFieldNames();
                if (header->get(0) == Deleted)
                {
                    diagramChildren.push_back(0);
                    continue;
                }
                if (header->get(0) != TextSharedTypeStr)
                {
                    return false;
                }

                // Create a Text block based on the input MList and add it to the diagram
                ScicosID newID = controller.createObject(ANNOTATION);
                TextAdapter* newAdaptor = new TextAdapter(std::static_pointer_cast<model::Annotation>(controller.getObject(newID)));
                // Fill the block with the input mlist
                if (!newAdaptor->setAsTList(modelElement, controller))
                {
                    return false;
                }

                // Modify the input list to save the new block
                list->set(i, newAdaptor);

                controller.setObjectProperty(newID, ANNOTATION, PARENT_DIAGRAM, adaptee->id());
                diagramChildren.push_back(newID);
            }
            else if (list->get(i)->getType() == types::InternalType::ScilabList)
            {
                // Allow to pass empty lists to 'objs', representing deleted Blocks
                types::List* modelElement = list->get(i)->getAs<types::List>();
                if (modelElement->getSize() != 0)
                {
                    return false;
                }

                // Mark deleted objects with value '0'
                diagramChildren.push_back(0);
            }
            else
            {
                return false;
            }
        }
        controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);

        adaptor.setListObjects(v);

        std::vector<link_t> from_content (linkListView.size());
        std::vector<link_t> to_content (linkListView.size());
        // Do the linking at model-level
        for (int i = 0; i < static_cast<int>(linkListView.size()); ++i)
        {
            // Trigger 'from' and 'to' properties
            from_content[i] = linkListView[i]->getFrom();
            linkListView[i]->setFromInModel(from_content[i], controller);

            to_content[i] = linkListView[i]->getTo();
            linkListView[i]->setToInModel(to_content[i], controller);
        }
        adaptor.setFrom(from_content);
        adaptor.setTo(to_content);

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

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getContribContent();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
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
    list_objects(),
    from_vec(adapter.from_vec),
    to_vec(adapter.to_vec),
    contrib_content(adapter.getContribContent())
{
    // Generate an Adapter for each child of the cloned Diagram and store them all in 'list_objects'
    Controller controller;
    std::vector<ScicosID> diagramChildren;
    controller.getObjectProperty(getAdaptee()->id(), DIAGRAM, CHILDREN, diagramChildren);

    types::List* oldList_objects = adapter.getListObjects()->getAs<types::List>();

    list_objects = deepCreateAdapters(diagramChildren, oldList_objects, from_vec, to_vec, controller);
}

DiagramAdapter::~DiagramAdapter()
{
    // Unlink the diagram's children if the adaptee is being deleted
    if (getAdaptee().use_count() == 3)
    {
        Controller controller;
        std::vector<ScicosID> diagramChildren;
        controller.getObjectProperty(getAdaptee()->id(), DIAGRAM, CHILDREN, diagramChildren);
for (ScicosID id : diagramChildren)
        {
            if (id != 0)
            {
                auto o = controller.getObject(id);
                controller.setObjectProperty(id, o->kind(), PARENT_DIAGRAM, 0ll);
            }
        }
        diagramChildren.clear();
        controller.setObjectProperty(getAdaptee()->id(), DIAGRAM, CHILDREN, diagramChildren);
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

std::vector<link_t> DiagramAdapter::getFrom() const
{
    return from_vec;
}

void DiagramAdapter::setFrom(const std::vector<link_t>& from)
{
    from_vec = from;
}

std::vector<link_t> DiagramAdapter::getTo() const
{
    return to_vec;
}

void DiagramAdapter::setTo(const std::vector<link_t>& to)
{
    to_vec = to;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
