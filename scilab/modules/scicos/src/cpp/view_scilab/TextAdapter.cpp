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
#include <memory>

#include "double.hxx"
#include "string.hxx"
#include "mlist.hxx"
#include "internal.hxx"

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

const std::wstring Model(L"model");
const std::wstring sim(L"sim");
const std::wstring rpar(L"rpar");
const std::wstring ipar(L"ipar");

struct graphics
{
    static types::InternalType* get(const TextAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        types::MList* o = new types::MList();
        types::String* MListFields = new types::String(1, 4);
        MListFields->set(0, Graphics.c_str());
        MListFields->set(1, orig.c_str());
        MListFields->set(2, sz.c_str());
        MListFields->set(3, exprs.c_str());
        o->set(0, MListFields);

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
        o->set(1, origField);
        o->set(2, szField);

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
        o->set(3, exprsField);

        return o;
    }

    static bool set(TextAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabMList)
        {
            return false;
        }

        types::MList* current = v->getAs<types::MList>();
        if (current->getSize() < 4)
        {
            return false;
        }

        types::InternalType* currentField;
        types::Double* currentFieldDouble;
        types::String* currentFieldString;

        ScicosID adaptee = adaptor.getAdaptee()->id();

        // orig
        if ((currentField = current->getField(orig.c_str())) == NULL)
        {
            return false;
        }
        if (currentField->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }
        currentFieldDouble = currentField->getAs<types::Double>();
        if (currentFieldDouble->getRows() != 1 || currentFieldDouble->getCols() != 2)
        {
            return false;
        }
        std::vector<double> origField;
        controller.getObjectProperty(adaptee, ANNOTATION, GEOMETRY, origField);
        origField[0] = currentFieldDouble->get(0);
        origField[1] = currentFieldDouble->get(1);
        controller.setObjectProperty(adaptee, ANNOTATION, GEOMETRY, origField);

        // sz
        if ((currentField = current->getField(sz.c_str())) == NULL)
        {
            return false;
        }
        if (currentField->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }
        currentFieldDouble = currentField->getAs<types::Double>();
        if (currentFieldDouble->getRows() != 1 || currentFieldDouble->getCols() != 2)
        {
            return false;
        }
        std::vector<double> szField;
        controller.getObjectProperty(adaptee, ANNOTATION, GEOMETRY, szField);
        szField[2] = currentFieldDouble->get(0);
        szField[3] = currentFieldDouble->get(1);
        controller.setObjectProperty(adaptee, ANNOTATION, GEOMETRY, szField);

        // exprs
        if ((currentField = current->getField(exprs.c_str())) == NULL)
        {
            return false;
        }
        if (currentField->getType() == types::InternalType::ScilabString)
        {
            currentFieldString = currentField->getAs<types::String>();
            if (currentFieldString->getCols() != 1 || currentFieldString->getSize() != 3)
            {
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
            return true;
        }
        else if (currentField->getType() == types::InternalType::ScilabDouble)
        {
            currentFieldDouble = currentField->getAs<types::Double>();
            if (currentFieldDouble->getSize() != 0)
            {
                return false;
            }

            std::vector<std::string> exprsField (3);
            controller.setObjectProperty(adaptee, ANNOTATION, DESCRIPTION, exprsField[0]);
            controller.setObjectProperty(adaptee, ANNOTATION, FONT, exprsField[1]);
            controller.setObjectProperty(adaptee, ANNOTATION, FONT_SIZE, exprsField[2]);
            return true;
        }

        return false;
    }
};

struct model
{

    static types::InternalType* get(const TextAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        // Return an empty "model"-typed mlist because this field isn't used.
        types::MList* o = new types::MList();
        types::String* MListFields = new types::String(1, 4);

        MListFields->set(0, Model.c_str());
        MListFields->set(1, sim.c_str());
        MListFields->set(2, rpar.c_str());
        MListFields->set(3, ipar.c_str());

        o->set(0, MListFields);
        // 'sim' field needs to be defined for the console display.
        o->set(1, types::Double::Empty());
        o->set(2, types::Double::Empty());
        o->set(3, types::Double::Empty());
        return o;
    }

    static bool set(TextAdapter& /*adaptor*/, types::InternalType* /*v*/, Controller& /*controller*/)
    {
        // Everything should be right as the properties mapped using this adapter do not perform anything
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

} /* namespace */

template<> property<TextAdapter>::props_t property<TextAdapter>::fields = property<TextAdapter>::props_t();

TextAdapter::TextAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Annotation> adaptee) :
    BaseAdapter<TextAdapter, org_scilab_modules_scicos::model::Annotation>(adaptee)
{
    if (property<TextAdapter>::properties_have_not_been_set())
    {
        property<TextAdapter>::fields.reserve(4);
        property<TextAdapter>::add_property(Graphics, &graphics::get, &graphics::set);
        property<TextAdapter>::add_property(Model, &model::get, &model::set);
        property<TextAdapter>::add_property(L"void", &dummy_property::get, &dummy_property::set);
        property<TextAdapter>::add_property(L"gui", &dummy_property::get, &dummy_property::set);
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
