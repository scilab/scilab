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
    if(result_get() != NULL && result_get()->isCallable())
    {//function call
        InternalType* pIT = result_get();
        Callable *pCall = pIT->getAs<Callable>();
        types::typed_list out;
        types::typed_list in;

        int iRetCount = expected_getSize();

        //get function arguments
        for (itExp = e.args_get().begin (); itExp != e.args_get().end (); ++itExp)
        {
            expected_size_set(1);
            (*itExp)->accept (*this);

            if(result_get() == NULL)
            {
                //special case for empty extraction of list ( list()(:) )
                continue;
            }

            pIT = result_get();
            if(result_get()->isImplicitList())
            {
                ImplicitList* pIL = pIT->getAs<ImplicitList>();
                if(pIL->isComputable() == false)
                {
                    Double* pVal = new Double(-1, -1);
                    pVal->getReal()[0] = 1;
                    result_set(pVal);
                }
                else
                {
                    result_set(pIL->extractFullMatrix());
                }
            }

            if(is_single_result())
            {
                in.push_back(result_get());
                result_get()->IncreaseRef();
            }
            else
            {
                for(int i = 0 ; i < result_getSize() ; i++)
                {
                    in.push_back(result_get(i));
                    result_get(i)->IncreaseRef();
                }
            }
        }

        ////reset result
        //result_clear();

        try
        {
            expected_size_set(iRetCount);
            iRetCount = Max(1, iRetCount);
            Function::ReturnValue Ret = pCall->call(in, iRetCount, out, this);
            //reset result
            result_clear();
            if(Ret == Callable::OK)
            {
                if(expected_getSize() == 1 && out.size() == 0) //some function have no returns
                {
                    if(static_cast<int>(out.size()) < iRetCount)
                    {
                        //clear input parameters
                        for(unsigned int k = 0; k < in.size(); k++)
                        {
                            in[k]->DecreaseRef();
                            if(in[k]->isDeletable())
                            {
                                delete in[k];
                            }
                        }

                        std::wostringstream os;
                        os << L"bad lhs, expected : " << iRetCount << L" returned : " << out.size() << std::endl;
                        throw ScilabError(os.str(), 999, e.location_get());
                    }
                }

                if(out.size() == 1)
                {
                    result_set(out[0]);
                }
                else
                {
                    for(int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                    {
                        result_set(i, out[i]);
                    }
                }
            }
            else if(Ret == Callable::Error)
            {
                ConfigVariable::setLastErrorFunction(pCall->getName());
                ConfigVariable::setLastErrorLine(e.location_get().first_line);
                throw ScilabError();
            }
        }
        catch(ScilabMessage sm)
        {
            //clear input parameters
            for(unsigned int k = 0; k < in.size(); k++)
            {
                if(in[k]->isDeletable())
                {
                    delete in[k];
                }
            }

            if(pCall->isMacro() || pCall->isMacroFile())
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
        for(unsigned int k = 0; k < in.size(); k++)
        {
            //check if input data are use as output data
            bool bFind = false;
            for(int i = 0 ; i < out.size() ; i++)
            {
                if(out[i] == in[k])
                {
                    bFind = true;
                    break;
                }
            }

            in[k]->DecreaseRef();
            if(bFind == false)
            {
                if(in[k]->isDeletable())
                {
                    delete in[k];
                }
            }
        }
    }
    else if(result_get() != NULL)
    {//a(xxx) with a variable, extraction

        //get symbol of variable
        InternalType *pIT = NULL;

        //WARNING can be a fieldexp
        const SimpleVar *Var = dynamic_cast<const SimpleVar*>(&e.name_get());
        if(Var != NULL)
        {
            pIT = symbol::Context::getInstance()->get(Var->name_get());
        }
        else
        {
            pIT = result_get();
        }

        int iArgDim = static_cast<int>(e.args_get().size());
        InternalType *pOut = NULL;
        std::vector<InternalType*> ResultList;

        //To manage extraction without parameter like SCI()
        if(iArgDim == 0)
        {
            result_set(pIT);
            return;
        }
        else if(pIT->isTList())
        {
            list<wstring> stFields;
            list<Exp*>::const_iterator it1;

            InternalType::RealType rtIndex = InternalType::RealInternal;
            bool bTypeSet = false;
            for(it1 = e.args_get().begin() ; it1 != e.args_get().end() ; it1++)
            {
                (*it1)->accept(*this);

                if(bTypeSet && result_get()->getType() != rtIndex)
                {//TODO: error
                    scilabWriteW(L"merdouille");
                }

                if(result_get()->isString())
                {
                    rtIndex = InternalType::RealString;
                    bTypeSet = true;
                    InternalType* pVar  = result_get();
                    String *pString = pVar->getAs<types::String>();
                    for(int i = 0 ; i < pString->getSize() ; i++)
                    {
                        stFields.push_back(pString->get(i));
                    }
                }
                else if(result_get()->isDouble())
                {//manage error
                    rtIndex = InternalType::RealDouble;
                    bTypeSet = true;
                    break;
                }
            }
            result_set(NULL);

            if(rtIndex  == InternalType::RealDouble)
            {
                //Create list of indexes
                //ArrayOf<double>* pArray = pIT->getAs<ArrayOf<double> >();
                //bool bSeeAsVector   = iArgDim < pArray->getDims();
                //int *piIndexSeq		= NULL;
                //int *piMaxDim       = NULL;
                //int *piDimSize		= new int[iArgDim];
                //int iDims           = 0;
                //int iTotalCombi		= GetIndexList(pIT, e.args_get(), &piIndexSeq, &piMaxDim, &iDims, pIT, piDimSize);

                ////typed_list *pArgs = GetArgumentList(e.args_get());

                ////check we don't have bad indexes like "< 1"
                //for(int i = 0 ; i < iTotalCombi * iArgDim; i++)
                //{
                //    if(piIndexSeq[i] < 1)
                //    {
                //        //manage error
                //        std::wostringstream os;
                //        os << _W("Indexes must be positive .\n");
                //        os << ((Location)e.name_get().location_get()).location_getString() << std::endl;
                //        throw ScilabError(os.str(), 999, e.name_get().location_get());
                //    }
                //}
                //ResultList = pIT->getAsTList()->extract(iTotalCombi, piIndexSeq, piMaxDim, iDims, piDimSize, bSeeAsVector);
            }
            else if(rtIndex  == InternalType::RealString)
            {
                ResultList = pIT->getAsTList()->extractStrings(stFields);
            }

            if(ResultList.size() == 1)
            {
                result_set(ResultList[0]);
            }
            else
            {
                for(int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
                {
                    result_set(i, ResultList[i]);
                }
            }
        }
        else
        {
            //Create list of indexes
            typed_list *pArgs = GetArgumentList(e.args_get());

            switch(pIT->getType())
            {
            case InternalType::RealDouble :
                pOut = pIT->getAs<Double>()->extract(pArgs);
                break;
            case InternalType::RealString :
                pOut = pIT->getAs<String>()->extract(pArgs);
                break;
            case InternalType::RealBool :
                pOut = pIT->getAs<Bool>()->extract(pArgs);
                break;
            case InternalType::RealPoly :
                pOut = pIT->getAs<Polynom>()->extract(pArgs);
                break;
            case InternalType::RealInt8 :
                pOut = pIT->getAs<Int8>()->extract(pArgs);
                break;
            case InternalType::RealUInt8 :
                pOut = pIT->getAs<UInt8>()->extract(pArgs);
                break;
            case InternalType::RealInt16 :
                pOut = pIT->getAs<Int16>()->extract(pArgs);
                break;
            case InternalType::RealUInt16 :
                pOut = pIT->getAs<UInt16>()->extract(pArgs);
                break;
            case InternalType::RealInt32 :
                pOut = pIT->getAs<Int32>()->extract(pArgs);
                break;
            case InternalType::RealUInt32 :
                pOut = pIT->getAs<UInt32>()->extract(pArgs);
                break;
            case InternalType::RealInt64 :
                pOut = pIT->getAs<Int64>()->extract(pArgs);
                break;
            case InternalType::RealUInt64 :
                pOut = pIT->getAs<UInt64>()->extract(pArgs);
                break;
            case InternalType::RealList :
                {
                    ResultList = pIT->getAsList()->extract(pArgs);

                    switch(ResultList.size())
                    {
                    case 0 :
                        {
                            //std::wostringstream os;
                            //os << L"inconsistent dimensions\n";
                            //throw ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
                            result_set(NULL);
                        }
                        break;
                    case 1 :
                        result_set(ResultList[0]);
                        break;
                    default :
                        for(int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
                        {
                            result_set(i, ResultList[i]);
                        }
                        break;
                    }
                }
                break;
            case InternalType::RealCell :
                pOut = pIT->getAs<Cell>()->extract(pArgs);
                break;
            case InternalType::RealStruct :
                {
                    Struct* pStr = pIT->getAs<Struct>();
                    if(pArgs->size() == 1 && (*pArgs)[0]->isString())
                    {//s(["x","xx"])
                        list<wstring> wstFields;
                        String *pS = (*pArgs)[0]->getAs<types::String>();
                        for(int i = 0 ; i < pS->getSize() ; i++)
                        {
                            wstring wstField(pS->get(i));
                            if(pStr->exists(wstField))
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
                        if(ResultList.size() == 1 && ResultList[0]->getAs<List>()->getSize() == 1)
                        {
                            result_set(ResultList[0]->getAs<List>()->get(0));
                        }
                        else
                        {
                            for(int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
                            {
                                result_set(i, ResultList[i]);
                            }
                        }
                        return;
                    }
                    else
                    {
                        pOut = pIT->getAs<Struct>()->extract(pArgs);
                    }
                }
            default :
                break;
            }

            //clean pArgs return by GetArgumentList
            for(int iArg = 0 ; iArg < pArgs->size() ; iArg++)
            {
                if((*pArgs)[iArg]->isDeletable())
                {
                    delete (*pArgs)[iArg];
                }
            }
            delete pArgs;
        }

        //List extraction can return multiple items
        if(pIT->isList() == false && pIT->isTList() == false)
        {
            if(pOut == NULL)
            {
                // Special case, try to extract from an empty matrix.
                if (pIT->isDouble() && pIT->getAs<Double>()->getSize() == 0)
                {
                    pOut = Double::Empty();
                }
                else
                {
                    std::wostringstream os;
                    os << L"Invalid index\n";
                    //os << ((*e.args_get().begin())->location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
                }
            }
            result_set(pOut);
        }
        else
        {
            if(ResultList.size() == 0)
            {
                if(pIT->isList())
                {
                    result_set(NULL);
                }
                else
                {
                    std::wostringstream os;
                    os << L"inconsistent row/column dimensions\n";
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

    if(execMeCell.result_get() != NULL)
    {//a{xxx} with a variable, extraction
        InternalType *pIT = NULL;

        pIT = execMeCell.result_get();

        if(pIT)
        {

            if (pIT->isCell() == false)
            {
                throw ScilabError(_W("[error] Cell contents reference from a non-cell array object.\n"), 999, (*e.args_get().begin())->location_get());
            }
            //Create list of indexes
            typed_list *pArgs = GetArgumentList(e.args_get());

            List* pList = pIT->getAs<Cell>()->extractCell(pArgs);

            if(pList == NULL)
            {
                std::wostringstream os;
                os << L"inconsistent row/column dimensions\n";
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
            for(int iArg = 0 ; iArg < pArgs->size() ; iArg++)
            {
                if((*pArgs)[iArg]->isDeletable())
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
