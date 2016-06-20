/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

//file included in runvisitor.cpp
namespace ast {

template<class T>
void RunVisitorT<T>::visitprivate(const CallExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    types::typed_list outTmp;
    types::typed_list inTmp;
    std::vector<std::wstring> vectOptName;
    std::vector<int> vectNbResult;

    int iRetCount = getExpectedSize();
    int iSaveExpectedSize = iRetCount;

    //get function arguments
    exps_t args = e.getArgs();
    try
    {
        for (auto& arg : args)
        {
            int iSize = getExpectedSize();
            if (arg->isAssignExp())
            {
                AssignExp* pAssign = static_cast<AssignExp*>(arg);
                //optional parameter
                Exp* pL = &pAssign->getLeftExp();
                if (!pL->isSimpleVar())
                {
                    std::wostringstream os;
                    os << _W("left side of optional parameter must be a variable") << std::endl;
                    CoverageInstance::stopChrono((void*)&e);
                    throw ast::InternalError(os.str(), 999, e.getLocation());
                }

                SimpleVar* pVar = pL->getAs<SimpleVar>();
                Exp* pR = &pAssign->getRightExp();
                // optional parameter have only one output argument
                setExpectedSize(1);
                try
                {
                    pR->accept(*this);
                }
                catch (ScilabException &)
                {
                    CoverageInstance::stopChrono((void*)&e);
                    throw;
                }
                setExpectedSize(iSize);
                types::InternalType* pITR = getResult();
                // IncreaseRef to protect opt argument of scope_end delete
                // It will be deleted by clear_opt
                pITR->IncreaseRef();

                vectOptName.push_back(pVar->getSymbol().getName());
                inTmp.push_back(pITR);
                vectNbResult.push_back(1);

                clearResult();
                continue;
            }

            setExpectedSize(-1);
            try
            {
                arg->accept(*this);
            }
            catch (ScilabException &)
            {
                CoverageInstance::stopChrono((void*)&e);
                throw;
            }
            setExpectedSize(iSize);

            if (getResult() == NULL)
            {
                //special case for empty extraction of list ( list()(:) )
                vectNbResult.push_back(0);
                continue;
            }

            if (isSingleResult())
            {
                inTmp.push_back(getResult());
                getResult()->IncreaseRef();
            }
            else
            {
                for (int i = 0; i < getResultSize(); i++)
                {
                    types::InternalType * pITArg = getResult(i);
                    pITArg->IncreaseRef();
                    inTmp.push_back(pITArg);
                }
            }

            vectNbResult.push_back(getResultSize());
            clearResult();
        }
    }
    catch (const InternalError& ie)
    {
        clearResult();
        cleanIn(inTmp, outTmp);
        CoverageInstance::stopChrono((void*)&e);
        throw ie;
    }

    // get function/variable
    try
    {
        e.getName().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }
    types::InternalType* pIT = getResult();

    // pIT can be NULL if one of call return nothing. foo()(1) with foo return nothing.
    if(pIT == NULL)
    {
        clearResult();
        std::wostringstream os;
        os << _W("Cannot extract from nothing.") << std::endl;
        CoverageInstance::stopChrono((void*)&e);
        throw ast::InternalError(os.str(), 999, e.getLocation());
    }

    types::typed_list out;
    types::typed_list in;
    types::optional_list opt;

    // manage case [a,b]=foo() where foo is defined as a=foo()
    if (pIT->getInvokeNbOut() != -1 && pIT->getInvokeNbOut() < iRetCount)
    {
        clearResult();
        std::wostringstream os;
        os << _W("Wrong number of output arguments.\n") << std::endl;
        CoverageInstance::stopChrono((void*)&e);
        throw ast::InternalError(os.str(), 999, e.getLocation());
    }

    if (pIT->isCallable())
    {
        CoverageInstance::invoke(static_cast<types::Callable *>(pIT));
    }

    // manage input according the function/variable
    int iLoop = -1;
    int iterIn = 0;
    int iterOptName = 0;
    for (auto& arg : args)
    {
        iLoop++;

        //special case for empty extraction of list ( list()(:) )
        if (vectNbResult[iLoop] == 0)
        {
            continue;
        }

        //extract implicit list for call()
        if (pIT->isCallable() || pIT->isUserType())
        {
            if (inTmp[iterIn]->isImplicitList())
            {
                types::ImplicitList* pIL = inTmp[iterIn]->getAs<types::ImplicitList>();
                if (pIL->isComputable())
                {
                    types::InternalType* pITExtract = pIL->extractFullMatrix();
                    pITExtract->IncreaseRef();
                    inTmp[iterIn] = pITExtract;
                    pIL->DecreaseRef();
                    pIL->killMe();
                }
            }
        }

        // management of optional input
        if (arg->isAssignExp())
        {
            if (pIT->hasInvokeOption())
            {
                opt[vectOptName[iterOptName++]] = inTmp[iterIn++];

                //in case of macro/macrofile, we have to shift input param
                //so add NULL item in in list to keep initial order
                if (pIT->isMacro() || pIT->isMacroFile())
                {
                    in.push_back(NULL);
                }
            }
            else
            {
                in.push_back(inTmp[iterIn++]);
            }

            continue;
        }

        // default case
        for (int i = 0; i < vectNbResult[iLoop]; i++, iterIn++)
        {
            in.push_back(inTmp[iterIn]);
        }
    }

    try
    {
        // Extraction with a List in input argument.
        // This extraction must be a recursive extract.
        int iLoopSize = 1;
        types::List* pListArg = NULL;
        if (pIT->isCallable() == false && in.size() == 1 && in[0]->isList())
        {
            pListArg = in[0]->getAs<types::List>();
            iLoopSize = pListArg->getSize();
            cleanOpt(opt, out);
        }

        setExpectedSize(iSaveExpectedSize);
        iRetCount = std::max(1, iRetCount);

        for (int i = 0; i < iLoopSize; i++)
        {
            if (pListArg)
            {
                in[0] = pListArg->get(i);

                if (in[0]->isList())
                {
                    if (pIT->isCallable())
                    {
                        // list used like "varargin"
                        types::List* pLFuncArgs = in[0]->getAs<types::List>();
                        types::typed_list input;
                        for (int j = 0; j < pLFuncArgs->getSize(); j++)
                        {
                            input.push_back(pLFuncArgs->get(j));
                            input.back()->IncreaseRef();
                        }

                        in = input;
                    }
                    else
                    {
                        pListArg->DecreaseRef();
                        pListArg->killMe();

                        std::wostringstream os;
                        os << _W("Invalid index.\n");
                        throw ast::InternalError(os.str(), 999, e.getFirstLocation());
                    }
                }
                else
                {
                    in[0]->IncreaseRef();
                }
            }

            bool ret = false;
            if (pIT->isInvokable() == false)
            {
                // call overload
                ret = Overload::call(L"%" + pIT->getShortTypeStr() + L"_e", in, iRetCount, out, true);
            }
            else
            {
                ret = pIT->invoke(in, opt, iRetCount, out, e);
                if (ret == false && pIT->isUserType())
                {
                    // call overload
                    ret = Overload::call(L"%" + pIT->getShortTypeStr() + L"_e", in, iRetCount, out, true);
                }
            }

            if (ret)
            {
                if (iSaveExpectedSize != -1 && iSaveExpectedSize > out.size())
                {
                    char szError[bsiz];
                    if (pIT->isCallable())
                    {
                        char* strFName = wide_string_to_UTF8(pIT->getAs<types::Callable>()->getName().c_str());
                        os_sprintf(szError, _("%s: Wrong number of output argument(s): %d expected.\n"), strFName, out.size());
                        FREE(strFName);
                    }
                    else
                    {
                        os_sprintf(szError, _("%s: Wrong number of output argument(s): %d expected.\n"), "extract", out.size());
                    }

                    wchar_t* wError = to_wide_string(szError);
                    std::wstring err(wError);
                    FREE(wError);
                    throw InternalError(err, 999, e.getLocation());
                }

                setExpectedSize(iSaveExpectedSize);
                setResult(out);
                cleanIn(in, out);
                cleanOpt(opt, out);

                // In case a.b(), getResult contain pIT ("b").
                // If out == pIT, do not delete it.
                if (getResult() != pIT)
                {
                    // protect element of out in case where
                    // out contain elements of pIT
                    for (int i = 0; i < out.size(); i++)
                    {
                        out[i]->IncreaseRef();
                    }

                    pIT->killMe();

                    // unprotect
                    for (int i = 0; i < out.size(); i++)
                    {
                        out[i]->DecreaseRef();
                    }
                }

                if (pListArg && i + 1 != iLoopSize)
                {
                    pIT = out[0];
                    out.clear();
                    setResult(NULL);
                }
            }
            else
            {
                std::wostringstream os;
                os << _W("Invalid index.\n");
                throw ast::InternalError(os.str(), 999, e.getFirstLocation());
            }
        }

        if (pListArg)
        {
            pListArg->DecreaseRef();
            pListArg->killMe();
        }
    }
    catch (InternalAbort & ia)
    {
        setExpectedSize(iSaveExpectedSize);
        if (pIT != getResult())
        {
            pIT->killMe();
        }

        clearResult();
        cleanInOut(in, out);
        cleanOpt(opt, out);
        CoverageInstance::stopChrono((void*)&e);

        throw ia;
    }
    catch (const InternalError& ie)
    {
        setExpectedSize(iSaveExpectedSize);
        if (pIT != getResult())
        {
            pIT->killMe();
        }

        clearResult();
        cleanInOut(in, out);
        cleanOpt(opt, out);
        CoverageInstance::stopChrono((void*)&e);

        throw ie;
    }

    CoverageInstance::stopChrono((void*)&e);
}

template<class T>
void RunVisitorT<T>::visitprivate(const CellCallExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);

    //get head
    T execMeCell;
    try
    {
        e.getName().accept(execMeCell);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    if (execMeCell.getResult() != NULL)
    {
        //a{xxx} with a variable, extraction
        types::InternalType *pIT = NULL;

        pIT = execMeCell.getResult();

        if (pIT)
        {

            if (pIT->isCell() == false)
            {
                CoverageInstance::stopChrono((void*)&e);
                throw ast::InternalError(_W("[error] Cell contents reference from a non-cell array object.\n"), 999, e.getFirstLocation());
            }
            //Create list of indexes
            ast::exps_t exps = e.getArgs();
            types::typed_list *pArgs = GetArgumentList(exps);

            if (pArgs->size() == 0)
            {
                // Case a{}
                delete pArgs;
                std::wostringstream os;
                os << _W("Cell : Cannot extract without arguments.\n");
                CoverageInstance::stopChrono((void*)&e);
                throw ast::InternalError(os.str(), 999, e.getFirstLocation());
            }

            types::List* pList = pIT->getAs<types::Cell>()->extractCell(pArgs);

            if (pList == NULL)
            {
                delete pArgs;
                std::wostringstream os;
                os << _W("inconsistent row/column dimensions\n");
                //os << ((*e.args_get().begin())->getLocation()).getLocationString() << std::endl;
                CoverageInstance::stopChrono((void*)&e);
                throw ast::InternalError(os.str(), 999, e.getFirstLocation());
            }

            if (pList->getSize() == 1)
            {
                types::InternalType* ret = pList->get(0);
                setResult(ret);

                ret->IncreaseRef();
                pList->killMe();
                ret->DecreaseRef();
            }
            else
            {
                setResult(pList);
            }


            //clean pArgs return by GetArgumentList
            for (int iArg = 0; iArg < (int)pArgs->size(); iArg++)
            {
                (*pArgs)[iArg]->killMe();
            }
            delete pArgs;
        }
    }
    else
    {
        //result == NULL ,variable doesn't exist :(
        // Should never be in this case
        // In worst case variable pointing to function does not exists
        // visitprivate(SimpleVar) will throw the right exception.
    }
    CoverageInstance::stopChrono((void*)&e);
}

} /* namespace ast */
