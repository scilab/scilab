/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#include "mlist.hxx"
#include "callable.hxx"
#include "configvariable.hxx"
#include "context.hxx"
#include "exp.hxx"
#include "overload.hxx"
#include "types_tools.hxx"
#include <sstream>

#ifndef NDEBUG
#include "inspector.hxx"
#endif

namespace types
{

MList::~MList()
{
    typed_list in;
    typed_list out;
    optional_list opt;
    IncreaseRef();
    in.push_back(this);

    try
    {
        Overload::generateNameAndCall(L"clear", in, 0, out, false, false);
    }
    catch (ast::InternalError& /*se*/)
    {
        // avoid error message about undefined overload %type_clear
        ConfigVariable::resetError();
        // reset where error filled by generateNameAndCall
        ConfigVariable::resetWhereError();
    }

    DecreaseRef();
}

std::wstring MList::getTypeStr() const
{
    if (getSize() > 0 && (*m_plData)[0]->isString())
    {
        return TList::getTypeStr();
    }
    // malformed mlist without field names (e.g. built through api_scilab)
    return L"mlist";
}

bool MList::getMemory(long long* _piSize, long long* _piSizePlusType)
{
    *_piSize = 0;
    *_piSizePlusType = 0;
    for (auto pData : *m_plData)
    {
        long long piS, piSPT;
        if (pData->getMemory(&piS, &piSPT))
        {
            *_piSize += piS;
            *_piSizePlusType += piSPT;
        }
    }

    *_piSizePlusType += sizeof(MList);
    return true;
}

bool MList::invoke(typed_list & in, optional_list & /*opt*/, int _iRetCount, typed_list & out, const ast::Exp & e)
{
    if (in.size() == 0)
    {
        out.push_back(this);
        return true;
    }
    else if (in.size() == 1)
    {
        InternalType * arg = in[0];
        InternalType* _out = NULL;
        if (arg->isString())
        {
            std::list<std::wstring> stFields;
            String * pString = arg->getAs<types::String>();
            for (int i = 0; i < pString->getSize(); ++i)
            {
                stFields.push_back(pString->get(i));
            }

            _out = extractStrings(stFields);

            if (_out)
            {
                List* pList = _out->getAs<types::List>();
                for (int i = 0; i < pList->getSize(); i++)
                {
                    out.push_back(pList->get(i));
                }
                delete pList;
            }
        }

        if (!out.empty())
        {
            return true;
        }
    }

    Callable::ReturnValue ret;
    // Overload of extraction need
    // the mlist from where we extract
    this->IncreaseRef();
    in.push_back(this);

    std::wstring wstrFuncName = L"%" + getShortTypeStr() + L"_e";

    ret = Overload::call(wstrFuncName, in, _iRetCount, out, false, false, e.getLocation());
    if(ret == types::Callable::OK_NoResult)
    {
        // overload not defined, try with the short name.
        // to compatibility with scilab 5 code.
        // tlist/mlist name are truncated to 8 first character
        std::wstring stType = getShortTypeStr();
        wstrFuncName = L"%" + stType.substr(0, 8) + L"_e";
        ret = Overload::call(wstrFuncName, in, _iRetCount, out, false, false, e.getLocation());
    }

    if(ret == types::Callable::OK_NoResult)
    {
        // last try that will throw an error if it not exists
        wstrFuncName = L"%l_e";
        ret = Overload::call(wstrFuncName, in, _iRetCount, out, false, true, e.getLocation());
    }

    // Remove this from "in" for keep "in" unchanged.
    this->DecreaseRef();
    in.pop_back();

    if (ret == Callable::Error)
    {
        throw ast::InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
    }

    // An extraction have to return something
    if(out.empty())
    {
        wchar_t wcstrError[512];
        os_swprintf(wcstrError, 512, _W("%ls: Extraction must have at least one output.\n").c_str(), wstrFuncName.c_str());
        throw ast::InternalError(wcstrError, 999, e.getLocation());
    }

    return true;
}
}
