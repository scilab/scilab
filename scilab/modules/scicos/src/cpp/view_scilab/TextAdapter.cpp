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
#include <vector>

#include "double.hxx"
#include "string.hxx"
#include "mlist.hxx"
#include "internal.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "TextAdapter.hxx"

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

const std::wstring Graphics(L"graphics");
const std::wstring orig(L"orig");
const std::wstring sz(L"sz");
const std::wstring exprs(L"exprs");
const std::wstring style(L"style");

struct graphics
{
    static types::InternalType* get(const TextAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        types::MList* o = new types::MList();
        types::String* MListFields = new types::String(1, 5);
        MListFields->set(0, Graphics.c_str());
        MListFields->set(1, orig.c_str());
        MListFields->set(2, sz.c_str());
        MListFields->set(3, exprs.c_str());
        MListFields->set(4, style.c_str());
        o->append(MListFields);

        // orig and sz
        std::vector<double> geom;
        controller.getObjectProperty(adaptee, ANNOTATION, GEOMETRY, geom);

        double* dataOrig;
        double* dataSz;
        types::Double* origField = new types::Double(1, 2, &dataOrig);
        types::Double* szField   = new types::Double(1, 2, &dataSz);
        dataOrig[0] = geom[0];
        dataOrig[1] = geom[1];
        dataSz[0]   = geom[2];
        dataSz[1]   = geom[3];
        o->append(origField);
        o->append(szField);

        // exprs
        std::vector<std::string> Exprs (3);
        controller.getObjectProperty(adaptee, ANNOTATION, DESCRIPTION, Exprs[0]);
        controller.getObjectProperty(adaptee, ANNOTATION, FONT, Exprs[1]);
        controller.getObjectProperty(adaptee, ANNOTATION, FONT_SIZE, Exprs[2]);

        types::String* exprsField = new types::String(3, 1);
        for (int i = 0; i < (int) Exprs.size(); ++i)
        {
            exprsField->set(i, Exprs[i].data());
        }
        o->append(exprsField);

        // style
        std::string Style;
        controller.getObjectProperty(adaptee, ANNOTATION, STYLE, Style);
        types::String* styleField = new types::String(Style.data());
        o->append(styleField);

        return o;
    }

    static bool set(TextAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabMList)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: Mlist expected.\n"), "graphics");
            return false;
        }

        types::MList* current = v->getAs<types::MList>();
        if (current->getSize() < 4)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong size for field %s: at least %d elements expected.\n"), "graphics", 4);
            return false;
        }

        types::InternalType* currentField;
        types::Double* currentFieldDouble;
        types::String* currentFieldString;

        ScicosID adaptee = adaptor.getAdaptee()->id();

        // orig
        if ((currentField = current->getField(orig.c_str())) == nullptr)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong value for header of field %s: field %s expected.\n"), "graphics", "orig");
            return false;
        }
        if (currentField->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: Real matrix expected.\n"), "graphics", "orig");
            return false;
        }
        currentFieldDouble = currentField->getAs<types::Double>();
        if (currentFieldDouble->getSize() != 2)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimensions for field %s.%s: %d-by-%d expected.\n"), "graphics", "orig", 1, 2);
            return false;
        }
        std::vector<double> origField;
        controller.getObjectProperty(adaptee, ANNOTATION, GEOMETRY, origField);
        origField[0] = currentFieldDouble->get(0);
        origField[1] = currentFieldDouble->get(1);
        controller.setObjectProperty(adaptee, ANNOTATION, GEOMETRY, origField);

        // sz
        if ((currentField = current->getField(sz.c_str())) == nullptr)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong value for header of field %s: field %s expected.\n"), "graphics", "sz");
            return false;
        }
        if (currentField->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: Real matrix expected.\n"), "graphics", "sz");
            return false;
        }
        currentFieldDouble = currentField->getAs<types::Double>();
        if (currentFieldDouble->getSize() != 2)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimensions for field %s.%s: %d-by-%d expected.\n"), "graphics", "sz", 1, 2);
            return false;
        }
        std::vector<double> szField;
        controller.getObjectProperty(adaptee, ANNOTATION, GEOMETRY, szField);
        szField[2] = currentFieldDouble->get(0);
        szField[3] = currentFieldDouble->get(1);
        controller.setObjectProperty(adaptee, ANNOTATION, GEOMETRY, szField);

        // exprs
        if ((currentField = current->getField(exprs.c_str())) == nullptr)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong value for header of field %s: field %s expected.\n"), "graphics", "exprs");
            return false;
        }
        if (currentField->getType() == types::InternalType::ScilabString)
        {
            currentFieldString = currentField->getAs<types::String>();
            if (currentFieldString->getSize() != 3)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimensions for field %s.%s: %d-by-%d expected.\n"), "graphics", "exprs", 1, 3);
                return false;
            }

            std::vector<std::string> exprsField (3);
            for (int i = 0; i < (int) exprsField.size(); ++i)
            {
                char* c_str = wide_string_to_UTF8(currentFieldString->get(i));
                exprsField[i] = std::string(c_str);
                FREE(c_str);
            }
            controller.setObjectProperty(adaptee, ANNOTATION, DESCRIPTION, exprsField[0]);
            controller.setObjectProperty(adaptee, ANNOTATION, FONT, exprsField[1]);
            controller.setObjectProperty(adaptee, ANNOTATION, FONT_SIZE, exprsField[2]);
        }
        else if (currentField->getType() == types::InternalType::ScilabDouble)
        {
            currentFieldDouble = currentField->getAs<types::Double>();
            if (currentFieldDouble->getSize() != 0)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: String matrix expected.\n"), "graphics", "exprs");
                return false;
            }

            std::vector<std::string> exprsField (3);
            controller.setObjectProperty(adaptee, ANNOTATION, DESCRIPTION, exprsField[0]);
            controller.setObjectProperty(adaptee, ANNOTATION, FONT, exprsField[1]);
            controller.setObjectProperty(adaptee, ANNOTATION, FONT_SIZE, exprsField[2]);
        }
        else
        {
            return false;
        }

        // style, if it is present
        if ((current->getSize() >= 5) && ((currentField = current->getField(style.c_str())) != nullptr))
        {
            if (currentField->getType() == types::InternalType::ScilabDouble)
            {
                currentFieldDouble = currentField->getAs<types::Double>();
                if (currentFieldDouble->getSize() != 0)
                {
                    get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: String matrix expected.\n"), "graphics", "style");
                    return false;
                }
                std::string styleField;
                controller.setObjectProperty(adaptee, ANNOTATION, STYLE, styleField);
                return true;
            }

            if (currentField->getType() != types::InternalType::ScilabString)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: String matrix expected.\n"), "graphics", "style");
                return false;
            }
            currentFieldString = currentField->getAs<types::String>();
            if (currentFieldString->getSize() != 1)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimensions for field %s.%s: %d-by-%d expected.\n"), "graphics", "style", 1, 1);
                return false;
            }

            char* c_str = wide_string_to_UTF8(currentFieldString->get(0));
            std::string styleField (c_str);
            FREE(c_str);
            controller.setObjectProperty(adaptee, ANNOTATION, STYLE, styleField);
        }

        return true;
    }
};

struct dummy_property
{

    static types::InternalType* get(const TextAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        // Return an empty matrix because this field isn't used.
        return types::Double::Empty();
    }

    static bool set(TextAdapter& /*adaptor*/, types::InternalType* /*v*/, Controller& /*controller*/)
    {
        // everything should be right as the properties mapped using this adapter do not perform anything
        return true;
    }
};

struct gui
{

    static types::InternalType* get(const TextAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        return new types::String("TEXT_f");
    }
};

} /* namespace */

template<> property<TextAdapter>::props_t property<TextAdapter>::fields = property<TextAdapter>::props_t();

TextAdapter::TextAdapter(const Controller& c, org_scilab_modules_scicos::model::Annotation* adaptee) :
    BaseAdapter<TextAdapter, org_scilab_modules_scicos::model::Annotation>(c, adaptee)
{
    if (property<TextAdapter>::properties_have_not_been_set())
    {
        property<TextAdapter>::fields.reserve(4);
        property<TextAdapter>::add_property(Graphics, &graphics::get, &graphics::set);
        property<TextAdapter>::add_property(L"model", &dummy_property::get, &dummy_property::set);
        property<TextAdapter>::add_property(L"void", &dummy_property::get, &dummy_property::set);
        property<TextAdapter>::add_property(L"gui", &gui::get, &dummy_property::set);
    }
}

TextAdapter::TextAdapter(const TextAdapter& adapter) :
    BaseAdapter<TextAdapter, org_scilab_modules_scicos::model::Annotation>(adapter)
{
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
