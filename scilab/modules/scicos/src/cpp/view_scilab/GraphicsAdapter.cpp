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

#include "list.hxx"
#include "tlist.hxx"
#include "double.hxx"
#include "string.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "GraphicsAdapter.hxx"
#include "ports_management.hxx"

#include "var2vec.hxx"
#include "vec2var.hxx"

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
const std::wstring MPBLOCK (L"MPBLOCK");
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

        std::vector<double> exprs;
        controller.getObjectProperty(adaptee, BLOCK, EXPRS, exprs);

        types::InternalType* res;
        if (!vec2var(exprs, res))
        {
            return 0;
        }
        return res;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        // From here on, only perform check on 'v'. If it is correct, encode it in the model via 'var2vec'
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (!current->isEmpty())
            {
                return false;
            }

        }
        // All the following cases are meant for blocks:
        // CBLOCK, fortran_block, EXPRESSION, scifunc_block_m, PDE, CONSTRAINT2_c, LOOKUP2D, MBLOCK and MPBLOCK
        else if (v->getType() == types::InternalType::ScilabList)
        {
            types::List* initial_list = v->getAs<types::List>();

            if (initial_list->getSize() == 2)
            {
                size_t nParams = 0;
                // Whatever the block kind, the first element is necessarily a string matrix (can be empty)
                if (initial_list->get(0)->getType() == types::InternalType::ScilabDouble)
                {
                    types::Double* empty_matrix_expected = initial_list->get(0)->getAs<types::Double>();
                    if (empty_matrix_expected->getSize() != 0)
                    {
                        return false;
                    }
                }
                else if (initial_list->get(0)->getType() == types::InternalType::ScilabString)
                {
                    types::String* initial_string = initial_list->get(0)->getAs<types::String>();
                    nParams = initial_string->getSize();
                }
                else
                {
                    return false;
                }

                // The second element determines the block kind
                if (initial_list->get(1)->getType() == types::InternalType::ScilabDouble)
                {
                    types::Double* empty_matrix_expected = initial_list->get(1)->getAs<types::Double>();
                    if (!empty_matrix_expected->isEmpty())
                    {
                        return false;
                    }
                    // List coming from a C block
                }
                else if (initial_list->get(1)->getType() == types::InternalType::ScilabString)
                {
                    types::String* second_string = initial_list->get(1)->getAs<types::String>();
                    // List coming from a "user-defined function" block
                }
                else if (initial_list->get(1)->getType() == types::InternalType::ScilabList)
                {
                    types::List* second_list = initial_list->get(1)->getAs<types::List>();

                    if (second_list->getSize() == 1)
                    {
                        // Default fortran_block value: must contain an empty matrix
                        if (second_list->get(0)->getType() != types::InternalType::ScilabDouble)
                        {
                            return false;
                        }
                        types::Double* empty = second_list->get(0)->getAs<types::Double>();
                        if (!empty->isEmpty())
                        {
                            return false;
                        }
                        // List coming from a fortran_block
                    }
                    else if (second_list->getSize() == 3)
                    {
                        // List coming from a SuperBlock (masked or not)
                        // The three elements are: parameter names, title message and parameters types & sizes

                        // Parameters names (string matrix, can be empty)
                        if (second_list->get(0)->getType() == types::InternalType::ScilabDouble)
                        {
                            types::Double* empty_matrix_expected = second_list->get(0)->getAs<types::Double>();
                            if (empty_matrix_expected->getSize() != 0)
                            {
                                return false;
                            }
                            // No parameters are present, so nothing needs to be saved
                        }
                        else if (second_list->get(0)->getType() == types::InternalType::ScilabString)
                        {
                            types::String* second_string = second_list->get(0)->getAs<types::String>();

                            if (second_string->getSize() != static_cast<int>(nParams))
                            {
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }

                        // Title message (string matrix, can be empty)
                        if (second_list->get(1)->getType() == types::InternalType::ScilabDouble)
                        {
                            types::Double* empty_matrix_expected = second_list->get(1)->getAs<types::Double>();
                            if (empty_matrix_expected->getSize() != 0 || nParams != 0)
                            {
                                return false;
                            }
                            // No parameters are present, so nothing needs to be saved
                        }
                        else if (second_list->get(1)->getType() == types::InternalType::ScilabString)
                        {
                            types::String* title_message = second_list->get(1)->getAs<types::String>();
                            if (title_message->getSize() != static_cast<int>(nParams + 1))
                            {
                                // There must be as many parameter descriptions as there are parameters, plus the title message
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }

                        // Parameters types & sizes (list mixing strings and integers, can be empty)
                        if (second_list->get(2)->getType() != types::InternalType::ScilabList)
                        {
                            return false;
                        }
                        types::List* third_list = second_list->get(2)->getAs<types::List>();

                        if (third_list->getSize() != static_cast<int>(2 * nParams))
                        {
                            // There must be one type and one size for each parameter, so '2*nParams' elements
                            if (!(third_list->getSize() == 1 && nParams == 0))
                            {
                                // Allow third_list != 2*params only for the dummy case 'third_list=list([])'. Do nothing then
                                return false;
                            }
                        }
                    }
                    else if (second_list->getSize() == 7)
                    {
                        // List coming from a scifunc block

                        // Code parts (string matrices)
                        for (int i = 0; i < 7; ++i)
                        {
                            if (second_list->get(i)->getType() == types::InternalType::ScilabDouble)
                            {
                                types::Double* ith_double = second_list->get(i)->getAs<types::Double>();
                                if (!ith_double->isEmpty())
                                {
                                    return false;
                                }
                            }
                            else if (second_list->get(i)->getType() != types::InternalType::ScilabString)
                            {
                                return false;
                            }
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else if (initial_list->getSize() == 3)
            {
                // List coming from a PDE block
                if (initial_list->get(0)->getType() == types::InternalType::ScilabTList)
                {
                    types::TList* ParamsPDE = initial_list->get(0)->getAs<types::TList>();

                    types::String* header = ParamsPDE->getFieldNames();
                    int nParams = header->getSize() - 1; // The first element is the TList type
                    // Check the header
                    for (int i = 1; i < nParams; ++i) // Stop at 'nParams-1' to treat the last element differently (string matrix)
                    {
                        // Its corresponding element in the TList
                        if (ParamsPDE->get(i)->getType() != types::InternalType::ScilabString)
                        {
                            return false;
                        }
                        types::String* ith_string = ParamsPDE->get(i)->getAs<types::String>();
                        if (!ith_string->isScalar())
                        {
                            return false;
                        }
                    }
                    // Header element for the last element of ParamsPDE
                    if (ParamsPDE->get(nParams)->getType() != types::InternalType::ScilabString)
                    {
                        return false;
                    }
                    types::String* last_string = ParamsPDE->get(nParams)->getAs<types::String>();

                    // Next comes some code
                    if (initial_list->get(1)->getType() == types::InternalType::ScilabDouble)
                    {
                        types::Double* empty_matrix_expected = initial_list->get(1)->getAs<types::Double>();
                        if (!empty_matrix_expected->isEmpty())
                        {
                            return false;
                        }
                    }
                    else if (initial_list->get(1)->getType() == types::InternalType::ScilabString)
                    {
                        types::String* code = initial_list->get(1)->getAs<types::String>();
                    }
                    else
                    {
                        return false;
                    }

                    // Finally, the block name
                    if (initial_list->get(2)->getType() != types::InternalType::ScilabString)
                    {
                        return false;
                    }
                    types::String* name = initial_list->get(2)->getAs<types::String>();
                    if (!name->isScalar())
                    {
                        return false;
                    }
                }
                else if (initial_list->get(0)->getType() == types::InternalType::ScilabString)
                {
                    // List coming from a CONSTRAINT2_c block
                    types::String* x = initial_list->get(0)->getAs<types::String>();
                    if (!x->isScalar())
                    {
                        return false;
                    }

                    if (initial_list->get(1)->getType() != types::InternalType::ScilabString)
                    {
                        return false;
                    }
                    types::String* xd = initial_list->get(1)->getAs<types::String>();
                    if (!xd->isScalar())
                    {
                        return false;
                    }

                    if (initial_list->get(2)->getType() != types::InternalType::ScilabString)
                    {
                        return false;
                    }
                    types::String* id = initial_list->get(2)->getAs<types::String>();
                    if (!id->isScalar())
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else if (initial_list->getSize() == 5)
            {
                // List coming from a LOOKUP2D block
                types::String* xx = initial_list->get(0)->getAs<types::String>();
                if (!xx->isScalar())
                {
                    return false;
                }

                if (initial_list->get(1)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* yy = initial_list->get(1)->getAs<types::String>();
                if (!yy->isScalar())
                {
                    return false;
                }

                if (initial_list->get(2)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* zz = initial_list->get(2)->getAs<types::String>();
                if (!zz->isScalar())
                {
                    return false;
                }

                if (initial_list->get(3)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* Method = initial_list->get(3)->getAs<types::String>();
                if (!Method->isScalar())
                {
                    return false;
                }

                if (initial_list->get(4)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* Graf = initial_list->get(4)->getAs<types::String>();
                if (!Graf->isScalar())
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
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
            if (header->get(0) != MBLOCK && header->get(0) != MPBLOCK)
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

            // 'in'
            if (current->get(1)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* inField = current->get(1)->getAs<types::String>();
            if (!inField->isScalar())
            {
                return false;
            }

            // 'intype'
            if (current->get(2)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* intypeField = current->get(2)->getAs<types::String>();
            if (!intypeField->isScalar())
            {
                return false;
            }

            // 'out'
            if (current->get(3)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* outField = current->get(3)->getAs<types::String>();
            if (!inField->isScalar())
            {
                return false;
            }

            // 'outtype'
            if (current->get(4)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* outtypeField = current->get(4)->getAs<types::String>();
            if (!outtypeField->isScalar())
            {
                return false;
            }

            // 'param'
            if (current->get(5)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* paramField = current->get(5)->getAs<types::String>();
            if (!paramField->isScalar())
            {
                return false;
            }

            // 'paramv'
            if (current->get(6)->getType() != types::InternalType::ScilabList)
            {
                return false;
            }
            types::List* list = current->get(6)->getAs<types::List>();

            for (size_t i = 0; i < list->getSize(); ++i)
            {
                if (list->get(static_cast<int>(i))->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
            }

            // 'pprop'
            if (current->get(7)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* ppropField = current->get(7)->getAs<types::String>();
            if (!ppropField->isScalar())
            {
                return false;
            }

            // 'nameF'
            if (current->get(8)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }
            types::String* nameFField = current->get(8)->getAs<types::String>();
            if (!nameFField->isScalar())
            {
                return false;
            }

            // 'funtxt'
            if (current->get(9)->getType() == types::InternalType::ScilabDouble)
            {
                types::Double* funtxtFieldDouble = current->get(9)->getAs<types::Double>();
                if (funtxtFieldDouble->getSize() != 0)
                {
                    return false;
                }
            }
            else
            {
                if (current->get(9)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* funtxtField = current->get(9)->getAs<types::String>();
            }
        }
        else
        {
            return false;
        }

        // 'v' is OK, now perform the actual encoding
        std::vector<double> exprs;
        if (!var2vec(v, exprs))
        {
            return false;
        }
        controller.setObjectProperty(adaptee, BLOCK, EXPRS, exprs);
        return true;
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
        if (!current->isScalar())
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
            if (!current->isScalar())
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
static void initialize_fields()
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

GraphicsAdapter::GraphicsAdapter() :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(),
    gr_i_content(types::Double::Empty())
{
    initialize_fields();
}

GraphicsAdapter::GraphicsAdapter(const Controller& c, model::Block* adaptee) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(c, adaptee),
    gr_i_content(types::Double::Empty())
{
    initialize_fields();
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
