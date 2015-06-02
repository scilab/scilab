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

#include "list.hxx"
#include "tlist.hxx"
#include "double.hxx"
#include "string.hxx"

#include "utilities.hxx"
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

        std::vector<std::string> exprs;
        controller.getObjectProperty(adaptee, BLOCK, EXPRS, exprs);

        if (exprs.empty())
        {
            return 0;
        }
        else if (exprs[0] == "List_fortran")
        {
            // Get and return a list
            types::List* o = new types::List();

            // Element #1: Block parameters
            std::istringstream nParamsStr (exprs[1]);
            int nParams;
            nParamsStr >> nParams;
            if (nParams == 0)
            {
                types::Double* paramsField = types::Double::Empty();
                o->append(paramsField);
            }
            else
            {
                types::String* paramsField = new types::String(nParams, 1);
                for (int i = 0; i < nParams; ++i)
                {
                    paramsField->set(i, exprs[2 + i].c_str());
                }
                o->append(paramsField);
            }

            // Element #2: List containing an empty matrix (default value)
            types::List* emptyMatrixList = new types::List();
            types::Double* emptyMatrix = types::Double::Empty();
            emptyMatrixList->append(emptyMatrix);
            o->append(emptyMatrixList);

            return o;
        }
        else if (exprs[0] == "List_scifunc")
        {
            // Get and return a list
            types::List* o = new types::List();

            // Element #1: Block parameters
            std::istringstream nParamsStr (exprs[1]);
            int nParams;
            nParamsStr >> nParams;
            if (nParams == 0)
            {
                types::Double* paramsField = types::Double::Empty();
                o->append(paramsField);
            }
            else
            {
                types::String* paramsField = new types::String(nParams, 1);
                for (int i = 0; i < nParams; ++i)
                {
                    paramsField->set(i, exprs[2 + i].c_str());
                }
                o->append(paramsField);
            }

            // Element #2: Code parts
            types::List* codePartsField = new types::List();

            for (int i = 0; i < 7; ++i)
            {
                std::istringstream nLinesStr (exprs[2 + nParams]);
                int nLines;
                nLinesStr >> nLines;
                if (nLines == 0)
                {
                    // Null 'nLines' signals an empty matrix
                    types::Double* empty_matrix = types::Double::Empty();
                    codePartsField->append(empty_matrix);
                }
                else
                {
                    types::String* part = new types::String(nLines, 1);
                    for (int j = 0; j < nLines; ++j)
                    {
                        part->set(j, exprs[2 + nParams + 1 + j].c_str());
                    }
                    codePartsField->append(part);
                }

                nParams += nLines + 1;
            }
            o->append(codePartsField);

            return o;
        }
        else if (exprs[0] == "List_code" || exprs[0] == "List_super")
        {
            // Get and return a list
            types::List* o = new types::List();

            // Element #1: Default values
            std::istringstream nParamsStr (exprs[1]);
            int nParams;
            nParamsStr >> nParams;
            if (nParams == 0)
            {
                types::Double* defaultValuesField = types::Double::Empty();
                o->append(defaultValuesField);
            }
            else
            {
                types::String* defaultValuesField = new types::String(nParams, 1);
                for (int i = 0; i < nParams; ++i)
                {
                    defaultValuesField->set(i, exprs[2 + i].c_str());
                }
                o->append(defaultValuesField);
            }

            // Element #2 depends on the block kind
            if (exprs[0] == "List_code")
            {
                std::istringstream nLinesStr (exprs[2 + nParams]);
                int nLines;
                nLinesStr >> nLines;
                if (nLines == 0)
                {
                    types::Double* functionBodyField = types::Double::Empty();
                    o->append(functionBodyField);
                }
                else
                {
                    types::String* functionBodyField = new types::String(nLines, 1);
                    for (int i = 0; i < nLines; ++i)
                    {
                        functionBodyField->set(i, exprs[2 + nParams + 1 + i].c_str());
                    }
                    o->append(functionBodyField);
                }
            }
            else // List_super
            {
                types::List* secondElement = new types::List();

                // Paramaters names
                if (nParams == 0)
                {
                    types::Double* namesField = types::Double::Empty();
                    secondElement->append(namesField);
                }
                else
                {
                    types::String* namesField = new types::String(nParams, 1);
                    for (int i = 0; i < nParams; ++i)
                    {
                        namesField->set(i, exprs[2 + nParams + i].c_str());
                    }
                    secondElement->append(namesField);
                }

                // Title message and paramaters decriptions
                types::String* titleField = new types::String(nParams + 1, 1);
                for (int i = 0; i < nParams + 1; ++i)
                {
                    titleField->set(i, exprs[2 + 2 * nParams + i].c_str());
                }
                secondElement->append(titleField);

                // Parameters types and sizes
                types::List* typesAndSizes = new types::List();

                if (nParams == 0)
                {
                    types::Double* typeSizeField = types::Double::Empty();
                    typesAndSizes->append(typeSizeField);
                }
                else
                {
                    for (int i = 0; i < nParams; ++i)
                    {
                        // Read type
                        types::String* Type = new types::String(exprs[2 + 3 * nParams + 1 + i].c_str());
                        typesAndSizes->append(Type);

                        // Read size
                        std::istringstream sizeStr (exprs[2 + 4 * nParams + 1 + i]);
                        int size;
                        sizeStr >> size;
                        types::Double* Size = new types::Double(size);
                        typesAndSizes->append(Size);
                    }
                }
                secondElement->append(typesAndSizes);

                o->append(secondElement);
            }

            return o;
        }
        else if (exprs[0] == "List_MBLOCK" || exprs[0] == "List_MPBLOCK")
        {
            // Get and return a Modelica tlist
            types::TList* o = new types::TList();

            // Header, starting with "List_MBLOCK"
            types::String* header = new types::String(1, 10);
            if (exprs[0] == "List_MBLOCK")
            {
                header->set(0, MBLOCK.c_str());
            }
            else
            {
                header->set(0, MPBLOCK.c_str());
            }
            header->set(1, in.c_str());
            header->set(2, intype.c_str());
            header->set(3, out.c_str());
            header->set(4, outtype.c_str());
            header->set(5, param.c_str());
            header->set(6, paramv.c_str());
            header->set(7, pprop.c_str());
            header->set(8, nameF.c_str());
            header->set(9, funtxt.c_str());
            o->append(header);

            // 'in'
            types::String* inField = new types::String(exprs[1].c_str());
            o->append(inField);

            // 'intype'
            types::String* intypeField = new types::String(exprs[2].c_str());
            o->append(intypeField);

            // 'out'
            types::String* outField = new types::String(exprs[3].c_str());
            o->append(outField);

            // 'outtype'
            types::String* outtypeField = new types::String(exprs[4].c_str());
            o->append(outtypeField);

            // 'param'
            types::String* paramField = new types::String(exprs[5].c_str());
            o->append(paramField);

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
            o->append(paramvField);

            // 'pprop'
            types::String* ppropField = new types::String(exprs[7 + paramvSize].c_str());
            o->append(ppropField);

            // 'nameF'
            types::String* nameFField = new types::String(exprs[7 + paramvSize + 1].c_str());
            o->append(nameFField);

            // 'funtxt'
            std::istringstream funtxtSizeStr (exprs[7 + paramvSize + 2]);
            int funtxtSize;
            funtxtSizeStr >> funtxtSize;

            if (funtxtSize == 0)
            {
                // An empty 'funtxt' field returns an empty matrix
                o->append(types::Double::Empty());
                return o;
            }
            types::String* funtxtField = new types::String(funtxtSize, 1);
            for (int i = 0; i < funtxtSize; ++i)
            {
                funtxtField->set(i, exprs[7 + paramvSize + 3 + i].c_str());
            }
            o->append(funtxtField);

            return o;
        }
        else if (exprs[0] == "List_PDE")
        {
            // Get and return a PDE list
            types::List* o = new types::List();

            // Parameters
            types::TList* params = new types::TList();

            // First, the header
            std::istringstream nParamsStr (exprs[1]);
            int nParams;
            nParamsStr >> nParams;
            types::String* header = new types::String(nParams + 1, 1); // The first element of the header is the TList type
            for (int i = 0; i < nParams + 1; ++i)
            {
                header->set(i, exprs[2 + i].c_str());
            }
            params->append(header);

            // Then, the content
            for (int i = 0; i < nParams - 1; ++i) // Stop at 'nParams-1' to get the last element as a string matrix
            {
                types::String* paramsElement = new types::String(exprs[3 + nParams + i].c_str());
                params->append(paramsElement);
            }
            // Last element of the content (string matrix)
            std::istringstream nColsStr (exprs[3 + 2 * nParams]);
            int nCols;
            nColsStr >> nCols;
            types::String* last_string = new types::String(nCols, 1);
            for (int i = 0; i < nCols; ++i)
            {
                last_string->set(i, exprs[4 + 2 * nParams + i].c_str());
            }
            params->append(last_string);

            o->append(params);

            // Next, the code
            std::istringstream nLinesStr (exprs[4 + 2 * nParams + nCols]);
            int nLines;
            nLinesStr >> nLines;
            if (nLines == 0)
            {
                types::Double* empty_matrix = types::Double::Empty();
                o->append(empty_matrix);
            }
            else
            {
                types::String* code = new types::String(nLines, 1);
                for (int i = 0; i < nLines + 1; ++i)
                {
                    code->set(i, exprs[5 + 2 * nParams + nCols + i].c_str());
                }
                o->append(code);
            }

            // Finally, the block name
            types::String* name = new types::String(exprs[5 + 2 * nParams + nCols + nLines].c_str());
            o->append(name);

            return o;
        }
        else if (exprs[0] == "List_CONSTRAINT")
        {
            // Get and return a CONSTRAINT2_c list
            types::List* o = new types::List();

            // A "CONSTRAINT2_c" block has 3 strings in 'exprs'
            for (int i = 1; i < 4; ++i)
            {
                types::String* ithElement = new types::String(exprs[i].c_str());
                o->append(ithElement);
            }
            return o;
        }
        else if (exprs[0] == "List_LOOKUP2D")
        {
            // Get and return a LOOKUP2D list
            types::List* o = new types::List();

            // A "LOOKUP2D" block has 5 strings in 'exprs'
            for (int i = 1; i < 6; ++i)
            {
                types::String* ithElement = new types::String(exprs[i].c_str());
                o->append(ithElement);
            }
            return o;
        }
        else if (exprs[0] == "String")
        {
            // Simple case, 'exprs' is a string matrix (can be empty)
            if (exprs.size() == 1)
            {
                return types::Double::Empty();
            }
            types::String* o = new types::String((int)exprs.size() - 1, 1);
            for (int i = 0; i < (int)exprs.size() - 1; ++i)
            {
                o->set(i, exprs[i + 1].data());
            }
            return o;
        }
        else
        {
            return 0;
        }
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<std::string> exprs (1); // First element will contain "String", "List_code", "List_super, "List_scifunc", "List_fortran", "List_MBLOCK" or "List_PDE"

        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();

            exprs[0] = "String";
            exprs.resize(1 + current->getSize());
            for (int i = 1; i < (int)exprs.size(); ++i)
            {
                char* c_str = wide_string_to_UTF8(current->get(i - 1));
                exprs[i] = std::string(c_str);
                FREE(c_str);
            }
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (!current->isEmpty())
            {
                return false;
            }

            exprs[0] = "String";
        }
        else if (v->getType() == types::InternalType::ScilabList)
        {
            types::List* initial_list = v->getAs<types::List>();

            // Leave 'exprs[0]' empty for the moment, because we don't know yet what we are going to deal with
            char* c_str; // Buffer

            // All these blocks have two elements in 'exprs', except for blocks "PDE", "CONSTRAINT2_c" and "LOOKUP2D"
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
                    exprs.resize(2);
                    exprs[1] = "0"; // Indicating empty matrix
                }
                else if (initial_list->get(0)->getType() == types::InternalType::ScilabString)
                {
                    types::String* initial_string = initial_list->get(0)->getAs<types::String>();

                    nParams = initial_string->getSize();
                    exprs.resize(exprs.size() + 1 + nParams); // Allocation for the first string
                    std::ostringstream strSize;
                    strSize << nParams;
                    std::string sizeStr = strSize.str();
                    exprs[1] = sizeStr; // Saving the size of the initial string

                    for (size_t i = 0; i < nParams; ++i)
                    {
                        c_str = wide_string_to_UTF8(initial_string->get(i));
                        std::string stringElement(c_str);
                        FREE(c_str);
                        exprs[2 + i] = stringElement;
                    }
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

                    exprs[0] = "List_code"; // List coming from a C block
                    exprs.push_back("0"); // Indicating empty matrix
                }
                else if (initial_list->get(1)->getType() == types::InternalType::ScilabString)
                {
                    types::String* second_string = initial_list->get(1)->getAs<types::String>();

                    exprs[0] = "List_code"; // List coming from a "user-defined function" block
                    size_t second_size = second_string->getSize();
                    int exprsSize = exprs.size(); // Saving last index before resizing
                    exprs.resize(exprs.size() + 1 + second_size); // Allocation for the first string
                    std::ostringstream strSize2;
                    strSize2 << second_size;
                    std::string sizeStr2 = strSize2.str();
                    exprs[exprsSize] = sizeStr2; // Saving the size of the second string

                    for (size_t i = 0; i < second_size; ++i)
                    {
                        c_str = wide_string_to_UTF8(second_string->get(i));
                        std::string stringElement(c_str);
                        FREE(c_str);
                        exprs[exprsSize + 1 + i] = stringElement;
                    }
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
                        exprs[0] = "List_fortran"; // List coming from a fortran_block
                    }
                    else if (second_list->getSize() == 3)
                    {
                        // Parameter names, Title message and Parameters types & sizes
                        exprs[0] = "List_super"; // List coming from a SuperBlock (masked or not)

                        // Parameters Names (string matrix, can be empty)
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

                            int exprsSize = exprs.size(); // Saving last index before resizing
                            exprs.resize(exprs.size() + nParams); // Allocation for the first string

                            for (size_t i = 0; i < nParams; ++i)
                            {
                                c_str = wide_string_to_UTF8(second_string->get(i));
                                std::string stringElement(c_str);
                                FREE(c_str);
                                exprs[exprsSize + i] = stringElement;
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

                            int exprsSize = exprs.size(); // Saving last index before resizing
                            exprs.resize(exprs.size() + nParams + 1); // Allocation for the first string, minding the title message

                            for (size_t i = 0; i < nParams + 1; ++i)
                            {
                                c_str = wide_string_to_UTF8(title_message->get(i));
                                std::string stringElement(c_str);
                                FREE(c_str);
                                exprs[exprsSize + i] = stringElement;
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

                        // First, read all types
                        for (size_t i = 0; i < nParams; ++i)
                        {
                            if (third_list->get(2 * i)->getType() != types::InternalType::ScilabString)
                            {
                                return false;
                            }
                            types::String* paramType = third_list->get(2 * i)->getAs<types::String>();
                            if (!paramType->isScalar())
                            {
                                return false;
                            }
                            c_str = wide_string_to_UTF8(paramType->get(0));
                            std::string paramTypeStored(c_str);
                            FREE(c_str);

                            exprs.push_back(paramTypeStored);
                        }
                        // Then, read all sizes
                        for (size_t i = 0; i < nParams; ++i)
                        {
                            if (third_list->get(2 * i + 1)->getType() != types::InternalType::ScilabDouble)
                            {
                                return false;
                            }
                            types::Double* paramSize = third_list->get(2 * i + 1)->getAs<types::Double>();
                            if (!paramSize->isScalar())
                            {
                                return false;
                            }

                            std::ostringstream paramSizeStored;
                            paramSizeStored << paramSize->get(0);
                            std::string paramSizeStr = paramSizeStored.str();
                            exprs.push_back(paramSizeStr);
                        }
                    }
                    else if (second_list->getSize() == 7)
                    {
                        exprs[0] = "List_scifunc"; // List coming from a scifunc block

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
                                exprs.push_back(std::string("0"));
                            }
                            else if (second_list->get(i)->getType() == types::InternalType::ScilabString)
                            {
                                types::String* ith_string = second_list->get(i)->getAs<types::String>();
                                size_t stringSize = ith_string->getSize();
                                int exprsSize = exprs.size(); // Saving last index before resizing
                                exprs.resize(exprs.size() + 1 + stringSize); // Allocation for the first string
                                std::ostringstream strSizeIth;
                                strSizeIth << stringSize;
                                std::string sizeStrIth = strSizeIth.str();
                                exprs[exprsSize] = sizeStrIth; // Saving the size of the ith string

                                for (size_t j = 0; j < stringSize; ++j)
                                {
                                    c_str = wide_string_to_UTF8(ith_string->get(j));
                                    std::string ithStringElement(c_str);
                                    FREE(c_str);
                                    exprs[exprsSize + 1 + j] = ithStringElement;
                                }
                            }
                            else
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
                if (initial_list->get(0)->getType() == types::InternalType::ScilabTList)
                {
                    exprs[0] = "List_PDE"; // List coming from a PDE block
                    types::TList* ParamsPDE = initial_list->get(0)->getAs<types::TList>();

                    types::String* header = ParamsPDE->getFieldNames();
                    int nParams = header->getSize() - 1; // The first element is the TList type
                    exprs.resize(3 + 2 * nParams); // Allocation for the header size, the TList type and the header
                    // Save the header size
                    std::ostringstream strNParams;
                    strNParams << nParams;
                    std::string nParamsStr = strNParams.str();
                    exprs[1] = nParamsStr; // Saving the header size
                    // Save the TList type
                    c_str = wide_string_to_UTF8(header->get(0));
                    std::string tlistType(c_str);
                    FREE(c_str);
                    exprs[2] = tlistType;

                    // Save the header
                    for (int i = 1; i < nParams; ++i) // Stop at 'nParams-1' to treat the last element differently (string matrix)
                    {
                        // Header element
                        c_str = wide_string_to_UTF8(header->get(i));
                        std::string headerElement(c_str);
                        FREE(c_str);
                        exprs[2 + i] = headerElement;

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
                        c_str = wide_string_to_UTF8(ith_string->get(0));
                        std::string stringElement(c_str);
                        FREE(c_str);
                        exprs[2 + nParams + i] = stringElement;
                    }
                    // Header element for the last element of ParamsPDE
                    c_str = wide_string_to_UTF8(header->get(nParams));
                    std::string lastHeaderElement(c_str);
                    FREE(c_str);
                    exprs[2 + nParams] = lastHeaderElement;
                    if (ParamsPDE->get(nParams)->getType() != types::InternalType::ScilabString)
                    {
                        return false;
                    }
                    types::String* last_string = ParamsPDE->get(nParams)->getAs<types::String>();

                    int nCols = last_string->getSize();
                    int exprsSize = exprs.size(); // Saving last index before resizing
                    exprs.resize(exprs.size() + 1 + nCols); // Store the size of the last string because it can be a matrix
                    std::ostringstream strSize;
                    strSize << nCols;
                    std::string sizeStr = strSize.str();
                    exprs[exprsSize] = sizeStr;

                    for (int i = 0; i < nCols; ++i)
                    {
                        c_str = wide_string_to_UTF8(last_string->get(i));
                        std::string lastStringElement(c_str);
                        FREE(c_str);
                        exprs[exprsSize + 1 + i] = lastStringElement;
                    }

                    // Next comes some code
                    if (initial_list->get(1)->getType() == types::InternalType::ScilabDouble)
                    {
                        types::Double* empty_matrix_expected = initial_list->get(1)->getAs<types::Double>();
                        if (!empty_matrix_expected->isEmpty())
                        {
                            return false;
                        }
                        exprs.push_back("0"); // Indicating empty matrix
                    }
                    else if (initial_list->get(1)->getType() == types::InternalType::ScilabString)
                    {
                        types::String* code = initial_list->get(1)->getAs<types::String>();

                        int nLines = code->getSize();
                        exprsSize = exprs.size(); // Saving last index before resizing
                        exprs.resize(exprs.size() + 1 + nLines); // Store the size of the last string because it can be a matrix
                        std::ostringstream strLines;
                        strLines << nLines;
                        std::string linesStr = strLines.str();
                        exprs[exprsSize] = linesStr;

                        for (int i = 0; i < nLines; ++i)
                        {
                            c_str = wide_string_to_UTF8(code->get(i));
                            std::string codeElement(c_str);
                            FREE(c_str);
                            exprs[exprsSize + 1 + i] = codeElement;
                        }
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
                    c_str = wide_string_to_UTF8(name->get(0));
                    std::string nameStr(c_str);
                    FREE(c_str);
                    exprs.push_back(nameStr);
                }
                else if (initial_list->get(0)->getType() == types::InternalType::ScilabString)
                {
                    exprs[0] = "List_CONSTRAINT"; // List coming from a CONSTRAINT2_c block

                    types::String* x = initial_list->get(0)->getAs<types::String>();
                    if (!x->isScalar())
                    {
                        return false;
                    }
                    c_str = wide_string_to_UTF8(x->get(0));
                    std::string xStr(c_str);
                    FREE(c_str);
                    exprs.push_back(xStr);

                    if (initial_list->get(1)->getType() != types::InternalType::ScilabString)
                    {
                        return false;
                    }
                    types::String* xd = initial_list->get(1)->getAs<types::String>();
                    if (!xd->isScalar())
                    {
                        return false;
                    }
                    c_str = wide_string_to_UTF8(xd->get(0));
                    std::string xdStr(c_str);
                    FREE(c_str);
                    exprs.push_back(xdStr);

                    if (initial_list->get(2)->getType() != types::InternalType::ScilabString)
                    {
                        return false;
                    }
                    types::String* id = initial_list->get(2)->getAs<types::String>();
                    if (!id->isScalar())
                    {
                        return false;
                    }
                    c_str = wide_string_to_UTF8(id->get(0));
                    std::string idStr(c_str);
                    FREE(c_str);
                    exprs.push_back(idStr);
                }
                else
                {
                    return false;
                }
            }
            else if (initial_list->getSize() == 5)
            {
                exprs[0] = "List_LOOKUP2D"; // List coming from a LOOKUP2D block

                types::String* xx = initial_list->get(0)->getAs<types::String>();
                if (!xx->isScalar())
                {
                    return false;
                }
                c_str = wide_string_to_UTF8(xx->get(0));
                std::string xxStr(c_str);
                FREE(c_str);
                exprs.push_back(xxStr);

                if (initial_list->get(1)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* yy = initial_list->get(1)->getAs<types::String>();
                if (!yy->isScalar())
                {
                    return false;
                }
                c_str = wide_string_to_UTF8(yy->get(0));
                std::string yyStr(c_str);
                FREE(c_str);
                exprs.push_back(yyStr);

                if (initial_list->get(2)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* zz = initial_list->get(2)->getAs<types::String>();
                if (!zz->isScalar())
                {
                    return false;
                }
                c_str = wide_string_to_UTF8(zz->get(0));
                std::string zzStr(c_str);
                FREE(c_str);
                exprs.push_back(zzStr);

                if (initial_list->get(3)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* Method = initial_list->get(3)->getAs<types::String>();
                if (!Method->isScalar())
                {
                    return false;
                }
                c_str = wide_string_to_UTF8(Method->get(0));
                std::string MethodStr(c_str);
                FREE(c_str);
                exprs.push_back(MethodStr);

                if (initial_list->get(4)->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* Graf = initial_list->get(4)->getAs<types::String>();
                if (!Graf->isScalar())
                {
                    return false;
                }
                c_str = wide_string_to_UTF8(Graf->get(0));
                std::string GrafStr(c_str);
                FREE(c_str);
                exprs.push_back(GrafStr);
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
            if (header->get(0) == MBLOCK)
            {
                exprs[0] = "List_MBLOCK"; // TList coming from an MBLOCK
            }
            else if (header->get(0) == MPBLOCK)
            {
                exprs[0] = "List_MPBLOCK"; // TList coming from an MPBLOCK
            }
            else
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

            char* c_str; // Buffer

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
            if (!intypeField->isScalar())
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
            if (!inField->isScalar())
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
            if (!outtypeField->isScalar())
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
            if (!paramField->isScalar())
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
                if (list->get(static_cast<int>(i))->getType() != types::InternalType::ScilabString)
                {
                    return false;
                }
                types::String* listElement = list->get(static_cast<int>(i))->getAs<types::String>();
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
            if (!ppropField->isScalar())
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
            if (!nameFField->isScalar())
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
                size_t exprsSize = exprs.size(); // Saving last index before resizing
                exprs.resize(exprs.size() + 1 + funtxtSize); // Allocation for the 'funtxt' strings
                strFuntxt << funtxtSize;
                std::string funtxtSizeStr = strFuntxt.str();
                exprs[exprsSize] = funtxtSizeStr; // Saving the size of the 'funtxt' field'

                for (size_t i = 0; i < funtxtSize; ++i)
                {
                    c_str = wide_string_to_UTF8(funtxtField->get(static_cast<int>(i)));
                    std::string funtxtElement(c_str);
                    FREE(c_str);
                    exprs[exprsSize + 1 + i] = funtxtElement;
                }
            }
        }
        else
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
