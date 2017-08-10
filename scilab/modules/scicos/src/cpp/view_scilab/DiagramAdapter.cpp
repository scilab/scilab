/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
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

#include <bitset>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>

#include "types.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "tlist.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "controller_helpers.hxx"

#include "view_scilab/Adapters.hxx"
#include "DiagramAdapter.hxx"
#include "ParamsAdapter.hxx"
#include "BlockAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"
#include "TextAdapter.hxx"
#include "model/BaseObject.hxx"
#include "model/Diagram.hxx"
#include "model/Block.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
#include "localization.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::wstring Deleted (L"Deleted");
const std::wstring Text (L"Text");
const std::wstring Block (L"Block");
const std::wstring Link (L"Link");

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
    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::BaseObject* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);
        types::List* ret = new types::List();
        // TODO: ret.reserve(children.size());
        for (int i = 0; i < static_cast<int>(children.size()); ++i)
        {
            if (children[i] == ScicosID())
            {
                // Deleted adapter
                types::MList* deleted = new types::MList();
                deleted->set(0, new types::String(Deleted.data()));
                ret->append(deleted);
                continue;
            }

            model::BaseObject* o = controller.getObject(children[i]);
            switch (o->kind())
            {
                case ANNOTATION :
                    ret->append(new TextAdapter(controller, controller.referenceObject<model::Annotation>(o)));
                    break;
                case BLOCK :
                    ret->append(new BlockAdapter(controller, controller.referenceObject<model::Block>(o)));
                    break;
                case LINK :
                {
                    ret->append(new LinkAdapter(controller, controller.referenceObject<model::Link>(o)));
                    break;
                }
                default:
                {
                    types::MList* deleted = new types::MList();
                    deleted->set(0, new types::String(Deleted.data()));
                    ret->append(deleted);
                    break;
                }
            }
        }

        return ret;
    }

    template <kind_t kind, typename Adapter, typename Adaptee>
    static Adapter* allocAndSet(types::MList* modelElement, Controller& controller)
    {
        ScicosID id = controller.createObject(kind);
        Adaptee* localAdaptee = controller.getObject<Adaptee>(id);

        Adapter* localAdaptor = new Adapter(controller, localAdaptee);
        localAdaptor->setAsTList(modelElement, controller);

        return localAdaptor;
    };

    /**
     * Allocate a model element from its mlist() representation
     *
     * \param modelElement the mlist representation
     * \param controller the shared controller
     * \param o the model object
     * \param a any adapter compatible with \o
     */
    static void allocateAsMList(types::MList* modelElement, Controller& controller, model::BaseObject*& o, types::UserType*& a)
    {
        types::String* header = modelElement->getFieldNames();

        if (header->get(0) == Deleted)
        {
            a = nullptr;
            o = nullptr;
        }
        else if (header->get(0) == Text)
        {
            TextAdapter* adapter = allocAndSet<ANNOTATION, TextAdapter, model::Annotation>(modelElement, controller);
            a = adapter;
            o = adapter->getAdaptee();
        }
        else if (header->get(0) == Block)
        {
            BlockAdapter* adapter = allocAndSet<BLOCK, BlockAdapter, model::Block>(modelElement, controller);
            a = adapter;
            o = adapter->getAdaptee();
        }
        else if (header->get(0) == Link)
        {
            LinkAdapter* adapter = allocAndSet<LINK, LinkAdapter, model::Link>(modelElement, controller);
            a = adapter;
            o = adapter->getAdaptee();
        }
        else
        {
            a = nullptr;
            o = nullptr;
        }
    }

    // Helper to manage updates
    //  * set an adapter at a position
    //  * clear an adapter (eg. remove object) if the value is nullptr
    struct update_t
    {
        update_t(int index, model::BaseObject* adaptee, types::InternalType* adapter) :
            index(index), adaptee(adaptee), adapter(adapter) {}

        int index;
        model::BaseObject* adaptee;
        types::InternalType* adapter;
    };

    // Reference all the inserted children before processing them.
    // This struct (cstr and dstr) will avoid early deletion
    struct ChildrenToUpdateOwner
    {
        ChildrenToUpdateOwner(Controller& controller, const std::vector<update_t>& childrenToUpdate) :
            controller(controller),
            childrenToUpdate(childrenToUpdate)
        {
            std::for_each(childrenToUpdate.begin(), childrenToUpdate.end(), [&controller] (const update_t & u)
            {
                if (u.adaptee != nullptr)
                {
                    controller.referenceObject(u.adaptee->id());
                }
            });
        }

        ~ChildrenToUpdateOwner()
        {
            std::for_each(childrenToUpdate.begin(), childrenToUpdate.end(), [this] (const update_t & u)
            {
                if (u.adaptee != nullptr)
                {
                    controller.deleteObject(u.adaptee->id());
                }
            });
        }

        Controller& controller;
        const std::vector<update_t>& childrenToUpdate;
    };

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // Decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: list expected.\n"), "objs");
            return false;
        }

        model::BaseObject* adaptee = adaptor.getAdaptee();
        types::List* argumentList = v->getAs<types::List>();

        // retrieve the current children to update
        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

        // A boolean to know if we are removing an object
        bool deletion = children.size() > static_cast<size_t>(argumentList->getSize());
        // There is going to be as many children as the input list suggests, but don't lose information on old children just yet
        if (!deletion)
        {
            children.resize(argumentList->getSize());
        }

        /*
         * Fill a buffer of things to update
         */

        // work buffer :
        std::vector<update_t> childrenToUpdate;

        // fill the work buffers accordingly to the arguments
        for (int i = 0; i < argumentList->getSize(); ++i)
        {
            types::InternalType* pIT = argumentList->get(i);
            switch (pIT->getType())
            {
                case types::InternalType::ScilabUserType:
                {
                    model::BaseObject* o = Adapters::instance().descriptor(pIT);
                    if (o == nullptr)
                    {
                        get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for element %d of field %s: unknown Scicos object.\n"), i + 1, "objs");
                        return false;
                    }

                    // clone if the adapter is used somewhere else (eg. not owned by the list)
                    if (pIT->getRef() > 1 || o->refCount() > 0) // over-clone some elements but PASS the tests
                        //                     if (pIT->getRef() > 1) // TODO: investigate why this expression is not enough
                    {
                        types::InternalType* clonedAdapter = pIT->clone();
                        model::BaseObject* cloned = Adapters::instance().descriptor(clonedAdapter);

                        LinkAdapter::add_partial_links_information(controller, o, cloned);
                        GraphicsAdapter::add_partial_links_information(controller, o, cloned);
                        childrenToUpdate.emplace_back(i, cloned, clonedAdapter);
                    }
                    else
                    {
                        pIT->IncreaseRef();
                        childrenToUpdate.emplace_back(i, o, pIT);
                    }
                    break;
                }
                case types::InternalType::ScilabMList:
                    model::BaseObject* o;
                    types::UserType* a;
                    allocateAsMList(pIT->getAs<types::MList>(), controller, o, a);
                    childrenToUpdate.emplace_back(i, o, a);
                    break;
                case types::InternalType::ScilabList:
                    // clear on list()
                    if (pIT->getAs<types::List>()->getSize() != 0)
                    {
                        get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for element %d of field %s: unknown Scicos object.\n"), i + 1, "objs");
                        return false;
                    }
                    childrenToUpdate.emplace_back(i, nullptr, nullptr);
                    break;
                default:
                    get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for element %d of field %s: unknown Scicos object.\n"), i + 1, "objs");
                    return false;
            }
        }

        // clear trailing objects
        for (int i = argumentList->getSize() ; i < static_cast<int>(children.size()); ++i)
        {
            childrenToUpdate.emplace_back(i, nullptr, nullptr);
        }

        /*
         * Update parent / children
         */
        ChildrenToUpdateOwner tempOwning(controller, childrenToUpdate);

        // Process the children / parent relationship
        ScicosID parentDiagram;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PARENT_DIAGRAM, parentDiagram);
        int offset = 0;
        for (const auto & update : childrenToUpdate)
        {
            // reference / derefence the content
            if (update.adapter == nullptr)
            {
                controller.deleteObject(children[update.index]);
            }
            else
            {
                controller.referenceObject(update.adaptee->id());
                if (deletion && children[update.index] == ScicosID())
                {
                    // This object is the one being deleted in the diagram:
                    //  - we are in effective delete mode
                    //  - the old object is a "Deleted" mlist (deletion in two steps)
                    //  - the new object is not a "Deleted" mlist (just replacing the old one)
                    // Then 'offset' will skip the mlist so all the old children are deleted
                    ++offset;
                }
                controller.deleteObject(children[update.index + offset]);
            }

            // manage insertion and field update
            if (update.adapter == nullptr)
            {
                children[update.index] = ScicosID();
            }
            else
            {
                children[update.index] = update.adaptee->id();

                if (adaptee->kind() == BLOCK)
                {
                    controller.setObjectProperty(update.adaptee->id(), update.adaptee->kind(), PARENT_DIAGRAM, parentDiagram);
                    controller.setObjectProperty(update.adaptee->id(), update.adaptee->kind(), PARENT_BLOCK, adaptee->id());
                }
                else
                {
                    controller.setObjectProperty(update.adaptee->id(), update.adaptee->kind(), PARENT_DIAGRAM, adaptee->id());
                    controller.setObjectProperty(update.adaptee->id(), update.adaptee->kind(), PARENT_BLOCK, ScicosID());
                }
            }
        }

        // We don't need old children information anymore: reduce children size
        if (deletion)
        {
            children.resize(argumentList->getSize());
        }

        /*
         * Update partial linking information (links then ports)
         */
        for (const auto & update : childrenToUpdate)
        {
            if (update.adaptee != nullptr && update.adaptee->kind() == LINK)
            {
                LinkAdapter::relink(controller, update.adaptee, children);
            }
        }
        for (const auto & update : childrenToUpdate)
        {
            if (update.adaptee != nullptr && update.adaptee->kind() == BLOCK)
            {
                GraphicsAdapter::relink(controller, update.adaptee, children);
            }
        }

        // unref the Adapters as the ownership has been transfered to the Model
        for (const auto & update : childrenToUpdate)
        {
            if (update.adapter != nullptr)
            {
                update.adapter->DecreaseRef();
                update.adapter->killMe();
            }
        }

        // set the children after update
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        std::string version;
        if (adaptor.getAdaptee()->kind() == BLOCK)
        {
            model::Block* adaptee = static_cast<model::Block*>(adaptor.getAdaptee());

            ScicosID parentDiagram;
            controller.getObjectProperty(adaptee->id(), adaptee->kind(), PARENT_DIAGRAM, parentDiagram);
            controller.getObjectProperty(parentDiagram, DIAGRAM, VERSION_NUMBER, version);
        }
        else
        {
            model::Diagram* adaptee = static_cast<model::Diagram*>(adaptor.getAdaptee());
            controller.getObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
        }

        return new types::String(version.data());
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s: %d-by-%d expected.\n"), "version", 1, 1);
                return false;
            }

            if (adaptor.getAdaptee()->kind() != DIAGRAM)
            {
                // version field is not present on the model for non-diagram ; let's pass it !
                return true;
            }
            model::Diagram* adaptee = static_cast<model::Diagram*>(adaptor.getAdaptee());

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string version (c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong size for field %s: at least %d-by-%d expected.\n"), "version", 1, 1);
                return false;
            }

            if (adaptor.getAdaptee()->kind() != DIAGRAM)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("\"%s\" is a read-only field.\n"), "version");
                return false;
            }
            model::Diagram* adaptee = static_cast<model::Diagram*>(adaptor.getAdaptee());

            std::string version;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
            return true;
        }

        get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: Real matrix expected.\n"), "graphics", "orig");
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
        adaptor.setContribContent(v);
        return true;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(const Controller& c, org_scilab_modules_scicos::model::BaseObject* adaptee) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::BaseObject>(c, adaptee),
    contrib_content(default_value<types::List>())
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
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::BaseObject>(adapter),
    contrib_content(reference_value(adapter.contrib_content))
{
    contrib_content->IncreaseRef();
}

DiagramAdapter::~DiagramAdapter()
{
    contrib_content->DecreaseRef();
    contrib_content->killMe();
}

std::wstring DiagramAdapter::getTypeStr() const
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr() const
{
    return getSharedTypeStr();
}

types::InternalType* DiagramAdapter::getContribContent() const
{
    return contrib_content;
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    // v and contrib_content should not be nullptr

    types::InternalType* temp = contrib_content;

    v->IncreaseRef();
    contrib_content = v;

    temp->DecreaseRef();
    temp->killMe();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
