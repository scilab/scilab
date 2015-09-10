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
#include <algorithm>

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

#include "view_scilab/Adapters.hxx"
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

const std::wstring Deleted (L"Deleted");
const std::wstring TextSharedTypeStr (L"Text");
const std::wstring BlockSharedTypeStr (L"Block");
const std::wstring LinkSharedTypeStr (L"Link");

struct props
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        ParamsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        return localAdaptor.setAsTList(v, controller);
    }
};

struct objs
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& /*controller*/)
    {
        // FIXME: reconstruct the list of objects accordingly to the CHILDREN content to manage xcos modification
        return adaptor.getListObjects();
    }

    /**
     * Clone the object if it is owned by something else than the list
     */
    static types::InternalType* cloneIfNeeded(types::InternalType* v, const std::vector<types::InternalType*>& oldChildren)
    {
        if (v->getRef() == 0)
        {
            return v;
        }

        // avoid a copy, if v is already in the previous children list and will be cleanup later
        if (v->getRef() == 1 && std::binary_search(oldChildren.begin(), oldChildren.end(), v))
        {
            return v;
        }

        return v->clone();
    }

    static std::vector<types::InternalType*> extractAndSort(types::List* v)
    {
        std::vector<types::InternalType*> ret;
        if (v == nullptr)
        {
            return ret;
        }

        ret.reserve(v->getSize());
        for (int i = 0; i < v->getSize(); ++i)
        {
            ret.push_back(v->get(i));
        }

        std::sort(ret.begin(), ret.end());
        return ret;
    }


    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // Decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        /*
         * Always deep clone both the list and children as the PARENT_DIAGRAM property will be updated
         */
        model::Diagram* adaptee = adaptor.getAdaptee();

        types::List* argumentList = v->getAs<types::List>();
        types::List* list = new types::List();
        std::vector<types::InternalType*> oldChildren = extractAndSort(adaptor.getListObjects());

        std::vector<ScicosID> oldDiagramChildren;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, oldDiagramChildren);

        /*
         * First pass on objects :
         *  - store IDs if they exists and are valid ; 0ll otherwise
         *  - store all the links to update link connections later
         *  - store all the valid mlist content ('Text' content)
         *  - store all the deleted mlist content ('Deleted' content)
         */

        std::vector<ScicosID> diagramChildren;
        diagramChildren.reserve(argumentList->getSize());
        std::vector<kind_t> diagramChildrenKind;
        diagramChildrenKind.reserve(argumentList->getSize());

        std::vector<LinkAdapter*> links;
        std::vector<int> textAsMListIndices;
        std::vector<int> deletedAsMListIndices;

        for (int i = 0; i < argumentList->getSize(); ++i)
        {
            if (argumentList->get(i)->getType() == types::InternalType::ScilabUserType)
            {
                const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(argumentList->get(i)->getShortTypeStr());

                ScicosID id;
                kind_t kind;
                types::InternalType* adapter;
                switch (adapter_index)
                {
                    case Adapters::BLOCK_ADAPTER:
                    {
                        BlockAdapter* modelElement = cloneIfNeeded(argumentList->get(i), oldChildren)->getAs<BlockAdapter>();
                        id = modelElement->getAdaptee()->id();
                        kind = modelElement->getAdaptee()->kind();
                        adapter = modelElement;
                        break;
                    }
                    case Adapters::LINK_ADAPTER:
                    {
                        LinkAdapter* modelElement = cloneIfNeeded(argumentList->get(i), oldChildren)->getAs<LinkAdapter>();
                        id = modelElement->getAdaptee()->id();
                        kind = modelElement->getAdaptee()->kind();
                        adapter = modelElement;

                        // Do the linking in the next loop, in case the Link points to a Block that has not been added yet
                        links.push_back(modelElement);
                        break;
                    }
                    case Adapters::TEXT_ADAPTER:
                    {
                        TextAdapter* modelElement = cloneIfNeeded(argumentList->get(i), oldChildren)->getAs<TextAdapter>();
                        id = modelElement->getAdaptee()->id();
                        kind = modelElement->getAdaptee()->kind();
                        adapter = modelElement;
                        break;
                    }
                    default:
                        list->killMe();
                        return false;
                }

                diagramChildren.push_back(id);
                diagramChildrenKind.push_back(kind);
                list->set(i, adapter);
            }
            else if (argumentList->get(i)->getType() == types::InternalType::ScilabMList)
            {
                // Allow to pass mlists to 'objs', representing 'Deleted' or 'Text' objects
                types::MList* modelElement = argumentList->get(i)->getAs<types::MList>();
                types::String* header = modelElement->getFieldNames();

                if (header->get(0) == Deleted)
                {
                    deletedAsMListIndices.push_back(i);
                    diagramChildren.push_back(0ll);
                    diagramChildrenKind.push_back(ANNOTATION);
                }
                else if (header->get(0) == TextSharedTypeStr)
                {
                    textAsMListIndices.push_back(i);
                    // will be filled later
                    diagramChildren.push_back(0ll);
                    diagramChildrenKind.push_back(ANNOTATION);
                }
                else if (header->get(0) == BlockSharedTypeStr)
                {
                    ScicosID localAdaptee = controller.createObject(BLOCK);
                    BlockAdapter* localAdaptor = new BlockAdapter(controller, controller.getObject<model::Block>(localAdaptee));
                    if (!localAdaptor->setAsTList(modelElement, controller))
                    {
                        list->killMe();
                        return false;
                    }

                    diagramChildren.push_back(localAdaptee);
                    list->set(i, localAdaptor);
                }
                else if (header->get(0) == LinkSharedTypeStr)
                {
                    ScicosID localAdaptee = controller.createObject(LINK);
                    LinkAdapter* localAdaptor = new LinkAdapter(controller, controller.getObject<model::Link>(localAdaptee));
                    if (!localAdaptor->setAsTList(modelElement, controller))
                    {
                        list->killMe();
                        return false;
                    }

                    diagramChildren.push_back(localAdaptee);
                    list->set(i, localAdaptor);
                }
                else
                {
                    list->killMe();
                    return false;
                }
            }
            else if (argumentList->get(i)->getType() == types::InternalType::ScilabList)
            {
                // Allow to pass empty lists to 'objs', representing deleted Blocks
                types::List* modelElement = argumentList->get(i)->getAs<types::List>();
                if (modelElement->getSize() != 0)
                {
                    list->killMe();
                    return false;
                }

                deletedAsMListIndices.push_back(i);
                // Mark deleted objects with value '0'
                diagramChildren.push_back(0ll);
                diagramChildrenKind.push_back(ANNOTATION);
            }
            else
            {
                list->killMe();
                return false;
            }
        }

        /*
         * Create all Annotation, decoding mlist content
         */
        for (const auto index : textAsMListIndices)
        {
            ScicosID localAdaptee = controller.createObject(ANNOTATION);
            TextAdapter* localAdaptor = new TextAdapter(controller, controller.getObject<model::Annotation>(localAdaptee));
            if (!localAdaptor->setAsTList(argumentList->get(index), controller))
            {
                // do not return there ; the annotation will be empty
            }

            diagramChildren[index] = localAdaptee;
            list->set(index, localAdaptor);
        }

        /*
         * Recreate 'Deleted' mlist if needed
         */
        for (const auto index : deletedAsMListIndices)
        {
            types::MList* deleted = new types::MList();
            deleted->set(0, new types::String(Deleted.data()));

            list->set(index, deleted);
        }

        /*
         * Set the parent diagram of all the blocks
         */
        controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
        {
            std::sort(oldDiagramChildren.begin(), oldDiagramChildren.end());
            for (const ScicosID id : diagramChildren)
            {
                if (id != 0 && !std::binary_search(oldDiagramChildren.begin(), oldDiagramChildren.end(), id))
                {
                    auto o = controller.getObject(id);
                    controller.setObjectProperty(o->id(), o->kind(), PARENT_DIAGRAM, adaptee->id());

                    controller.referenceObject(id);
                }
            }

            std::sort(diagramChildren.begin(), diagramChildren.end());
            for (const ScicosID id : oldDiagramChildren)
            {
                if (id != 0 && !std::binary_search(diagramChildren.begin(), diagramChildren.end(), id))
                {
                    auto o = controller.getObject(id);
                    controller.setObjectProperty(o->id(), o->kind(), PARENT_DIAGRAM, ScicosID());

                    controller.deleteObject(id);
                }
            }
        }

        /*
         * Store the children to the local adaptor, avoiding the deletion of argument
         */
        v->IncreaseRef();
        adaptor.setListObjects(list);

        /*
         * Re-sync the partial link information
         */
        std::vector<link_t> from_content (links.size());
        std::vector<link_t> to_content (links.size());
        // Do the linking at model-level
        for (int i = 0; i < static_cast<int>(links.size()); ++i)
        {
            // Trigger 'from' and 'to' properties
            from_content[i] = links[i]->getFrom();
            links[i]->setFromInModel(from_content[i], controller);

            to_content[i] = links[i]->getTo();
            links[i]->setToInModel(to_content[i], controller);
        }
        adaptor.setFrom(from_content);
        adaptor.setTo(to_content);

        v->DecreaseRef();
        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

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

            model::Diagram* adaptee = adaptor.getAdaptee();

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

            model::Diagram* adaptee = adaptor.getAdaptee();

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

DiagramAdapter::DiagramAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(c, adaptee),
    list_objects(nullptr),
    from_vec(),
    to_vec(),
    contrib_content(nullptr)
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }

    setListObjects(new types::List());
    setContribContent(new types::List());
}

DiagramAdapter::DiagramAdapter(const DiagramAdapter& adapter) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(adapter, false),
    list_objects(nullptr),
    from_vec(),
    to_vec(),
    contrib_content(nullptr)
{
    Controller controller;

    // set the list and perform from / to links update
    objs::set(*this, adapter.getListObjects(), controller);

    setContribContent(adapter.getContribContent());
}

DiagramAdapter::~DiagramAdapter()
{
    // CHILDREN will be unreferenced on Controller::deleteObject

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
    return contrib_content;
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    types::InternalType* temp = contrib_content;

    // do not check if v is nullptr on purpose ; it *should* not
    v->IncreaseRef();
    contrib_content = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }


}

types::List* DiagramAdapter::getListObjects() const
{
    return list_objects;
}

void DiagramAdapter::setListObjects(types::List* v)
{
    types::InternalType* temp = list_objects;

    // do not check if v is nullptr on purpose ; it *should* not
    v->IncreaseRef();
    list_objects = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }
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
