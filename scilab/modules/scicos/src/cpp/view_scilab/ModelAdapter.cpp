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
#include <cstring>

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include "bool.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "Adapters.hxx"
#include "ModelAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "ports_management.hxx"
#include "utilities.hxx"

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

const std::string input ("input");
const std::string output ("output");
const std::string inimpl ("inimpl");
const std::string outimpl ("outimpl");

const std::wstring modelica (L"modelica");
const std::wstring model (L"model");
const std::wstring inputs (L"inputs");
const std::wstring outputs (L"outputs");
const std::wstring parameters (L"parameters");

struct sim
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        // First, extract the function Name
        std::string name;
        controller.getObjectProperty(adaptee, BLOCK, SIM_FUNCTION_NAME, name);
        types::String* Name = new types::String(1, 1);
        Name->set(0, name.data());

        // Then the Api. If it is zero, then just return the Name. Otherwise, return a list containing both.
        int api;
        controller.getObjectProperty(adaptee, BLOCK, SIM_FUNCTION_API, api);

        if (api == 0)
        {
            return Name;
        }
        else
        {
            types::Double* Api = new types::Double(static_cast<double>(api));
            types::List* o = new types::List();
            o->set(0, Name);
            o->set(1, Api);
            return o;
        }
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
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
            std::string name(c_str);
            FREE(c_str);

            // If the input is a scalar string, then the functionApi is 0.
            int api = 0;

            controller.setObjectProperty(adaptee, BLOCK, SIM_FUNCTION_NAME, name);
            controller.setObjectProperty(adaptee, BLOCK, SIM_FUNCTION_API, api);
        }
        else if (v->getType() == types::InternalType::ScilabList)
        {
            // If the input is a 2-sized list, then it must be string and positive integer.
            types::List* current = v->getAs<types::List>();
            if (current->getSize() != 2)
            {
                return false;
            }
            if (current->get(0)->getType() != types::InternalType::ScilabString || current->get(1)->getType() != types::InternalType::ScilabDouble)
            {
                return false;
            }

            types::String* Name = current->get(0)->getAs<types::String>();
            if (Name->getSize() != 1)
            {
                return false;
            }
            char* c_str = wide_string_to_UTF8(Name->get(0));
            std::string name(c_str);
            FREE(c_str);

            types::Double* Api = current->get(1)->getAs<types::Double>();
            if (Api->getSize() != 1)
            {
                return false;
            }
            double api = Api->get(0);
            if (floor(api) != api)
            {
                return false;
            }
            int api_int = static_cast<int>(api);

            controller.setObjectProperty(adaptee, BLOCK, SIM_FUNCTION_NAME, name);
            controller.setObjectProperty(adaptee, BLOCK, SIM_FUNCTION_API, api_int);
        }
        else
        {
            return false;
        }
        return true;
    }
};

struct in
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, INPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, INPUTS, controller, v);
    }
};

struct in2
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_COLS>(adaptor, INPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, DATATYPE_COLS>(adaptor, INPUTS, controller, v);
    }
};

struct intyp
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_TYPE>(adaptor, INPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, DATATYPE_TYPE>(adaptor, INPUTS, controller, v);
    }
};

struct out
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, OUTPUTS, controller, v);
    }
};

struct out2
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_COLS>(adaptor, OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, DATATYPE_COLS>(adaptor, OUTPUTS, controller, v);
    }
};

struct outtyp
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_TYPE>(adaptor, OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, DATATYPE_TYPE>(adaptor, OUTPUTS, controller, v);
    }
};

struct evtin
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, EVENT_INPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, EVENT_INPUTS, controller, v);
    }
};

struct evtout
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, EVENT_OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, EVENT_OUTPUTS, controller, v);
    }
};

struct state
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> state;
        controller.getObjectProperty(adaptee, BLOCK, STATE, state);

        double* data;
        types::Double* o = new types::Double((int)state.size(), 1, &data);

#ifdef _MSC_VER
        std::copy(state.begin(), state.end(), stdext::checked_array_iterator<double*>(data, state.size()));
#else
        std::copy(state.begin(), state.end(), data);
#endif
        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        // Only allow vectors and empty matrices
        if (!current->isVector() && current->getSize() != 0)
        {
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> state (current->getSize());
        std::copy(current->getReal(), current->getReal() + current->getSize(), state.begin());

        controller.setObjectProperty(adaptee, BLOCK, STATE, state);
        return true;
    }
};

struct dstate
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> dstate;
        controller.getObjectProperty(adaptee, BLOCK, DSTATE, dstate);

        double* data;
        types::Double* o = new types::Double((int)dstate.size(), 1, &data);

#ifdef _MSC_VER
        std::copy(dstate.begin(), dstate.end(), stdext::checked_array_iterator<double*>(data, dstate.size()));
#else
        std::copy(dstate.begin(), dstate.end(), data);
#endif
        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            std::vector<double> dstate;
            controller.setObjectProperty(adaptee, BLOCK, DSTATE, dstate);
            return true;
        }

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }
        types::Double* current = v->getAs<types::Double>();
        // Only allow vectors and empty matrices
        if (!current->isVector() && current->getSize() != 0)
        {
            return false;
        }

        std::vector<double> dstate (current->getSize());
        std::copy(current->getReal(), current->getReal() + current->getSize(), dstate.begin());

        controller.setObjectProperty(adaptee, BLOCK, DSTATE, dstate);
        return true;
    }
};

struct odstate
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> prop_content;
        controller.getObjectProperty(adaptee, BLOCK, ODSTATE, prop_content);

        // Corner-case, the empty content is an empty double
        if (prop_content.empty())
        {
            return types::Double::Empty();
        }

        // The returned value is a list
        types::InternalType* res;
        if (!vec2var(prop_content, res))
        {
            return 0;
        }

        return res;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        // corner-case the empty content is an empty-double
        if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            // prop_content is empty
            std::vector<double> prop_content;
            controller.setObjectProperty(adaptee, BLOCK, ODSTATE, prop_content);
            return true;
        }

        std::vector<double> prop_content;
        if (!var2vec(v, prop_content))
        {
            return false;
        }

        controller.setObjectProperty(adaptee, BLOCK, ODSTATE, prop_content);
        return true;
    }
};

/*
 * When setting a diagram in 'rpar', the Superblock's ports must be consistent with the "port blocks" inside it.
 * By "port blocks", we mean IN_f, OUT_f, CLKIN_f, CLKOUT_f, CLKINV_f, CLKOUTV_f, INIMPL_f and OUTIMPL_f.
 */
bool setInnerBlocksRefs(ModelAdapter& adaptor, const std::vector<ScicosID>& children, Controller& controller)
{
    ScicosID adaptee = adaptor.getAdaptee()->id();

    for (std::vector<ScicosID>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        if (controller.getObject(*it)->kind() == BLOCK) // Rule out Annotations and Links
        {
            std::string name;
            controller.getObjectProperty(*it, BLOCK, SIM_FUNCTION_NAME, name);

            // Find the "port blocks"
            if (name == input || name == inimpl || name == output || name == outimpl)
            {
                std::vector<int> ipar;
                controller.getObjectProperty(*it, BLOCK, IPAR, ipar);
                if (ipar.size() != 1)
                {
                    return false;
                }
                int portIndex = ipar[0];

                // "name" is not enough to tell the event and data ports apart, so check the block's port.
                object_properties_t kind;
                std::vector<ScicosID> innerPort;
                if (name == input || name == inimpl)
                {
                    controller.getObjectProperty(*it, BLOCK, OUTPUTS, innerPort);
                    if (!innerPort.empty())
                    {
                        kind = INPUTS;
                    }
                    else
                    {
                        kind = EVENT_INPUTS;
                    }
                }
                else
                {
                    controller.getObjectProperty(*it, BLOCK, INPUTS, innerPort);
                    if (!innerPort.empty())
                    {
                        kind = OUTPUTS;
                    }
                    else
                    {
                        kind = EVENT_OUTPUTS;
                    }
                }

                std::vector<ScicosID> superPorts;
                controller.getObjectProperty(adaptee, BLOCK, kind, superPorts);
                if (static_cast<int>(superPorts.size()) < portIndex)
                {
                    return false;
                }

                ScicosID port = superPorts[portIndex - 1];

                // Check consistency of the implicitness between the inner and outer ports
                bool isImplicit;
                controller.getObjectProperty(port, PORT, IMPLICIT, isImplicit);
                if (name == input || name == output)
                {
                    if (isImplicit)
                    {
                        return false;
                    }
                }
                else
                {
                    if (!isImplicit)
                    {
                        return false;
                    }
                }

                controller.setObjectProperty(*it, BLOCK, PORT_REFERENCE, port);
            }
        }
    }
    return true;
}

struct rpar
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<ScicosID> diagramChildren;
        controller.getObjectProperty(adaptee, BLOCK, CHILDREN, diagramChildren);

        if (diagramChildren.empty())
        {
            std::vector<double> rpar;
            controller.getObjectProperty(adaptee, BLOCK, RPAR, rpar);

            double *data;
            types::Double* o = new types::Double((int)rpar.size(), 1, &data);
#ifdef _MSC_VER
            std::copy(rpar.begin(), rpar.end(), stdext::checked_array_iterator<double*>(data, rpar.size()));
#else
            std::copy(rpar.begin(), rpar.end(), data);
#endif
            return o;
        }
        else // SuperBlock, return the contained diagram (allocating it on demand)
        {
            DiagramAdapter* diagram = adaptor.getDiagram();

            /*
             * FIXME: Sync all diagram children as the blocks might be modified by xcos
             */

            return diagram;
        }
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();

            std::vector<double> rpar (current->getSize());
            for (int i = 0; i < current->getSize(); ++i)
            {
                rpar[i] = current->get(i);
            }

            controller.setObjectProperty(adaptee, BLOCK, RPAR, rpar);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabString)
        {
            // Allow Text blocks to define strings in rpar
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabUserType)
        {
            // Make sure the input describes a Diagram
            const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(v->getShortTypeStr());
            if (adapter_index != Adapters::DIAGRAM_ADAPTER)
            {
                return false;
            }

            // Translate 'v' to an DiagramAdapter ; copy if needed
            DiagramAdapter* diagram;
            if (v->getRef() > 1)
            {
                diagram = v->clone()->getAs<DiagramAdapter>();
            }
            else
            {
                diagram = v->getAs<DiagramAdapter>();
            }
            adaptor.setDiagram(diagram);

            // set the diagram children as block children ; referencing them
            std::vector<ScicosID> diagramChildren;
            controller.getObjectProperty(diagram->getAdaptee()->id(), DIAGRAM, CHILDREN, diagramChildren);
            std::vector<ScicosID> oldDiagramChildren;
            controller.getObjectProperty(adaptor.getAdaptee()->id(), BLOCK, CHILDREN, oldDiagramChildren);

            controller.setObjectProperty(adaptor.getAdaptee()->id(), BLOCK, CHILDREN, diagramChildren);
            {
                std::sort(oldDiagramChildren.begin(), oldDiagramChildren.end());
                for (const ScicosID id : diagramChildren)
                {
                    if (id != 0 && !std::binary_search(oldDiagramChildren.begin(), oldDiagramChildren.end(), id))
                    {
                        auto o = controller.getObject(id);
                        controller.setObjectProperty(o->id(), o->kind(), PARENT_BLOCK, adaptor.getAdaptee()->id());

                        controller.referenceObject(id);
                    }
                }

                std::sort(diagramChildren.begin(), diagramChildren.end());
                for (const ScicosID id : oldDiagramChildren)
                {
                    if (id != 0 && !std::binary_search(diagramChildren.begin(), diagramChildren.end(), id))
                    {
                        auto o = controller.getObject(id);
                        controller.setObjectProperty(o->id(), o->kind(), PARENT_BLOCK, ScicosID());

                        controller.deleteObject(id);
                    }
                }
            }

            // Link the Superblock ports to their inner "port blocks"
            return setInnerBlocksRefs(adaptor, diagramChildren, controller);
        }
        else
        {
            return false;
        }
    }
};

double toDouble(const int a)
{
    return static_cast<double>(a);
}

struct ipar
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<int> ipar;
        controller.getObjectProperty(adaptee, BLOCK, IPAR, ipar);

        double *data;
        types::Double* o = new types::Double((int)ipar.size(), 1, &data);

#ifdef _MSC_VER
        std::transform(ipar.begin(), ipar.end(), stdext::checked_array_iterator<double*>(data, ipar.size()), toDouble);
#else
        std::transform(ipar.begin(), ipar.end(), data, toDouble);
#endif
        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() == types::InternalType::ScilabList)
        {
            std::vector<int> ipar;
            controller.setObjectProperty(adaptee, BLOCK, IPAR, ipar);
            return true;
        }

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getCols() != 0 && current->getCols() != 1)
        {
            return false;
        }

        std::vector<int> ipar (current->getSize());
        for (int i = 0; i < current->getSize(); ++i)
        {
            if (floor(current->get(i)) != current->get(i))
            {
                return false;
            }
            ipar[i] = static_cast<int>(current->get(i));
        }

        controller.setObjectProperty(adaptee, BLOCK, IPAR, ipar);
        return true;
    }
};

struct opar
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> prop_content;
        controller.getObjectProperty(adaptee, BLOCK, OPAR, prop_content);

        // Corner-case, the empty content is an empty double
        if (prop_content.empty())
        {
            return types::Double::Empty();
        }

        // The returned value is a list
        types::InternalType* res;
        if (!vec2var(prop_content, res))
        {
            return 0;
        }

        return res;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        // corner-case the empty content is an empty-double
        if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            // prop_content should be empty
            std::vector<double> prop_content;
            controller.setObjectProperty(adaptee, BLOCK, OPAR, prop_content);
            return true;
        }

        std::vector<double> prop_content;
        if (!var2vec(v, prop_content))
        {
            return false;
        }

        controller.setObjectProperty(adaptee, BLOCK, OPAR, prop_content);
        return true;
    }
};

struct blocktype
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string type;
        controller.getObjectProperty(adaptee, BLOCK, SIM_BLOCKTYPE, type);

        types::String* o = new types::String(type.c_str());
        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getSize() != 1)
        {
            return false;
        }

        char* c_str = wide_string_to_UTF8(current->get(0));
        std::string type (c_str);
        FREE(c_str);

        // the value validation is performed on the model
        return controller.setObjectProperty(adaptee, BLOCK, SIM_BLOCKTYPE, type) != FAIL;
    }
};

struct firing
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, FIRING>(adaptor, EVENT_OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, FIRING>(adaptor, EVENT_OUTPUTS, controller, v);
    }
};

struct dep_ut
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<int> dep_ut;
        controller.getObjectProperty(adaptee, BLOCK, SIM_DEP_UT, dep_ut);

        int* dep;
        types::Bool* o = new types::Bool(1, 2, &dep);

        dep[0] = dep_ut[0];
        dep[1] = dep_ut[1];

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabBool)
        {
            return false;
        }

        types::Bool* current = v->getAs<types::Bool>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        std::vector<int> dep_ut (2);
        dep_ut[0] = current->get(0);
        dep_ut[1] = current->get(1);

        controller.setObjectProperty(adaptee, BLOCK, SIM_DEP_UT, dep_ut);
        return true;
    }
};

struct label
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string label;
        controller.getObjectProperty(adaptee, BLOCK, LABEL, label);

        types::String* o = new types::String(1, 1);
        o->set(0, label.data());

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
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
        std::string label(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee, BLOCK, LABEL, label);
        return true;
    }
};

struct nzcross
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<int> nzcross;
        controller.getObjectProperty(adaptee, BLOCK, NZCROSS, nzcross);

        double *data;
        types::Double* o = new types::Double((int)nzcross.size(), 1, &data);

#ifdef _MSC_VER
        std::transform(nzcross.begin(), nzcross.end(), stdext::checked_array_iterator<double*>(data, nzcross.size()), toDouble);
#else
        std::transform(nzcross.begin(), nzcross.end(), data, toDouble);
#endif
        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        // Only allow vectors and empty matrices
        if (!current->isVector() && current->getSize() != 0)
        {
            return false;
        }

        std::vector<int> nzcross (current->getSize());
        for (int i = 0; i < current->getSize(); ++i)
        {
            if (floor(current->get(i)) != current->get(i))
            {
                return false;
            }
            nzcross[i] = static_cast<int>(current->get(i));
        }

        controller.setObjectProperty(adaptee, BLOCK, NZCROSS, nzcross);
        return true;
    }
};

struct nmode
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<int> nmode;
        controller.getObjectProperty(adaptee, BLOCK, NMODE, nmode);

        double *data;
        types::Double* o = new types::Double((int)nmode.size(), 1, &data);

#ifdef _MSC_VER
        std::transform(nmode.begin(), nmode.end(), stdext::checked_array_iterator<double*>(data, nmode.size()), toDouble);
#else
        std::transform(nmode.begin(), nmode.end(), data, toDouble);
#endif
        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        // Only allow vectors and empty matrices
        if (!current->isVector() && current->getSize() != 0)
        {
            return false;
        }

        std::vector<int> nmode (current->getSize());
        for (int i = 0; i < current->getSize(); ++i)
        {
            if (floor(current->get(i)) != current->get(i))
            {
                return false;
            }
            nmode[i] = static_cast<int>(current->get(i));
        }

        controller.setObjectProperty(adaptee, BLOCK, NMODE, nmode);
        return true;
    }
};

struct equations
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<std::string> equations;
        controller.getObjectProperty(adaptee, BLOCK, EQUATIONS, equations);

        if (equations.size() == 0)
        {
            return new types::List();
        }

        types::TList* o = new types::TList();

        // Header, starting with "modelica"
        types::String* header = new types::String(1, 5);
        header->set(0, modelica.c_str());
        header->set(1, model.c_str());
        header->set(2, inputs.c_str());
        header->set(3, outputs.c_str());
        header->set(4, parameters.c_str());
        o->set(0, header);

        // 'model'
        if (equations[0].c_str() == std::string())
        {
            o->set(1, types::Double::Empty());
        }
        else
        {
            types::String* modelField = new types::String(1, 1);
            modelField->set(0, equations[0].c_str());
            o->set(1, modelField);
        }

        // 'inputs'
        std::istringstream inputsSizeStr (equations[1]);
        int inputsSize;
        inputsSizeStr >> inputsSize;
        if (inputsSize == 0)
        {
            types::Double* inputsField = types::Double::Empty();
            o->set(2, inputsField);
        }
        else
        {
            types::String* inputsField = new types::String(inputsSize, 1);
            for (int i = 0; i < inputsSize; ++i)
            {
                inputsField->set(i, equations[i + 2].c_str());
            }
            o->set(2, inputsField);
        }

        // 'outputs'
        std::istringstream outputsSizeStr (equations[2 + inputsSize]);
        int outputsSize;
        outputsSizeStr >> outputsSize;
        if (outputsSize == 0)
        {
            types::Double* outputsField = types::Double::Empty();
            o->set(3, outputsField);
        }
        else
        {
            types::String* outputsField = new types::String(outputsSize, 1);
            for (int i = 0; i < outputsSize; ++i)
            {
                outputsField->set(i, equations[i + 3 + inputsSize].c_str());
            }
            o->set(3, outputsField);
        }

        // 'parameters'
        types::List* parametersField = new types::List();

        // 'parameters' names
        std::istringstream parametersSizeStr (equations[3 + inputsSize + outputsSize]);
        int parametersSize;
        parametersSizeStr >> parametersSize;
        if (parametersSize == 0)
        {
            types::Double* parametersNames = types::Double::Empty();
            parametersField->set(0, parametersNames);
        }
        else
        {
            types::String* parametersNames = new types::String(parametersSize, 1);
            for (int i = 0; i < parametersSize; ++i)
            {
                parametersNames->set(i, equations[i + 4 + inputsSize + outputsSize].c_str());
            }
            parametersField->set(0, parametersNames);
        }

        // 'parameters' values
        types::List* parametersValues = new types::List();
        for (int i = 0; i < parametersSize; ++i)
        {
            std::istringstream parametersValueStr (equations[i + 4 + inputsSize + outputsSize + parametersSize]);
            double parametersVal;
            parametersValueStr >> parametersVal;
            types::Double* parametersValue = new types::Double(parametersVal);
            parametersValues->set(i, parametersValue);
        }
        parametersField->set(1, parametersValues);

        // 'parameters' states (optional, only check its presence if at least one parameter is present)
        if (parametersSize != 0)
        {
            std::string parametersStatesBool (equations[4 + inputsSize + outputsSize + 2 * parametersSize]);
            if (strcmp(parametersStatesBool.c_str(), "T") == 0) // Check the presence of the "states" field
            {
                types::Double* parametersStates = new types::Double(parametersSize, 1);
                for (int i = 0; i < parametersSize; ++i)
                {
                    std::istringstream parametersStateStr (equations[i + 5 + inputsSize + outputsSize + 2 * parametersSize]);
                    double parametersState;
                    parametersStateStr >> parametersState;
                    parametersStates->set(i, parametersState);
                }
                parametersField->set(2, parametersStates);
            }
        }

        o->set(4, parametersField);

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() == types::InternalType::ScilabList)
        {
            types::List* current = v->getAs<types::List>();
            if (current->getSize() != 0)
            {
                return false;
            }
            return true;
        }

        if (v->getType() != types::InternalType::ScilabTList)
        {
            return false;
        }

        types::TList* current = v->getAs<types::TList>();

        // Check the header
        types::String* header = current->getFieldNames();
        if (header->getSize() != 5)
        {
            return false;
        }
        if (header->get(0) != modelica)
        {
            return false;
        }
        if (header->get(1) != model)
        {
            return false;
        }
        if (header->get(2) != inputs)
        {
            return false;
        }
        if (header->get(3) != outputs)
        {
            return false;
        }
        if (header->get(4) != parameters)
        {
            return false;
        }

        char* c_str; // Temporary buffer used for conversions

        // 'model'
        std::vector<std::string> equations;
        if (current->get(1)->getType() == types::InternalType::ScilabString)
        {
            types::String* modelField = current->get(1)->getAs<types::String>();
            if (modelField->getSize() != 1)
            {
                return false;
            }

            c_str = wide_string_to_UTF8(modelField->get(0));
            std::string modelFieldStored(c_str);
            FREE(c_str);
            equations.push_back(modelFieldStored);
        }
        else if (current->get(1)->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* modelFieldDouble = current->get(1)->getAs<types::Double>();
            if (modelFieldDouble->getSize() != 0)
            {
                return false;
            }

            // An empty matrix stores an empty string, which will later be translated back to an empty matrix
            equations.push_back(std::string());
        }
        else
        {
            return false;
        }

        // 'inputs'
        size_t inputsSize;
        if (current->get(2)->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* inputsField = current->get(2)->getAs<types::Double>();
            if (inputsField->getSize() != 0)
            {
                return false;
            }

            inputsSize = 0;
            std::ostringstream strInputs;
            strInputs << inputsSize;
            std::string inputsSizeStr = strInputs.str();
            equations.push_back(inputsSizeStr); // When 'inputs'=[], just insert "0" in 'equations'
        }
        else
        {
            if (current->get(2)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }

            types::String* inputsField = current->get(2)->getAs<types::String>();
            inputsSize = inputsField->getSize();
            equations.resize(equations.size() + 1 + inputsSize);
            std::ostringstream strInputs;
            strInputs << inputsSize;
            std::string inputsSizeStr = strInputs.str();
            equations[1] = inputsSizeStr; // Saving the size of the 'inputs' field'
            for (size_t i = 0; i < inputsSize; ++i)
            {
                c_str = wide_string_to_UTF8(inputsField->get(static_cast<int>(i)));
                std::string inputsFieldStored(c_str);
                FREE(c_str);
                equations[i + 2] = inputsFieldStored;
            }
        }

        // 'outputs'
        size_t outputsSize;
        if (current->get(3)->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* outputsField = current->get(3)->getAs<types::Double>();
            if (outputsField->getSize() != 0)
            {
                return false;
            }

            outputsSize = 0;
            std::ostringstream strOutputs;
            strOutputs << outputsSize;
            std::string outputsSizeStr = strOutputs.str();
            equations.push_back(outputsSizeStr); // When 'outputs'=[], just insert "0" in 'equations'
        }
        else
        {
            if (current->get(3)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }

            types::String* outputsField = current->get(3)->getAs<types::String>();
            outputsSize = outputsField->getSize();
            equations.resize(equations.size() + 1 + outputsSize);
            std::ostringstream strOutputs;
            strOutputs << outputsSize;
            std::string outputsSizeStr = strOutputs.str();
            equations[2 + inputsSize] = outputsSizeStr; // Saving the size of the 'outputs' field'
            for (size_t i = 0; i < outputsSize; ++i)
            {
                c_str = wide_string_to_UTF8(outputsField->get(static_cast<int>(i)));
                std::string outputsFieldStored(c_str);
                FREE(c_str);
                equations[i + 3 + inputsSize] = outputsFieldStored;
            }
        }

        // 'parameters'
        int parametersIndex = 4;
        if (current->get(parametersIndex)->getType() == types::InternalType::ScilabDouble)
        {
            // For backward compatibility sake, allow the presence of an empty matrix here
            types::Double* emptyMatrix = current->get(parametersIndex)->getAs<types::Double>();
            if (emptyMatrix->getSize() != 0)
            {
                return false;
            }

            parametersIndex++;
        }

        if (current->get(parametersIndex)->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        types::List* list = current->get(parametersIndex)->getAs<types::List>();
        if (list->getSize() != 2 && list->getSize() != 3)
        {
            return false;
        }

        // 'parameters' names
        size_t parametersSize;
        if (list->get(0)->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* parametersNames = list->get(0)->getAs<types::Double>();
            if (parametersNames->getSize() != 0)
            {
                return false;
            }

            // When 'parameters(1)'=[], just insert "0" in 'equations', set in the model and return
            parametersSize = 0;
            std::ostringstream strParameters;
            strParameters << parametersSize;
            std::string parametersSizeStr = strParameters.str();
            equations.push_back(parametersSizeStr);

            controller.setObjectProperty(adaptee, BLOCK, EQUATIONS, equations);
            return true;
        }
        else
        {
            if (list->get(0)->getType() != types::InternalType::ScilabString)
            {
                return false;
            }

            types::String* parametersNames = list->get(0)->getAs<types::String>();
            parametersSize = parametersNames->getSize();
            equations.resize(equations.size() + 1 + parametersSize);
            std::ostringstream strParameters;
            strParameters << parametersSize;
            std::string parametersSizeStr = strParameters.str();
            equations[3 + inputsSize + outputsSize] = parametersSizeStr; // Saving the size of the 'parameters' field'
            for (size_t i = 0; i < parametersSize; ++i)
            {
                c_str = wide_string_to_UTF8(parametersNames->get(static_cast<int>(i)));
                std::string parametersName(c_str);
                FREE(c_str);
                equations[i + 4 + inputsSize + outputsSize] = parametersName;
            }
        }

        // 'parameters' values
        if (list->get(1)->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* parameterVal = list->get(1)->getAs<types::Double>();
            if (parameterVal->getSize() != static_cast<int>(parametersSize))
            {
                return false;
            }

            for (size_t i = 0; i < parametersSize; ++i)
            {
                std::ostringstream strParameterVal;
                strParameterVal << parameterVal->get(static_cast<int>(i));
                std::string parameterValStr = strParameterVal.str();
                equations.push_back(parameterValStr);
            }
        }
        else
        {
            if (list->get(1)->getType() != types::InternalType::ScilabList)
            {
                return false;
            }

            types::List* list2 = list->get(1)->getAs<types::List>();
            if (list2->getSize() != static_cast<int>(parametersSize))
            {
                return false;
            }

            equations.resize(equations.size() + parametersSize);
            for (size_t i = 0; i < parametersSize; ++i)
            {
                if (list2->get(static_cast<int>(i))->getType() != types::InternalType::ScilabDouble)
                {
                    return false;
                }

                types::Double* parametersVal = list2->get(static_cast<int>(i))->getAs<types::Double>();
                if (parametersVal->getSize() != 1)
                {
                    return false;
                }

                std::ostringstream strParametersVal;
                strParametersVal << parametersVal->get(0);
                std::string parametersValStr = strParametersVal.str();
                equations[i + 4 + inputsSize + outputsSize + parametersSize] = parametersValStr;
            }
        }

        // 'parameters' states (optional)
        equations.push_back("F"); // String boolean to indicate the presence, or not, of a "states" field
        if (list->getSize() == 3)
        {
            equations.back() = "T";
            if (list->get(2)->getType() != types::InternalType::ScilabDouble)
            {
                return false;
            }

            types::Double* parameterStates = list->get(2)->getAs<types::Double>();
            if (parameterStates->getSize() != static_cast<int>(parametersSize))
            {
                return false;
            }

            for (size_t i = 0; i < parametersSize; ++i)
            {
                std::ostringstream strParameterStates;
                strParameterStates << parameterStates->get(static_cast<int>(i));
                std::string parameterStatesStr = strParameterStates.str();
                equations.push_back(parameterStatesStr);
            }
        }

        controller.setObjectProperty(adaptee, BLOCK, EQUATIONS, equations);
        return true;
    }
};

struct uid
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string uid;
        controller.getObjectProperty(adaptee, BLOCK, UID, uid);

        types::String* o = new types::String(1, 1);
        o->set(0, uid.data());

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
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
        std::string uid(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee, BLOCK, UID, uid);
        return true;
    }
};

} /* namespace */

template<> property<ModelAdapter>::props_t property<ModelAdapter>::fields = property<ModelAdapter>::props_t();
static void initialize_fields()
{
    if (property<ModelAdapter>::properties_have_not_been_set())
    {
        property<ModelAdapter>::fields.reserve(23);
        property<ModelAdapter>::add_property(L"sim", &sim::get, &sim::set);
        property<ModelAdapter>::add_property(L"in", &in::get, &in::set);
        property<ModelAdapter>::add_property(L"in2", &in2::get, &in2::set);
        property<ModelAdapter>::add_property(L"intyp", &intyp::get, &intyp::set);
        property<ModelAdapter>::add_property(L"out", &out::get, &out::set);
        property<ModelAdapter>::add_property(L"out2", &out2::get, &out2::set);
        property<ModelAdapter>::add_property(L"outtyp", &outtyp::get, &outtyp::set);
        property<ModelAdapter>::add_property(L"evtin", &evtin::get, &evtin::set);
        property<ModelAdapter>::add_property(L"evtout", &evtout::get, &evtout::set);
        property<ModelAdapter>::add_property(L"state", &state::get, &state::set);
        property<ModelAdapter>::add_property(L"dstate", &dstate::get, &dstate::set);
        property<ModelAdapter>::add_property(L"odstate", &odstate::get, &odstate::set);
        property<ModelAdapter>::add_property(L"rpar", &rpar::get, &rpar::set);
        property<ModelAdapter>::add_property(L"ipar", &ipar::get, &ipar::set);
        property<ModelAdapter>::add_property(L"opar", &opar::get, &opar::set);
        property<ModelAdapter>::add_property(L"blocktype", &blocktype::get, &blocktype::set);
        property<ModelAdapter>::add_property(L"firing", &firing::get, &firing::set);
        property<ModelAdapter>::add_property(L"dep_ut", &dep_ut::get, &dep_ut::set);
        property<ModelAdapter>::add_property(L"label", &label::get, &label::set);
        property<ModelAdapter>::add_property(L"nzcross", &nzcross::get, &nzcross::set);
        property<ModelAdapter>::add_property(L"nmode", &nmode::get, &nmode::set);
        property<ModelAdapter>::add_property(L"equations", &equations::get, &equations::set);
        property<ModelAdapter>::add_property(L"uid", &uid::get, &uid::set);
    }
}

ModelAdapter::ModelAdapter() :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(),
    m_diagramAdapter(nullptr)
{
    initialize_fields();
}
ModelAdapter::ModelAdapter(const Controller& c, model::Block* adaptee, DiagramAdapter* diagramAdapter) :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(c, adaptee),
    m_diagramAdapter(diagramAdapter)
{
    initialize_fields();
}

ModelAdapter::~ModelAdapter()
{
}

std::wstring ModelAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring ModelAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

DiagramAdapter* ModelAdapter::getDiagram() const
{
    return m_diagramAdapter;
}

void ModelAdapter::setDiagram(DiagramAdapter* diagramAdapter)
{
    // does not increment reference as this adapter does not own the DiagramAdapter
    m_diagramAdapter = diagramAdapter;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
