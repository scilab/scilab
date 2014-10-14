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
#include <algorithm>
#include <sstream>

#include "int.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "Adapters.hxx"
#include "ModelAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "ports_management.hxx"
#include "utilities.hxx"

extern "C" {
#include "wchar.h"
#include "string.h"
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

struct sim
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        // First, extract the function Name
        std::string name;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
        types::String* Name = new types::String(1, 1);
        Name->set(0, name.data());

        // Then the Api. If it is zero, then just return the Name. Otherwise, return a list containing both.
        int api;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api);

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
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string name = std::string(c_str);
            FREE(c_str);

            // If the input is a scalar string, then the functionApi is 0.
            int api = 0;

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api);
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
            std::string name = std::string(c_str);
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

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api_int);
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
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> state;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), STATE, state);

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
        if (current->getCols() != 0 && current->getCols() != 1)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> state (current->getSize());
        std::copy(current->getReal(), current->getReal() + current->getSize(), state.begin());

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), STATE, state);
        return true;
    }
};

struct dstate
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> dstate;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), DSTATE, dstate);

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
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            std::vector<double> dstate;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), DSTATE, dstate);
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

        std::vector<double> dstate (current->getSize());
        std::copy(current->getReal(), current->getReal() + current->getSize(), dstate.begin());

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), DSTATE, dstate);
        return true;
    }
};

types::InternalType* getPropList(const ModelAdapter& adaptor, const Controller& controller, const object_properties_t prop)
{
    model::Block* adaptee = adaptor.getAdaptee();

    std::vector<int> prop_content;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), prop, prop_content);

    if (prop_content.empty())
    {
        return types::Double::Empty();
    }

    types::List* list = new types::List();

    int index = 1; // Index to each element of the returned list

    for (int i = 0; i < prop_content[0]; ++i) // 'o' must have exactly 'prop_content[0]' elements
    {
        int m, n, numberOfIntNeeded = 0;
        switch (prop_content[index])
        {
            case types::InternalType::ScilabDouble:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];
                int isComplex = prop_content[index + 3];

                double* real;
                types::Double* pDouble;

                if (isComplex == 0)
                {
                    pDouble = new types::Double(m, n, &real);
                    numberOfIntNeeded = 2 * m * n + 1;
                    memcpy(real, &prop_content[index + 3 + 1], m * n * sizeof(double));
                }
                else
                {
                    double* imag;
                    pDouble = new types::Double(m, n, &real, &imag);
                    numberOfIntNeeded = 4 * m * n + 1;
                    memcpy(real, &prop_content[index + 3 + 1], m * n * sizeof(double));
                    memcpy(imag, &prop_content[index + 3 + 1 + 2 * m * n], m * n * sizeof(double));
                }
                list->set(i, pDouble);
                break;
            }
            case types::InternalType::ScilabInt8:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];
                numberOfIntNeeded = m * n / 4;

                char* data;
                types::Int8* pInt8 = new types::Int8(m, n, &data);

                memcpy(data, &prop_content[index + 3], m * n * sizeof(char));
                list->set(i, pInt8);
                break;
            }
            case types::InternalType::ScilabUInt8:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];
                numberOfIntNeeded = m * n / 4;

                unsigned char* data;
                types::UInt8* pUInt8 = new types::UInt8(m, n, &data);

                memcpy(data, &prop_content[index + 3], m * n * sizeof(unsigned char));
                list->set(i, pUInt8);
                break;
            }
            case types::InternalType::ScilabInt16:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];
                numberOfIntNeeded = m * n / 2;

                short int* data;
                types::Int16* pInt16 = new types::Int16(m, n, &data);

                memcpy(data, &prop_content[index + 3], m * n * sizeof(short int));
                list->set(i, pInt16);
                break;
            }
            case types::InternalType::ScilabUInt16:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];
                numberOfIntNeeded = m * n / 2;

                unsigned short int* data;
                types::UInt16* pUInt16 = new types::UInt16(m, n, &data);

                memcpy(data, &prop_content[index + 3], m * n * sizeof(unsigned short int));
                list->set(i, pUInt16);
                break;
            }
            case types::InternalType::ScilabInt32:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];
                numberOfIntNeeded = m * n;

                int* data;
                types::Int32* pInt32 = new types::Int32(m, n, &data);

                memcpy(data, &prop_content[index + 3], m * n * sizeof(int));
                list->set(i, pInt32);
                break;
            }
            case types::InternalType::ScilabUInt32:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];
                numberOfIntNeeded = m * n;

                unsigned int* data;
                types::UInt32* pUInt32 = new types::UInt32(m, n, &data);

                memcpy(data, &prop_content[index + 3], m * n * sizeof(unsigned int));
                list->set(i, pUInt32);
                break;
            }
            case types::InternalType::ScilabString:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];

                types::String* pString = new types::String(m, n);

                for (int j = 0; j < m * n; ++j)
                {
                    int strLen = prop_content[index + 3 + numberOfIntNeeded];
                    wchar_t* str = new wchar_t[strLen + 1];
                    memcpy(str, &prop_content[index + 3 + numberOfIntNeeded + 1], strLen * sizeof(wchar_t));
                    str[strLen] = '\0';
                    pString->set(j, str);
                    delete str;

                    numberOfIntNeeded += 1 + strLen;
                }
                list->set(i, pString);
                break;
            }
            case types::InternalType::ScilabBool:
            {
                m = prop_content[index + 1];
                n = prop_content[index + 2];
                numberOfIntNeeded = m * n;

                int* data;
                types::Bool* pBool = new types::Bool(m, n, &data);

                memcpy(data, &prop_content[index + 3], m * n * sizeof(int));
                list->set(i, pBool);
                break;
            }
            default:
                return 0;
        }

        index += 3 + numberOfIntNeeded;
    }

    return list;
}

bool setPropList(ModelAdapter& adaptor, Controller& controller, const object_properties_t prop, types::InternalType* v)
{
    model::Block* adaptee = adaptor.getAdaptee();

    if (v->getType() == types::InternalType::ScilabDouble)
    {
        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 0)
        {
            return false;
        }

        std::vector<int> prop_content;
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), prop, prop_content);
        return true;
    }

    if (v->getType() != types::InternalType::ScilabList)
    {
        return false;
    }

    types::List* list = v->getAs<types::List>();

    // 'prop_content' will be a buffer containing the elements of the list, copied into 'int' type by bits
    std::vector<int> prop_content (1, list->getSize()); // Save the number of list elements in the first element
    int index = 1; // Index to point at every new list element

    for (int i = 0; i < list->getSize(); ++i)
    {
        // Save the variable type
        prop_content.resize(prop_content.size() + 1);
        prop_content[index] = list->get(i)->getType();

        int m, n, numberOfIntNeeded = 0;
        switch (list->get(i)->getType())
        {
            case types::InternalType::ScilabDouble:
            {
                types::Double* pDouble = list->get(i)->getAs<types::Double>();
                m = pDouble->getRows();
                n = pDouble->getCols();

                if (!pDouble->isComplex())
                {
                    // It takes 2 int (4 bytes) to save 1 real (1 double: 8 bytes)
                    // So reserve 2*m*n, 2 integers for the matrix dimensions and 1 for the complexity
                    numberOfIntNeeded = 2 * m * n + 1;
                    prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);
                    prop_content[index + 3] = 0; // Flag for real

                    // Using contiguity of the memory, we save the input into 'prop_content'
                    memcpy(&prop_content[index + 3 + 1], pDouble->getReal(), m * n * sizeof(double));
                }
                else
                {
                    // It takes 4 int (4 bytes) to save 1 complex (2 double: 16 bytes)
                    // So reserve 2*m*n, 2 integers for the matrix dimensions and 1 for the complexity
                    numberOfIntNeeded = 4 * m * n + 1;
                    prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);
                    prop_content[index + 3] = 1; // Flag for complex

                    // Contiguously save the real and complex parts
                    memcpy(&prop_content[index + 3 + 1], pDouble->getReal(), m * n * sizeof(double));
                    memcpy(&prop_content[index + 3 + 1 + 2 * m * n], pDouble->getImg(), m * n * sizeof(double));
                }
                break;
            }
            case types::InternalType::ScilabInt8:
            {
                types::Int8* pInt8 = list->get(i)->getAs<types::Int8>();
                m = pInt8->getRows();
                n = pInt8->getCols();

                // It takes 1 int (4 bytes) to save 4 char (1 byte)
                // So reserve m*n/4 and 2 integers for the matrix dimensions
                numberOfIntNeeded = m * n / 4;
                prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);

                // Using contiguity of the memory, we save the input into 'prop_content'
                memcpy(&prop_content[index + 3], pInt8->get(), m * n * sizeof(char));
                break;
            }
            case types::InternalType::ScilabUInt8:
            {
                types::Int16* pInt16 = list->get(i)->getAs<types::Int16>();
                m = pInt16->getRows();
                n = pInt16->getCols();

                // It takes 1 int (4 bytes) to save 4 unsigned char (1 byte)
                // So reserve m*n/4 and 2 integers for the matrix dimensions
                numberOfIntNeeded = m * n / 4;
                prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);

                // Using contiguity of the memory, we save the input into 'prop_content'
                memcpy(&prop_content[index + 3], pInt16->get(), m * n * sizeof(unsigned char));
                break;
            }
            case types::InternalType::ScilabInt16:
            {
                types::Int16* pInt16 = list->get(i)->getAs<types::Int16>();
                m = pInt16->getRows();
                n = pInt16->getCols();

                // It takes 1 int (4 bytes) to save 2 short int (2 bytes)
                // So reserve m*n/2 and 2 integers for the matrix dimensions
                numberOfIntNeeded = m * n / 2;
                prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);

                // Using contiguity of the memory, we save the input into 'prop_content'
                memcpy(&prop_content[index + 3], pInt16->get(), m * n * sizeof(short int));
                break;
            }
            case types::InternalType::ScilabUInt16:
            {
                types::UInt16* pUInt16 = list->get(i)->getAs<types::UInt16>();
                m = pUInt16->getRows();
                n = pUInt16->getCols();

                // It takes 1 int (4 bytes) to save 2 unsigned short int (2 bytes)
                // So reserve m*n/2 and 2 integers for the matrix dimensions
                numberOfIntNeeded = m * n / 2;
                prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);

                // Using contiguity of the memory, we save the input into prop_content
                memcpy(&prop_content[index + 3], pUInt16->get(), m * n * sizeof(unsigned short int));
                break;
            }
            case types::InternalType::ScilabInt32:
            {
                types::Int32* pInt32 = list->get(i)->getAs<types::Int32>();
                m = pInt32->getRows();
                n = pInt32->getCols();

                // It takes 1 int (4 bytes) to save 1 int (4 bytes)
                // So reserve m*n and 2 integers for the matrix dimensions
                numberOfIntNeeded = m * n;
                prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);

                // Using contiguity of the memory, we save the input into 'prop_content'
                memcpy(&prop_content[index + 3], pInt32->get(), m * n * sizeof(int));
                break;
            }
            case types::InternalType::ScilabUInt32:
            {
                types::UInt32* pUInt32 = list->get(i)->getAs<types::UInt32>();
                m = pUInt32->getRows();
                n = pUInt32->getCols();

                // It takes 1 int (4 bytes) to save 1 unsigned int (4 bytes)
                // So reserve m*n and 2 integers for the matrix dimensions
                numberOfIntNeeded = m * n;
                prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);

                // Using contiguity of the memory, we save the input into 'prop_content'
                memcpy(&prop_content[index + 3], pUInt32->get(), m * n * sizeof(unsigned int));
                break;
            }
            case types::InternalType::ScilabInt64:
            case types::InternalType::ScilabUInt64:
                // int64 and uint64 are not treated yet
                return false;
            case types::InternalType::ScilabString:
            {
                types::String* pString = list->get(i)->getAs<types::String>();
                m = pString->getRows();
                n = pString->getCols();

                // For the moment, we don't know how many characters each string is long, so only reserve the matrix size
                prop_content.resize(prop_content.size() + 2);

                for (int j = 0; j < m * n; ++j)
                {
                    // Extract the input string length and reserve as many characters in 'prop_content'
                    int strLen = static_cast<int>(wcslen(pString->get(j)));
                    prop_content.resize(prop_content.size() + 1 + strLen);
                    prop_content[index + 3 + numberOfIntNeeded] = strLen;

                    memcpy(&prop_content[index + 3 + numberOfIntNeeded + 1], pString->get(j), strLen * sizeof(wchar_t));
                    numberOfIntNeeded += 1 + strLen;
                }
                break;
            }
            case types::InternalType::ScilabBool:
            {
                types::Bool* pBool = list->get(i)->getAs<types::Bool>();
                m = pBool->getRows();
                n = pBool->getCols();

                // It takes 1 int (4 bytes) to save 1 bool (1 int: 4 byte)
                // So reserve m*n and 2 integers for the matrix dimensions
                numberOfIntNeeded = m * n;
                prop_content.resize(prop_content.size() + 2 + numberOfIntNeeded);

                // Using contiguity of the memory, we save the input into 'prop_content'
                memcpy(&prop_content[index + 3], pBool->get(), m * n * sizeof(int));
                break;
            }
            default:
                return false;
        }
        // Save the matrix dimensions in 'prop_content' and increment index to match the next list element
        prop_content[index + 1] = m;
        prop_content[index + 2] = n;
        index += 3 + numberOfIntNeeded;
    }

    controller.setObjectProperty(adaptee->id(), adaptee->kind(), prop, prop_content);
    return true;
}

struct odstate
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return getPropList(adaptor, controller, ODSTATE);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return setPropList(adaptor, controller, ODSTATE, v);
    }
};

/*
 * When setting a diagram in 'rpar', the Superblock's ports must be consistent with the "port blocks" inside it.
 * By "port blocks", we mean IN_f, OUT_f, CLKIN_f, CLKOUT_f, CLKINV_f, CLKOUTV_f, INIMPL_f and OUTIMPL_f.
 */
bool setInnerBlocksRefs(ModelAdapter& adaptor, const std::vector<ScicosID>& children, Controller& controller)
{
    model::Block* adaptee = adaptor.getAdaptee();

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
                controller.getObjectProperty(adaptee->id(), adaptee->kind(), kind, superPorts);
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

            // Regardless of the ports, use the loop to set each Block's 'parent_block' property
            controller.setObjectProperty(*it, BLOCK, PARENT_BLOCK, adaptee->id());
        }
    }
    return true;
}

struct rpar
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

        if (children.empty())
        {
            std::vector<double> rpar;
            controller.getObjectProperty(adaptee->id(), adaptee->kind(), RPAR, rpar);

            double *data;
            types::Double* o = new types::Double((int)rpar.size(), 1, &data);
#ifdef _MSC_VER
            std::copy(rpar.begin(), rpar.end(), stdext::checked_array_iterator<double*>(data, rpar.size()));
#else
            std::copy(rpar.begin(), rpar.end(), data);
#endif
            return o;
        }
        else // SuperBlock, return the contained diagram, whose ID is stored in children[0]
        {
            model::Diagram* diagram = static_cast<model::Diagram*>(Controller().getObject(children[0]));
            DiagramAdapter* o = new DiagramAdapter(false, diagram);
            return o;
        }
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();

            std::vector<double> rpar (current->getSize());
            for (int i = 0; i < current->getSize(); ++i)
            {
                rpar[i] = current->get(i);
            }

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), RPAR, rpar);
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

            // Translate 'v' to an DiagramAdapter and clone it, updating the new Diagram's children
            DiagramAdapter* diagram = v->getAs<DiagramAdapter>();
            ScicosID clone = controller.cloneObject(diagram->getAdaptee()->id());
            model::Diagram* newSubAdaptee = static_cast<model::Diagram*>(controller.getObject(clone));
            DiagramAdapter* newDiagram = new DiagramAdapter(true, newSubAdaptee);

            // Save the children list, adding the new diagram ID at the beginning
            std::vector<ScicosID> children;
            controller.getObjectProperty(newSubAdaptee->id(), newSubAdaptee->kind(), CHILDREN, children);
            children.insert(children.begin(), newSubAdaptee->id());
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

            // Link the Superblock ports to their inner "port blocks"
            return setInnerBlocksRefs(adaptor, children, controller);
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
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<int> ipar;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), IPAR, ipar);

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
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() == types::InternalType::ScilabList)
        {
            std::vector<int> ipar;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), IPAR, ipar);
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

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), IPAR, ipar);
        return true;
    }
};

struct opar
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return getPropList(adaptor, controller, OPAR);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return setPropList(adaptor, controller, OPAR, v);
    }
};

struct blocktype
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string type;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_BLOCKTYPE, type);

        types::String* o = new types::String(type.c_str());
        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

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
        return controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_BLOCKTYPE, type) != FAIL;
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
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<int> dep_ut;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_DEP_UT, dep_ut);

        int* dep;
        types::Bool* o = new types::Bool(1, 2, &dep);

        dep[0] = dep_ut[0];
        dep[1] = dep_ut[1];

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

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

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_DEP_UT, dep_ut);
        return true;
    }
};

struct label
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string label;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), LABEL, label);

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

        model::Block* adaptee = adaptor.getAdaptee();

        std::string label;
        char* c_str = wide_string_to_UTF8(current->get(0));
        label = std::string(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), LABEL, label);
        return true;
    }
};

struct nzcross
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        int nzcross;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), NZCROSS, nzcross);

        types::Double* o = new types::Double(static_cast<double>(nzcross));

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        int nzcross = 0; // Default value
        if (current->getSize() != 0)
        {
            if (current->getSize() != 1)
            {
                return false;
            }
            if (floor(current->get(0)) != current->get(0))
            {
                return false;
            }

            nzcross = static_cast<int>(current->get(0));
        }

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), NZCROSS, nzcross);
        return true;
    }
};

struct nmode
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        int nmode;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), NMODE, nmode);

        types::Double* o = new types::Double(static_cast<double>(nmode));

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        int nmode = 0; // Default value
        if (current->getSize() != 0)
        {
            if (current->getSize() != 1)
            {
                return false;
            }
            if (floor(current->get(0)) != current->get(0))
            {
                return false;
            }

            nmode = static_cast<int>(current->get(0));
        }

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), NMODE, nmode);
        return true;
    }
};

struct equations
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) adaptor;
        (void) controller;

        // FIXME: implement as a stored modelica equations

        // Return a default empty list.
        return new types::List();
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        types::List* current = v->getAs<types::List>();

        if (current->getSize() == 0)
        {
            return true;
        }
        else
        {
            // silent unused parameter warnings
            (void) adaptor;
            (void) v;
            (void) controller;

            // FIXME: implement as a stored modelica equations
            // FIXME: get the input list and store it in the equations field
            return false;
        }
    }
};

struct uid
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string uid;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), UID, uid);

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

        model::Block* adaptee = adaptor.getAdaptee();

        std::string uid;
        char* c_str = wide_string_to_UTF8(current->get(0));
        uid = std::string(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), UID, uid);
        return true;
    }
};

} /* namespace */

template<> property<ModelAdapter>::props_t property<ModelAdapter>::fields = property<ModelAdapter>::props_t();

ModelAdapter::ModelAdapter(bool ownAdaptee, org_scilab_modules_scicos::model::Block* adaptee) :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(ownAdaptee, adaptee)
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

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
