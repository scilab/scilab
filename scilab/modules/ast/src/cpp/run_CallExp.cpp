/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


//file included in runvisitor.cpp
template<class T>
void RunVisitorT<T>::visitprivate(const CallExp &e)
{
    std::list<Exp *>::const_iterator	itExp;

    e.name_get().accept(*this);
    if (result_get() != NULL && result_get()->isCallable())
    {
        //function call
        types::InternalType* pIT = result_get();
        types::Callable *pCall = pIT->getAs<types::Callable>();
        types::typed_list out;
        types::typed_list in;
        types::optional_list opt;

        int iRetCount = expected_getSize();

        // manage case [a,b]=foo() where foo is defined as a=foo()
        if (pCall->getNbOutputArgument() != -1 && pCall->getNbOutputArgument() < iRetCount)
        {
            pIT->killMe();
            std::wostringstream os;
            os << _W("Wrong number of output arguments.\n") << std::endl;
            throw ast::ScilabError(os.str(), 999, e.location_get());
        }

        //get function arguments
        for (itExp = e.args_get().begin (); itExp != e.args_get().end (); ++itExp)
        {
            if ((*itExp)->is_assign_exp())
            {
                AssignExp* pAssign = static_cast<AssignExp*>(*itExp);
                //optional parameter
                Exp* pL = &pAssign->left_exp_get();
                if (!pL->is_simple_var())
                {
                    pIT->killMe();
                    for (optional_list::const_iterator o = opt.begin(), end = opt.end(); o != end; ++o)
                    {
                        o->second->killMe();
                    }
                    std::wostringstream os;
                    os << _W("left side of optional parameter must be a variable") << std::endl;
                    throw ast::ScilabError(os.str(), 999, e.location_get());
                }

                SimpleVar* pVar = static_cast<SimpleVar*>(pL);
                Exp* pR = &pAssign->right_exp_get();
                pR->accept(*this);
                InternalType* pITR = result_get();

                opt.push_back(std::pair<std::wstring, InternalType*>(pVar->name_get().name_get(), pITR));
                //in case of macro/macrofile, we have to shift input param
                //so add NULL item in in list to keep initial order
                if (pIT->isMacro() || pIT->isMacroFile())
                {
                    in.push_back(NULL);
                }
                continue;
            }

            expected_setSize(1);
            (*itExp)->accept(*this);
            if (result_get() == NULL)
            {
                //special case for empty extraction of list ( list()(:) )
                continue;
            }

            types::InternalType* pITArg = result_get();
            if (pITArg->isImplicitList())
            {
                types::ImplicitList* pIL = pITArg->getAs<types::ImplicitList>();
                if (pIL->isComputable() == false)
                {
                    types::Double* pVal = new types::Double(-1, -1);
                    pVal->getReal()[0] = 1;
                    result_set(pVal);
                }
                else
                {
                    result_set(pIL->extractFullMatrix());
                }
                pITArg->killMe();
            }

            if (is_single_result())
            {
                in.push_back(result_get());
                result_get()->IncreaseRef();
            }
            else
            {
                for (int i = 0 ; i < result_getSize() ; i++)
                {
                    InternalType * pITArg = result_get(i);
                    pITArg->IncreaseRef();
                    in.push_back(pITArg);
                }
            }
        }

        try
        {
            int iSaveExpectedSize = iRetCount;
            expected_setSize(iSaveExpectedSize);
            iRetCount = std::max(1, iRetCount);

            //reset previous error before call function
            ConfigVariable::resetError();
            //update verbose";" flag
            ConfigVariable::setVerbose(e.is_verbose());
            // add line and function name in where
            ConfigVariable::where_begin(((int)e.location_get().first_line - ConfigVariable::getMacroFirstLines()) + 1, pCall->getName());
            //call function
            types::Function::ReturnValue Ret = pCall->call(in, opt, iRetCount, out, this);
            // remove the last call from where
            ConfigVariable::where_end();
            expected_setSize(iSaveExpectedSize);
            result_clear();

            if (Ret == types::Callable::OK)
            {
                if (expected_getSize() == 1 && out.size() == 0) //some function have no returns
                {
                    if (0 < iRetCount)
                    {
                        std::wostringstream os;
                        os << _W("bad lhs, expected : ") << iRetCount << _W(" returned : ") << out.size() << std::endl;
                        throw ast::ScilabError(os.str(), 999, e.location_get());
                    }
                }

                result_set(out);
                clean_in(in, out);
                clean_opt(opt);
                pCall->killMe();
            }
            else if (Ret == types::Callable::Error)
            {
                ConfigVariable::setLastErrorFunction(pCall->getName());
                ConfigVariable::setLastErrorLine(e.location_get().first_line);
                throw ast::ScilabError();
            }
        }
        catch (ScilabMessage & sm)
        {
            ConfigVariable::where_end();
            clean_in_out(in, out);
            clean_opt(opt);

            if (pCall->isMacro() || pCall->isMacroFile())
            {
                pCall->killMe();
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n").c_str(), sm.GetErrorLocation().first_line, pCall->getName().c_str());
                throw ast::ScilabMessage(szError);
            }
            else
            {
                pCall->killMe();
                throw sm;
            }
        }
        catch (ScilabError & se)
        {
            ConfigVariable::where_end();
            clean_in_out(in, out);
            clean_opt(opt);
            pCall->killMe();

            throw se;
        }
        catch (InternalAbort & ia)
        {
            ConfigVariable::where_end();
            clean_in_out(in, out);
            clean_opt(opt);
            pCall->killMe();

            throw ia;
        }
    }
    else if (result_get() != NULL)
    {
        //a(xxx) with a variable, extraction

        //get symbol of variable
        types::InternalType *pIT = result_get();
        int iArgDim = static_cast<int>(e.args_get().size());
        types::InternalType *pOut = NULL;
        std::vector<types::InternalType*> ResultList;

        //To manage extraction without parameter like SCI()
        if (iArgDim == 0)
        {
            return;
        }
        else
        {
            //Create list of indexes
            types::typed_list *pArgs = GetArgumentList(e.args_get());

            switch (pIT->getType())
            {
                case types::InternalType::ScilabDouble :
                    pOut = pIT->getAs<types::Double>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabString :
                    pOut = pIT->getAs<types::String>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabBool :
                    pOut = pIT->getAs<types::Bool>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabPolynom :
                    pOut = pIT->getAs<types::Polynom>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabInt8 :
                    pOut = pIT->getAs<types::Int8>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabUInt8 :
                    pOut = pIT->getAs<types::UInt8>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabInt16 :
                    pOut = pIT->getAs<types::Int16>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabUInt16 :
                    pOut = pIT->getAs<types::UInt16>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabInt32 :
                    pOut = pIT->getAs<types::Int32>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabUInt32 :
                    pOut = pIT->getAs<types::UInt32>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabInt64 :
                    pOut = pIT->getAs<types::Int64>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabUInt64 :
                    pOut = pIT->getAs<types::UInt64>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabList :
                {
                    ResultList = pIT->getAs<types::List>()->extract(pArgs);

                    switch (ResultList.size())
                    {
                        case 0 :
                        {
                            result_set(NULL);
                        }
                        break;
                        case 1 :
                            result_set(ResultList[0]);
                            break;
                        default :
                            for (int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
                            {
                                result_set(i, ResultList[i]);
                            }
                            break;
                    }
                }
                break;
                case InternalType::ScilabTList :
                {
                    Function::ReturnValue ret = Function::OK;
                    bool bCallOverLoad = false;
                    if (pArgs->size() == 1)
                    {
                        types::InternalType* pArg = (*pArgs)[0];
                        if ( pArg->isDouble() ||
                                pArg->isInt() ||
                                pArg->isBool() ||
                                pArg->isImplicitList() ||
                                pArg->isColon() ||
                                pArg->isDollar())

                        {
                            //call "normal" extract
                            typed_list iField;
                            iField.push_back(pArg);
                            ResultList = pIT->getAs<TList>()->extract(&iField);
                        }
                        else if (pArg->isString())
                        {
                            //extractStrings
                            list<wstring> stFields;
                            String *pString = (*pArgs)[0]->getAs<types::String>();
                            for (int i = 0 ; i < pString->getSize() ; i++)
                            {
                                stFields.push_back(pString->get(i));
                            }

                            ResultList = pIT->getAs<TList>()->extractStrings(stFields);
                            if (ResultList.empty())
                            {
                                bCallOverLoad = true;
                            }
                        }
                        else
                        {
                            bCallOverLoad = true;
                        }
                    }
                    else
                    {
                        bCallOverLoad = true;
                    }

                    if (bCallOverLoad)
                    {
                        types::typed_list in;

                        //create input argument list

                        //protect inputs
                        for (int i = 0 ; i < (int)pArgs->size() ; i++)
                        {
                            (*pArgs)[i]->IncreaseRef();
                            in.push_back((*pArgs)[i]);
                        }

                        //protect TList
                        pIT->IncreaseRef();
                        in.push_back(pIT);

                        try
                        {
                            //try to call specific exrtaction function
                            ret = Overload::call(L"%" + pIT->getAs<TList>()->getShortTypeStr() + L"_e", in, 1, ResultList, this);
                        }
                        catch (ast::ScilabError /*&e*/)
                        {
                            //if call failed try to call generic extraction function
                            ret = Overload::call(L"%l_e", in, 1, ResultList, this);
                        }

                        for (int i = 0 ; i < (int)pArgs->size() ; i++)
                        {
                            (*pArgs)[i]->DecreaseRef();
                        }
                        pIT->DecreaseRef();
                    }

                    if (ret == Function::OK)
                    {
                        switch (ResultList.size())
                        {
                            case 0 :
                            {
                                delete pArgs;
                                std::wostringstream os;
                                os << _W("Invalid index.\n");
                                throw ast::ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
                            }
                            break;
                            case 1 :
                                result_set(ResultList[0]);
                                break;
                            default :
                                for (int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
                                {
                                    result_set(i, ResultList[i]);
                                }
                                break;
                        }
                    }
                    else
                    {
                        delete pArgs;
                        throw ast::ScilabError();
                    }
                    break;
                }
                case InternalType::ScilabMList :
                {
                    Function::ReturnValue ret = Function::OK;
                    bool bCallOverLoad = false;
                    if (pArgs->size() == 1)
                    {
                        types::InternalType* pArg = (*pArgs)[0];
                        if (pArg->isString())
                        {
                            //extractStrings
                            list<wstring> stFields;
                            String *pString = (*pArgs)[0]->getAs<types::String>();
                            for (int i = 0 ; i < pString->getSize() ; i++)
                            {
                                stFields.push_back(pString->get(i));
                            }

                            ResultList = pIT->getAs<MList>()->extractStrings(stFields);
                            if (ResultList.empty())
                            {
                                bCallOverLoad = true;
                            }
                        }
                        else
                        {
                            bCallOverLoad = true;
                        }
                    }
                    else
                    {
                        bCallOverLoad = true;
                    }

                    if (bCallOverLoad)
                    {
                        types::typed_list in;

                        //create input argument list

                        //protect inputs
                        for (int i = 0 ; i < (int)pArgs->size() ; i++)
                        {
                            (*pArgs)[i]->IncreaseRef();
                            in.push_back((*pArgs)[i]);
                        }

                        //protect TList
                        pIT->IncreaseRef();
                        in.push_back(pIT);

                        try
                        {
                            //try to call specific exrtaction function
                            ret = Overload::call(L"%" + pIT->getAs<MList>()->getShortTypeStr() + L"_e", in, 1, ResultList, this);
                        }
                        catch (ast::ScilabError /*&e*/)
                        {
                            //if call failed try to call generic extraction function
                            ret = Overload::call(L"%l_e", in, 1, ResultList, this);
                        }

                        for (int i = 0 ; i < (int)pArgs->size() ; i++)
                        {
                            (*pArgs)[i]->DecreaseRef();
                        }
                        pIT->DecreaseRef();
                    }

                    if (ret == Function::OK)
                    {
                        switch (ResultList.size())
                        {
                            case 0 :
                            {
                                delete pArgs;
                                std::wostringstream os;
                                os << _W("Invalid index.\n");
                                throw ast::ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
                            }
                            break;
                            case 1 :
                                result_set(ResultList[0]);
                                break;
                            default :
                                for (int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
                                {
                                    result_set(i, ResultList[i]);
                                }
                                break;
                        }
                    }
                    else
                    {
                        delete pArgs;
                        throw ast::ScilabError();
                    }
                    break;
                }
                case InternalType::ScilabCell :
                    pOut = pIT->getAs<Cell>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabSparse :
                    pOut = pIT->getAs<types::Sparse>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabSparseBool :
                    pOut = pIT->getAs<types::SparseBool>()->extract(pArgs);
                    break;
                case types::InternalType::ScilabStruct :
                {
                    types::Struct* pStr = pIT->getAs<types::Struct>();
                    if (pArgs->size() == 1 && (*pArgs)[0]->isString())
                    {
                        //s(["x","xx"])
                        std::vector<wstring> wstFields;
                        types::String *pS = (*pArgs)[0]->getAs<types::String>();
                        for (int i = 0 ; i < pS->getSize() ; i++)
                        {
                            wstring wstField(pS->get(i));
                            if (pStr->exists(wstField))
                            {
                                wstFields.push_back(wstField);
                            }
                            else
                            {
                                delete pArgs;
                                wchar_t szError[bsiz];
                                os_swprintf(szError, bsiz, _W("Field \"%ls\" does not exists\n").c_str(), wstField.c_str());
                                throw ast::ScilabError(szError, 999, (*e.args_get().begin())->location_get());
                            }
                        }

                        ResultList = pStr->extractFields(wstFields);
                        if (ResultList.size() == 1 && ResultList[0]->isList() == false)
                        {
                            result_set(ResultList[0]);
                        }
                        else if (ResultList.size() == 1 && ResultList[0]->getAs<types::List>()->getSize() == 1)
                        {
                            result_set(ResultList[0]->getAs<types::List>()->get(0));
                        }
                        else
                        {
                            for (int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
                            {
                                result_set(i, ResultList[i]);
                            }
                        }
                        delete pArgs;
                        return;
                    }
                    else
                    {
                        pOut = pIT->getAs<types::Struct>()->extract(pArgs);
                    }
                    break;
                }
                case types::InternalType::ScilabHandle :
                {
                    if (pArgs->size() == 1 && (*pArgs)[0]->isString())
                    {
                        //s(["x"])
                        types::GraphicHandle* pH = pIT->getAs<types::GraphicHandle>();
                        types::String *pS = (*pArgs)[0]->getAs<types::String>();
                        typed_list in;
                        typed_list out;
                        optional_list opt;

                        in.push_back(pS);
                        in.push_back(pH);

                        static symbol::Variable* h_e = NULL;
                        if (h_e == NULL)
                        {
                            h_e = symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"%h_e"));
                        }

                        Function* pCall = (Function*)symbol::Context::getInstance()->get(h_e);
                        Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, this);
                        if (ret == Callable::OK)
                        {
                            pOut = out[0];
                        }
                    }
                    else
                    {
                        pOut = pIT->getAs<types::GraphicHandle>()->extract(pArgs);
                    }
                    break;
                }
                default :
                    break;
            }

            //clean pArgs return by GetArgumentList
            for (int iArg = 0 ; iArg < (int)pArgs->size() ; iArg++)
            {
                if ((*pArgs)[iArg]->isDeletable())
                {
                    delete (*pArgs)[iArg];
                }
            }
            delete pArgs;
        }

        //List extraction can return multiple items
        if (pIT->isList() == false && pIT->isTList() == false)
        {
            if (pOut == NULL)
            {
                // Special case, try to extract from an empty matrix.
                if (pIT->isDouble() && pIT->getAs<types::Double>()->getSize() == 0)
                {
                    pOut = types::Double::Empty();
                }
                else
                {
                    pIT->killMe();
                    std::wostringstream os;
                    os << _W("Invalid index.\n");
                    //os << ((*e.args_get().begin())->location_get()).location_getString() << std::endl;
                    throw ast::ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
                }
            }
            result_set(pOut);
        }
        else
        {
            if (ResultList.size() == 0)
            {
                if (pIT->isList())
                {
                    result_set(NULL);
                }
                else
                {
                    pIT->killMe();
                    std::wostringstream os;
                    os << _W("inconsistent row/column dimensions\n");
                    //os << ((*e.args_get().begin())->location_get()).location_getString() << std::endl;
                    throw ast::ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
                }
            }
        }

        if (result_get() != pIT)
        {
            pIT->killMe();
        }
    }
    else
    {
        //result == NULL ,variable doesn't exist :(
        // Sould never be in this case
        // In worst case variable pointing to function does not exists
        // visitprivate(SimpleVar) will throw the right exception.
    }
}

template<class T>
void RunVisitorT<T>::visitprivate(const CellCallExp &e)
{
    //get head
    T execMeCell;
    e.name_get().accept(execMeCell);

    if (execMeCell.result_get() != NULL)
    {
        //a{xxx} with a variable, extraction
        types::InternalType *pIT = NULL;

        pIT = execMeCell.result_get();

        if (pIT)
        {

            if (pIT->isCell() == false)
            {
                throw ast::ScilabError(_W("[error] Cell contents reference from a non-cell array object.\n"), 999, (*e.args_get().begin())->location_get());
            }
            //Create list of indexes
            types::typed_list *pArgs = GetArgumentList(e.args_get());

            types::List* pList = pIT->getAs<types::Cell>()->extractCell(pArgs);

            if (pList == NULL)
            {
                delete pArgs;
                std::wostringstream os;
                os << _W("inconsistent row/column dimensions\n");
                //os << ((*e.args_get().begin())->location_get()).location_getString() << std::endl;
                throw ast::ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
            }

            if (pList->getSize() == 1)
            {
                result_set(pList->get(0));
            }
            else
            {
                result_set(pList);
            }

            //clean pArgs return by GetArgumentList
            for (int iArg = 0 ; iArg < (int)pArgs->size() ; iArg++)
            {
                if ((*pArgs)[iArg]->isDeletable())
                {
                    delete (*pArgs)[iArg];
                }
            }
            delete pArgs;
        }
    }
    else
    {
        //result == NULL ,variable doesn't exist :(
        // Sould never be in this case
        // In worst case variable pointing to function does not exists
        // visitprivate(SimpleVar) will throw the right exception.
    }
}

