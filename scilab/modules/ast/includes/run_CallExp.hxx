/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This code is separated in run_CallExp.hxx
// but will be inlined in runvisitor.hxx
// using #include with RunVisitorT class declaration.
//
// If you need additionnal headers, please add it in runvisitor.hxx

void visitprivate(const CallExp &e)
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
            std::wostringstream os;
            os << _W("Wrong number of output arguments.\n") << std::endl;
            throw ScilabError(os.str(), 999, e.location_get());
        }

        //get function arguments
        for (itExp = e.args_get().begin (); itExp != e.args_get().end (); ++itExp)
        {
            AssignExp* pAssign = dynamic_cast<AssignExp*>(*itExp);
            if (pAssign)
            {
                //optional parameter
                Exp* pL = &pAssign->left_exp_get();
                SimpleVar* pVar = dynamic_cast<SimpleVar*>(pL);
                if (pVar == NULL)
                {
                    std::wostringstream os;
                    os << _W("left side of optional parameter must be a variable") << std::endl;
                    throw ScilabError(os.str(), 999, e.location_get());
                }

                Exp* pR = &pAssign->right_exp_get();
                pR->accept (*this);
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
            (*itExp)->accept (*this);

            if (result_get() == NULL)
            {
                //special case for empty extraction of list ( list()(:) )
                continue;
            }

            pIT = result_get();
            if (result_get()->isImplicitList())
            {
                types::ImplicitList* pIL = pIT->getAs<types::ImplicitList>();
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
                    in.push_back(result_get(i));
                    result_get(i)->IncreaseRef();
                }
            }
        }

        try
        {
            int iSaveExpectedSize = iRetCount;
            expected_setSize(iSaveExpectedSize);
            iRetCount = Max(1, iRetCount);

            //reset previous error before call function
            ConfigVariable::resetError();
            //update verbose";" flag
            ConfigVariable::setVerbose(e.is_verbose());
            // add line and function name in where
            ConfigVariable::where_begin((int)e.location_get().first_line, pCall->getName());
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
                    if (static_cast<int>(out.size()) < iRetCount)
                    {
                        //clear input parameters
                        for (unsigned int k = 0; k < in.size(); k++)
                        {
                            if (in[k] == NULL)
                            {
                                continue;
                            }

                            in[k]->DecreaseRef();
                            if (in[k]->isDeletable())
                            {
                                delete in[k];
                            }
                        }

                        std::wostringstream os;
                        os << _W("bad lhs, expected : ") << iRetCount << _W(" returned : ") << out.size() << std::endl;
                        throw ScilabError(os.str(), 999, e.location_get());
                    }
                }

                if (out.size() == 1)
                {
                    result_set(out[0]);
                }
                else
                {
                    for (int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                    {
                        result_set(i, out[i]);
                    }
                }
            }
            else if (Ret == types::Callable::Error)
            {
                ConfigVariable::setLastErrorFunction(pCall->getName());
                ConfigVariable::setLastErrorLine(e.location_get().first_line);
                throw ScilabError();
            }
        }
        catch (ScilabMessage sm)
        {
            // remove the last call from where
            ConfigVariable::where_end();

            //clear input parameters
            for (unsigned int k = 0; k < in.size(); k++)
            {
                if (in[k] && in[k]->isDeletable())
                {
                    delete in[k];
                }
            }

            if (pCall->isMacro() || pCall->isMacroFile())
            {
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), sm.GetErrorLocation().first_line, pCall->getName().c_str());
                throw ScilabMessage(szError);
            }
            else
            {
                throw sm;
            }
        }

        //clear input parameters but take care in case of in[k] == out[i]
        for (unsigned int k = 0; k < in.size(); k++)
        {
            if (in[k] == NULL)
            {
                continue;
            }

            //check if input data are use as output data
            bool bFind = false;
            for (int i = 0 ; i < (int)out.size() ; i++)
            {
                if (out[i] == in[k])
                {
                    bFind = true;
                    break;
                }
            }

            in[k]->DecreaseRef();
            if (bFind == false)
            {
                if (in[k]->isDeletable())
                {
                    delete in[k];
                }
            }
        }
    }
    else if (result_get() != NULL)
    {
        //a(xxx) with a variable, extraction

        //get symbol of variable
        types::InternalType *pIT = NULL;

        //WARNING can be a fieldexp
        const SimpleVar *Var = dynamic_cast<const SimpleVar*>(&e.name_get());
        if (Var != NULL)
        {
            pIT = symbol::Context::getInstance()->get(Var->name_get());
        }
        else
        {
            pIT = result_get();
        }

        int iArgDim = static_cast<int>(e.args_get().size());
        types::InternalType *pOut = NULL;
        std::vector<types::InternalType*> ResultList;

        //To manage extraction without parameter like SCI()
        if (iArgDim == 0)
        {
            result_set(pIT);
            return;
        }
        else
        {
            //Create list of indexes
            types::typed_list *pArgs = GetArgumentList(e.args_get());

            switch (pIT->getType())
            {
                case types::InternalType::RealDouble :
                    pOut = pIT->getAs<types::Double>()->extract(pArgs);
                    break;
                case types::InternalType::RealString :
                    pOut = pIT->getAs<types::String>()->extract(pArgs);
                    break;
                case types::InternalType::RealBool :
                    pOut = pIT->getAs<types::Bool>()->extract(pArgs);
                    break;
                case types::InternalType::RealPoly :
                    pOut = pIT->getAs<types::Polynom>()->extract(pArgs);
                    break;
                case types::InternalType::RealInt8 :
                    pOut = pIT->getAs<types::Int8>()->extract(pArgs);
                    break;
                case types::InternalType::RealUInt8 :
                    pOut = pIT->getAs<types::UInt8>()->extract(pArgs);
                    break;
                case types::InternalType::RealInt16 :
                    pOut = pIT->getAs<types::Int16>()->extract(pArgs);
                    break;
                case types::InternalType::RealUInt16 :
                    pOut = pIT->getAs<types::UInt16>()->extract(pArgs);
                    break;
                case types::InternalType::RealInt32 :
                    pOut = pIT->getAs<types::Int32>()->extract(pArgs);
                    break;
                case types::InternalType::RealUInt32 :
                    pOut = pIT->getAs<types::UInt32>()->extract(pArgs);
                    break;
                case types::InternalType::RealInt64 :
                    pOut = pIT->getAs<types::Int64>()->extract(pArgs);
                    break;
                case types::InternalType::RealUInt64 :
                    pOut = pIT->getAs<types::UInt64>()->extract(pArgs);
                    break;
                case types::InternalType::RealList :
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
                case InternalType::RealTList :
                {
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
                            Overload::call(L"%" + pIT->getAs<TList>()->getShortTypeStr() + L"_e", in, 1, ResultList, this);
                        }
                        catch (ScilabError /*&e*/)
                        {
                            //if call failed try to call generic extraction function
                            Overload::call(L"%l_e", in, 1, ResultList, this);
                        }

                        for (int i = 0 ; i < (int)pArgs->size() ; i++)
                        {
                            (*pArgs)[i]->DecreaseRef();
                        }
                        pIT->DecreaseRef();
                    }

                    switch (ResultList.size())
                    {
                        case 0 :
                        {
                            std::wostringstream os;
                            os << _W("Invalid index.\n");
                            throw ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
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
                    break;
                }
                case InternalType::RealMList :
                {
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
                            Overload::call(L"%" + pIT->getAs<MList>()->getShortTypeStr() + L"_e", in, 1, ResultList, this);
                        }
                        catch (ScilabError /*&e*/)
                        {
                            //if call failed try to call generic extraction function
                            Overload::call(L"%l_e", in, 1, ResultList, this);
                        }

                        for (int i = 0 ; i < (int)pArgs->size() ; i++)
                        {
                            (*pArgs)[i]->DecreaseRef();
                        }
                        pIT->DecreaseRef();
                    }

                    switch (ResultList.size())
                    {
                        case 0 :
                        {
                            std::wostringstream os;
                            os << _W("Invalid index.\n");
                            throw ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
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
                    break;
                }
                case InternalType::RealCell :
                    pOut = pIT->getAs<Cell>()->extract(pArgs);
                    break;
                case types::InternalType::RealSparse :
                    pOut = pIT->getAs<types::Sparse>()->extract(pArgs);
                    break;
                case types::InternalType::RealSparseBool :
                    pOut = pIT->getAs<types::SparseBool>()->extract(pArgs);
                    break;
                case types::InternalType::RealStruct :
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
                                wchar_t szError[bsiz];
                                os_swprintf(szError, bsiz, _W("Field \"%ls\" does not exists\n"), wstField.c_str());
                                throw ScilabError(szError, 999, (*e.args_get().begin())->location_get());
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
                        return;
                    }
                    else
                    {
                        pOut = pIT->getAs<types::Struct>()->extract(pArgs);
                    }
                    break;
                }
                case types::InternalType::RealHandle :
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

                        Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"%h_e"));
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
                    std::wostringstream os;
                    os << _W("Invalid index.\n");
                    //os << ((*e.args_get().begin())->location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
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
                    std::wostringstream os;
                    os << _W("inconsistent row/column dimensions\n");
                    //os << ((*e.args_get().begin())->location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
                }
            }
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

void visitprivate(const CellCallExp &e)
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
                throw ScilabError(_W("[error] Cell contents reference from a non-cell array object.\n"), 999, (*e.args_get().begin())->location_get());
            }
            //Create list of indexes
            types::typed_list *pArgs = GetArgumentList(e.args_get());

            types::List* pList = pIT->getAs<types::Cell>()->extractCell(pArgs);

            if (pList == NULL)
            {
                std::wostringstream os;
                os << _W("inconsistent row/column dimensions\n");
                //os << ((*e.args_get().begin())->location_get()).location_getString() << std::endl;
                throw ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
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
