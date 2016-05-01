/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine Elias
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

/*--------------------------------------------------------------------------*/
#include "data_structures_gw.hxx"
#include "internal.hxx"
#include "function.hxx"
#include "double.hxx"
#include "int.hxx"
#include "string.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "tlist.hxx"
#include "struct.hxx"
#include "user.hxx"

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "freeArrayOfString.h"
}

static types::Function::ReturnValue sci_getfieldStruct(types::typed_list &in, int _iRetCount, types::typed_list &out);
static types::Function::ReturnValue sci_getfieldUserType(types::typed_list &in, int _iRetCount, types::typed_list &out);

/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getfield(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "getfield", 2);
        return types::Function::Error;
    }

    //special case for struct
    if (in[1]->isStruct())
    {
        return sci_getfieldStruct(in, _iRetCount, out);
    }

    //special case for UserType
    if (in[1]->isUserType())
    {
        return sci_getfieldUserType(in, _iRetCount, out);
    }

    types::InternalType* pIndex = in[0];
    if (in[1]->isList() == false && in[1]->isMList() == false && in[1]->isTList() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: List expected.\n"), "getfield", 2);
        return types::Function::Error;
    }

    types::List* pL = in[1]->getAs<types::List>();
    types::InternalType* pITOut = NULL;

    if (pIndex->isString())
    {
        //extraction by fieldnames
        if (pL->isMList() == false && pL->isTList() == false)
        {
            Scierror(999, _("%s: Soft coded field names not yet implemented.\n"), "getfield");
            return types::Function::Error;
        }

        types::TList* pT = pL->getAs<types::TList>();
        types::String* pS = pIndex->getAs<types::String>();

        std::list<std::wstring> stFields;

        //check output arguments count
        for (int i = 0 ; i < pS->getSize() ; i++)
        {
            std::wstring wst = pS->get(i);
            if (pT->exists(wst) == false)
            {
                Scierror(999, _("%s: Invalid index.\n"), "getfield");
                return types::Function::Error;
            }

            stFields.push_back(pS->get(i));
        }

        pITOut = pT->extractStrings(stFields);
    }
    else
    {
        //extraction by index
        types::typed_list Args;
        Args.push_back(pIndex);
        pITOut = pL->extract(&Args);
    }

    if (pITOut == NULL)
    {
        Scierror(999, _("Invalid index.\n"));
        return types::Function::Error;
    }

    types::List* pList = pITOut->getAs<types::List>();
    int iListSize = pList->getSize();

    if (_iRetCount < iListSize)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "getfield", iListSize);
        return types::Function::Error;
    }

    int iIndex = 0;
    for (int i = 0; i < iListSize; i++)
    {
        if (pList->get(i)->isListUndefined())
        {
            switch (pIndex->getType())
            {
                case types::InternalType::ScilabType::ScilabDouble:
                {
                    iIndex = (int)pIndex->getAs<types::Double>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabInt8:
                {
                    iIndex = (int)pIndex->getAs<types::Int8>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabUInt8:
                {
                    iIndex = (int)pIndex->getAs<types::UInt8>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabInt16:
                {
                    iIndex = (int)pIndex->getAs<types::Int16>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabUInt16:
                {
                    iIndex = (int)pIndex->getAs<types::UInt16>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabInt32:
                {
                    iIndex = pIndex->getAs<types::Int32>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabUInt32:
                {
                    iIndex = (int)pIndex->getAs<types::UInt32>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabInt64:
                {
                    iIndex = (int)pIndex->getAs<types::Int64>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabUInt64:
                {
                    iIndex = (int)pIndex->getAs<types::UInt64>()->get(i);
                }
                break;
                case types::InternalType::ScilabType::ScilabString:
                {
                    std::wstring wField(pIndex->getAs<types::String>()->get(i));
                    iIndex = pL->getAs<types::TList>()->getIndexFromString(wField);
                    // The type (the first field) is not counted
                    iIndex++;
                }
                break;
                default:
                    break;
            }

            pList->killMe();
            Scierror(999, _("List element number %d is Undefined.\n"), iIndex);
            return types::Function::Error;
        }
    }

    for (int i = 0 ; i < iListSize ; i++)
    {
        out.push_back(pList->get(i));
    }

    pList->killMe();

    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/

static types::Function::ReturnValue sci_getfieldStruct(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pIndex = in[0];
    types::Struct* pSt = in[1]->getAs<types::Struct>();
    std::vector<types::InternalType*> vectResult;

    if (pIndex->isString())
    {
        types::String* pFields = pIndex->getAs<types::String>();
        std::vector<std::wstring> wstFields;

        for (int i = 0 ; i < pFields->getSize() ; i++)
        {
            wstFields.push_back(pFields->get(i));
        }

        vectResult = pSt->extractFields(wstFields);
    }
    else
    {
        //extraction by index
        // do not extract myself of myself
        types::typed_list input;
        input.push_back(in[0]);
        vectResult = pSt->extractFields(&input);
    }

    if (vectResult.size() == 0)
    {
        Scierror(78, _("%s: Invalid index.\n"), "getfield");
        return types::Function::Error;
    }

    if (_iRetCount != static_cast<int>(vectResult.size()))
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "getfield", vectResult.size());
        return types::Function::Error;
    }

    for (int i = 0 ; i < _iRetCount ; i++)
    {
        out.push_back(vectResult[i]);
    }

    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/

static types::Function::ReturnValue sci_getfieldUserType(types::typed_list &in, int /*_iRetCount*/, types::typed_list &out)
{
    types::UserType* pUT = in[1]->getAs<types::UserType>();

    if (in[0]->isDouble())
    {
        types::Double* pIndex = in[0]->getAs<types::Double>();

        // Extract the properties
        types::typed_list one (1, new types::Double(1));
        types::InternalType* properties = pUT->extract(&one);
        if (!properties->isString())
        {
            Scierror(999, _("%s: Could not read the argument #%d properties.\n"), "getfield", 2);
            delete one[0];
            return types::Function::Error;
        }
        delete one[0];

        types::String* propertiesStr = properties->getAs<types::String>();

        // Checking the index validity
        int index = pIndex->get(0);
        if (floor(index) != index)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "getfield", 1);
            return types::Function::Error;
        }
        if (index < 1 || index > 1 + propertiesStr->getSize())
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: At most %d expected.\n"), "getfield", 1, 1 + propertiesStr->getSize());
            return types::Function::Error;
        }

        if (index == 1)
        {
            // Return the properties
            types::String* ret = new types::String(1, 1 + propertiesStr->getSize());
            ret->set(0, pUT->getTypeStr().c_str());
            for (int i = 0; i < propertiesStr->getSize(); ++i)
            {
                ret->set(i + 1, propertiesStr->get(i));
            }
            out.push_back(ret);
        }
        else
        {
            // Return property number 'index-2'
            types::InternalType* field;
            pUT->extract(propertiesStr->get(index - 2), field);
            out.push_back(field);
        }

        properties->DecreaseRef();
        properties->killMe();

        return types::Function::OK;
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Integer expected.\n"), "getfield", 1);
        return types::Function::Error;
    }
}
/*-----------------------------------------------------------------------------------*/
