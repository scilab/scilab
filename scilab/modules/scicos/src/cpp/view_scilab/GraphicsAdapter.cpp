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

#include <cwchar>

#include <string>
#include <vector>
#include <sstream>
#include <memory>

#include "list.hxx"
#include "tlist.hxx"
#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "GraphicsAdapter.hxx"
#include "ports_management.hxx"

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

const std::wstring MBLOCK (L"MBLOCK");
const std::wstring in (L"in");
const std::wstring intype (L"intype");
const std::wstring out (L"out");
const std::wstring outtype (L"outtype");
const std::wstring param (L"param");
const std::wstring paramv (L"paramv");
const std::wstring pprop (L"pprop");
const std::wstring nameF (L"nameF");
const std::wstring funtxt (L"funtxt");

struct orig
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> geom;
        controller.getObjectProperty(adaptee, BLOCK, GEOMETRY, geom);

        data[0] = geom[0];
        data[1] = geom[1];

        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 2)
        {
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();
        std::vector<double> geom;
        controller.getObjectProperty(adaptee, BLOCK, GEOMETRY, geom);

        geom[0] = current->get(0);
        geom[1] = current->get(1);

        controller.setObjectProperty(adaptee, BLOCK, GEOMETRY, geom);
        return true;
    }
};

struct sz
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> geom;
        controller.getObjectProperty(adaptee, BLOCK, GEOMETRY, geom);

        data[0] = geom[2];
        data[1] = geom[3];
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 2)
        {
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();
        std::vector<double> geom;
        controller.getObjectProperty(adaptee, BLOCK, GEOMETRY, geom);

        geom[2] = current->get(0);
        geom[3] = current->get(1);

        controller.setObjectProperty(adaptee, BLOCK, GEOMETRY, geom);
        return true;
    }
};

struct flip
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        int* data;
        types::Bool* o = new types::Bool(1, 1, &data);
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> angle;
        controller.getObjectProperty(adaptee, BLOCK, ANGLE, angle);

        data[0] = static_cast<int>(angle[0]);
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabBool)
        {
            return false;
        }

        types::Bool* current = v->getAs<types::Bool>();
        if (current->isScalar() != true)
        {
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();
        std::vector<double> angle;
        controller.getObjectProperty(adaptee, BLOCK, ANGLE, angle);

        angle[0] = (current->get(0) == false) ? 0 : 1;

        controller.setObjectProperty(adaptee, BLOCK, ANGLE, angle);
        return true;
    }
};

struct theta
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 1, &data);
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> angle;
        controller.getObjectProperty(adaptee, BLOCK, ANGLE, angle);

        data[0] = angle[1];
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->isScalar() != true)
        {
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();
        std::vector<double> angle;
        controller.getObjectProperty(adaptee, BLOCK, ANGLE, angle);

        angle[1] = current->get(0);

        controller.setObjectProperty(adaptee, BLOCK, ANGLE, angle);
        return true;
    }
};

struct exprs
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<std::string> exprs;
        controller.getObjectProperty(adaptee, BLOCK, EXPRS, exprs);

        if (exprs.size() < 10 || (!exprs.empty() && exprs[0] != "MBLOCK"))
        {
            // Simple case
            types::String* o = new types::String((int)exprs.size(), 1);
            for (int i = 0; i < (int)exprs.size(); ++i)
            {
                o->set(i, exprs[i].data());
            }
            return o;
        }
        else
        {
            // Get and return a Modelica tlist
            types::TList* o = new types::TList();

            // Header, starting with "MBLOCK"
            types::String* header = new types::String(1, 10);
            header->set(0, MBLOCK.c_str());
            header->set(1, in.c_str());
            header->set(2, intype.c_str());
            header->set(3, out.c_str());
            header->set(4, outtype.c_str());
            header->set(5, param.c_str());
            header->set(6, paramv.c_str());
            header->set(7, pprop.c_str());
            header->set(8, nameF.c_str());
            header->set(9, funtxt.c_str());
            o->set(0, header);

            // 'in'
            types::String* inField = new types::String(exprs[1].c_str());
            o->set(1, inField);

            // 'intype'
            types::String* intypeField = new types::String(exprs[2].c_str());
            o->set(2, intypeField);

            // 'out'
            types::String* outField = new types::String(exprs[3].c_str());
            o->set(3, outField);

            // 'outtype'
            types::String* outtypeField = new types::String(exprs[4].c_str());
            o->set(4, outtypeField);

            // 'param'
            types::String* paramField = new types::String(exprs[5].c_str());
            o->set(5, paramField);

            // 'paramv'
            types::List* paramvField = new types::List();
            std::istringstream paramvSizeStr (exprs[6]);
            int paramvSize;
            paramvSizeStr >> paramvSize;
            for (int i = 0; i < paramvSize; ++i)
            {
                types::String* paramvElement = new types::String(exprs[7 + i].c_str());
                paramvField->set(i, paramvElement);
            }
            o->set(6, paramvField);

            // 'pprop'
            types::String* ppropField = new types::String(exprs[7 + paramvSize].c_str());
            o->set(7, ppropField);

            // 'nameF'
            types::String* nameFField = new types::String(exprs[7 + paramvSize + 1].c_str());
            o->set(8, nameFField);

            // 'funtxt'
            std::istringstream funtxtSizeStr (exprs[7 + paramvSize + 2]);
            int funtxtSize;
            funtxtSizeStr >> funtxtSize;

            if (funtxtSize == 0)
            {
                // An empty 'funtxt' field returns an empty matrix
                o->set(9, types::Double::Empty());
                return o;
            }
            types::String* funtxtField = new types::String(funtxtSize, 1);
            for (int i = 0; i < funtxtSize; ++i)
            {
                funtxtField->set(i, exprs[7 + paramvSize + 3 + i].c_str());
            }
            o->set(9, funtxtField);

            return o;
        }
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();

            std::vector<std::string> exprs (current->getSize());
            for (int i = 0; i < (int)exprs.size(); ++i)
            {
                char* c_str = wide_string_to_UTF8(current->get(i));
                exprs[i] = std::string(c_str);
                FREE(c_str);
            }

            controller.setObjectProperty(adaptee, BLOCK, EXPRS, exprs);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getRows() != 0 || current->getCols() != 0)
            {
                return false;
            }

            std::vector<std::string> exprs;
            controller.setObjectProperty(adaptee, BLOCK, EXPRS, exprs);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabList)
        {
            std::vector<std::string> exprs;
            controller.setObjectProperty(adaptee, BLOCK, EXPRS, exprs);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabTList)
        {
            types::TList* current = v->getAs<types::TList>();

            // Check the header
            types::String* header = current->getFieldNames();
            if (header->getSize() != 10)
            {
                return false;
            }
            if (header->get(0) != MBLOCK)
            {
                return false;
            }
            if (header->get(1) != in)
            {
                return false;
            }
            if (header->get(2) != intype)
            {
                return false;
            }
            if (header->get(3) != out)
            {
                return false;
            }
            if (header->get(4) != outtype)
            {
                return false;
            }
            if (header->get(5) != param)
            {
                return false;
            }
            if (header->get(6) != paramv)
            {
                return false;
            }
            if (header->get(7) != pprop)
            {
                return false;
            }
            if (header->get(8) != nameF)
            {
                return false;
            }
            if (header->get(9) != funtxt)
            {
                return false;
            }

            std::vector<std::string> exprs (1, "MBLOCK");
            char* c_str; // Buffer

            // 'in'
            if (current->get(1)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* inField = current->get(1)->getAs<types::String>();
            if (inField->getSize() != 1)
            {
                return false;
            }
            c_str = wide_string_to_UTF8(inField->get(0));
            std::string inFieldStored(c_str);
            FREE(c_str);
            exprs.push_back(inFieldStored);

            // 'intype'
            if (current->get(2)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* intypeField = current->get(2)->getAs<types::String>();
            if (intypeField->getSize() != 1)
            {
                return false;
            }
            c_str = wide_string_to_UTF8(intypeField->get(0));
            std::string intypeFieldStored(c_str);
            FREE(c_str);
            exprs.push_back(intypeFieldStored);

            // 'out'
            if (current->get(3)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* outField = current->get(3)->getAs<types::String>();
            if (inField->getSize() != 1)
            {
                return false;
            }
            c_str = wide_string_to_UTF8(outField->get(0));
            std::string outFieldStored(c_str);
            FREE(c_str);
            exprs.push_back(outFieldStored);

            // 'outtype'
            if (current->get(4)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* outtypeField = current->get(4)->getAs<types::String>();
            if (outtypeField->getSize() != 1)
            {
                return false;
            }
            c_str = wide_string_to_UTF8(outtypeField->get(0));
            std::string outtypeFieldStored(c_str);
            FREE(c_str);
            exprs.push_back(outtypeFieldStored);

            // 'param'
            if (current->get(5)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* paramField = current->get(5)->getAs<types::String>();
            if (paramField->getSize() != 1)
            {
                return false;
            }
            c_str = wide_string_to_UTF8(paramField->get(0));
            std::string paramFieldStored(c_str);
            FREE(c_str);
            exprs.push_back(paramFieldStored);

            // 'paramv'
            if (current->get(6)->getType() != types::InternalType::ScilabList)
            {
                return false;
            }
            types::List* list = current->get(6)->getAs<types::List>();

            size_t paramvSize = list->getSize();
            exprs.resize(exprs.size() + 1 + paramvSize); // Allocation for the 'paramv' strings
            std::ostringstream strParamv;
            strParamv << paramvSize;
            std::string paramvSizeStr = strParamv.str();
            exprs[6] = paramvSizeStr; // Saving the size of the 'paramv' field'

            for (size_t i = 0; i < paramvSize; ++i)
            {
                if (list->get(i)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* listElement = list->get(i)->getAs<types::String>();
                c_str = wide_string_to_UTF8(listElement->get(0));
                std::string paramvElement(c_str);
                FREE(c_str);
                exprs[7 + i] = paramvElement;
            }

            // 'pprop'
            if (current->get(7)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* ppropField = current->get(7)->getAs<types::String>();
            if (ppropField->getSize() != 1)
            {
                return false;
            }
            c_str = wide_string_to_UTF8(ppropField->get(0));
            std::string ppropFieldStored(c_str);
            FREE(c_str);
            exprs.push_back(ppropFieldStored);

            // 'nameF'
            if (current->get(8)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* nameFField = current->get(8)->getAs<types::String>();
            if (nameFField->getSize() != 1)
            {
                return false;
            }
            c_str = wide_string_to_UTF8(nameFField->get(0));
            std::string nameFFieldStored(c_str);
            FREE(c_str);
            exprs.push_back(nameFFieldStored);

            // 'funtxt'
            size_t funtxtSize;
            std::ostringstream strFuntxt;
            if (current->get(9)->getType() == types::InternalType::ScilabDouble)
            {
                types::Double* funtxtFieldDouble = current->get(9)->getAs<types::Double>();
                if (funtxtFieldDouble->getSize() != 0)
                {
                    return false;
                }
                // Allow empty matrix for 'funtxt', storing a size "0"
                funtxtSize = 0;
                strFuntxt << funtxtSize;
                std::string funtxtSizeStr = strFuntxt.str();
                exprs.push_back(funtxtSizeStr);
            }
            else
            {
                if (current->get(9)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* funtxtField = current->get(9)->getAs<types::String>();

                funtxtSize = funtxtField->getSize();
                int exprsSize = exprs.size(); // Saving last index before resizing
                exprs.resize(exprs.size() + 1 + funtxtSize); // Allocation for the 'funtxt' strings
                strFuntxt << funtxtSize;
                std::string funtxtSizeStr = strFuntxt.str();
                exprs[exprsSize] = funtxtSizeStr; // Saving the size of the 'funtxt' field'

                for (size_t i = 0; i < funtxtSize; ++i)
                {
                    c_str = wide_string_to_UTF8(funtxtField->get(i));
                    std::string funtxtElement(c_str);
                    FREE(c_str);
                    exprs[exprsSize + 1 + i] = funtxtElement;
                }
            }

            controller.setObjectProperty(adaptee, BLOCK, EXPRS, exprs);
            return true;
        }

        return false;
    }
};

struct pin
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, INPUTS, controller, v);
    }
};

struct pout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, OUTPUTS, controller, v);
    }
};

struct pein
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, EVENT_INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, EVENT_INPUTS, controller, v);
    }
};

struct peout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, EVENT_OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, EVENT_OUTPUTS, controller, v);
    }
};

struct gr_i
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getGrIContent();
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
        adaptor.setGrIContent(v->clone());
        return true;
    }
};

struct id
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string id;
        controller.getObjectProperty(adaptee, BLOCK, LABEL, id);

        types::String* o = new types::String(1, 1);
        o->set(0, id.data());

        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getSize() != 1)
        {
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();

        char* c_str = wide_string_to_UTF8(current->get(0));
        std::string id(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee, BLOCK, LABEL, id);
        return true;
    }
};

struct in_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, IMPLICIT>(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, IMPLICIT>(adaptor, INPUTS, controller, v);
    }
};

struct out_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, IMPLICIT>(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, IMPLICIT>(adaptor, OUTPUTS, controller, v);
    }
};

struct in_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, STYLE>(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, STYLE>(adaptor, INPUTS, controller, v);
    }
};

struct out_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, STYLE>(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, STYLE>(adaptor, OUTPUTS, controller, v);
    }
};

struct in_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, LABEL>(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, LABEL>(adaptor, INPUTS, controller, v);
    }
};

struct out_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, LABEL>(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, LABEL>(adaptor, OUTPUTS, controller, v);
    }
};

struct style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string style;
        controller.getObjectProperty(adaptee, BLOCK, STYLE, style);

        return new types::String(style.c_str());
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string style(c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee, BLOCK, STYLE, style);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            std::string style;
            controller.setObjectProperty(adaptee, BLOCK, STYLE, style);
            return true;
        }
        return false;
    }
};

} /* namespace */

template<> property<GraphicsAdapter>::props_t property<GraphicsAdapter>::fields = property<GraphicsAdapter>::props_t();

GraphicsAdapter::GraphicsAdapter(std::shared_ptr<model::Block> adaptee) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(adaptee),
    gr_i_content(types::Double::Empty())
{
    if (property<GraphicsAdapter>::properties_have_not_been_set())
    {
        property<GraphicsAdapter>::fields.reserve(18);
        property<GraphicsAdapter>::add_property(L"orig", &orig::get, &orig::set);
        property<GraphicsAdapter>::add_property(L"sz", &sz::get, &sz::set);
        property<GraphicsAdapter>::add_property(L"flip", &flip::get, &flip::set);
        property<GraphicsAdapter>::add_property(L"theta", &theta::get, &theta::set);
        property<GraphicsAdapter>::add_property(L"exprs", &exprs::get, &exprs::set);
        property<GraphicsAdapter>::add_property(L"pin", &pin::get, &pin::set);
        property<GraphicsAdapter>::add_property(L"pout", &pout::get, &pout::set);
        property<GraphicsAdapter>::add_property(L"pein", &pein::get, &pein::set);
        property<GraphicsAdapter>::add_property(L"peout", &peout::get, &peout::set);
        property<GraphicsAdapter>::add_property(L"gr_i", &gr_i::get, &gr_i::set);
        property<GraphicsAdapter>::add_property(L"id", &id::get, &id::set);
        property<GraphicsAdapter>::add_property(L"in_implicit", &in_implicit::get, &in_implicit::set);
        property<GraphicsAdapter>::add_property(L"out_implicit", &out_implicit::get, &out_implicit::set);
        property<GraphicsAdapter>::add_property(L"in_style", &in_style::get, &in_style::set);
        property<GraphicsAdapter>::add_property(L"out_style", &out_style::get, &out_style::set);
        property<GraphicsAdapter>::add_property(L"in_label", &in_label::get, &in_label::set);
        property<GraphicsAdapter>::add_property(L"out_label", &out_label::get, &out_label::set);
        property<GraphicsAdapter>::add_property(L"style", &style::get, &style::set);
    }
}

GraphicsAdapter::GraphicsAdapter(const GraphicsAdapter& adapter) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(adapter),
    gr_i_content(adapter.getGrIContent())
{
}

GraphicsAdapter::~GraphicsAdapter()
{
    gr_i_content->DecreaseRef();
    gr_i_content->killMe();
}

std::wstring GraphicsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring GraphicsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* GraphicsAdapter::getGrIContent() const
{
    gr_i_content->IncreaseRef();
    return gr_i_content;
}

void GraphicsAdapter::setGrIContent(types::InternalType* v)
{
    gr_i_content->DecreaseRef();
    gr_i_content->killMe();

    v->IncreaseRef();
    gr_i_content = v;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
