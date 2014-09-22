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
    std::list<Exp *>::const_iterator itExp;

    e.getName().accept(*this);

    if (getResult() != NULL && getResult()->isInvokable())
    {
        //function call
        types::InternalType* pIT = getResult();
        types::typed_list out;
        types::typed_list in;
        types::optional_list opt;

        int iRetCount = getExpectedSize();
        int iSaveExpectedSize = iRetCount;

        // manage case [a,b]=foo() where foo is defined as a=foo()
        if (pIT->getInvokeNbOut() != -1 && pIT->getInvokeNbOut() < iRetCount)
        {
            clearResult();
            std::wostringstream os;
            os << _W("Wrong number of output arguments.\n") << std::endl;
            throw ast::ScilabError(os.str(), 999, e.getLocation());
        }

        //get function arguments
        for (itExp = e.getArgs().begin (); itExp != e.getArgs().end (); ++itExp)
        {
            if ((*itExp)->isAssignExp())
            {
                AssignExp* pAssign = static_cast<AssignExp*>(*itExp);
                //optional parameter
                Exp* pL = &pAssign->getLeftExp();
                if (!pL->isSimpleVar())
                {
                    clearResult();
                    cleanOpt(opt);
                    cleanIn(in, out);

                    std::wostringstream os;
                    os << _W("left side of optional parameter must be a variable") << std::endl;
                    throw ast::ScilabError(os.str(), 999, e.getLocation());
                }

                SimpleVar* pVar = static_cast<SimpleVar*>(pL);
                Exp* pR = &pAssign->getRightExp();
                pR->accept(*this);
                InternalType* pITR = getResult();
                // IncreaseRef to protect opt argument of scope_end delete
                // It will be deleted by clear_opt
                pITR->IncreaseRef();

                if (pIT->hasInvokeOption())
                {
                    opt.push_back(std::pair<std::wstring, InternalType*>(pVar->getSymbol().getName(), pITR));
                    //in case of macro/macrofile, we have to shift input param
                    //so add NULL item in in list to keep initial order
                    if (pIT->isMacro() || pIT->isMacroFile())
                    {
                        in.push_back(NULL);
                    }
                }
                else
                {
                    in.push_back(pITR);
                }

                clearResult();
                continue;
            }

            int iSize = getExpectedSize();
            setExpectedSize(-1);
            (*itExp)->accept(*this);
            setExpectedSize(iSize);

            if (getResult() == NULL)
            {
                //special case for empty extraction of list ( list()(:) )
                continue;
            }

            InternalType * pITArg = getResult();
            if (pITArg->isImplicitList())
            {
                types::ImplicitList* pIL = pITArg->getAs<types::ImplicitList>();
                if (pIL->isComputable())
                {
                    setResult(pIL->extractFullMatrix());
                    pITArg->killMe();
                }
            }

            if (isSingleResult())
            {
                in.push_back(getResult());
                getResult()->IncreaseRef();
                clearResult();
            }
            else
            {
                for (int i = 0 ; i < getResultSize() ; i++)
                {
                    InternalType * pITArg = getResult(i);
                    pITArg->IncreaseRef();
                    in.push_back(pITArg);
                }

                clearResult();
            }
        }

        try
        {
            setExpectedSize(iSaveExpectedSize);
            iRetCount = std::max(1, iRetCount);
            if (pIT->invoke(in, opt, iRetCount, out, *this, e))
            {
                if (iSaveExpectedSize != -1 && iSaveExpectedSize > out.size())
                {
                    std::wostringstream os;
                    os << _W("bad lhs, expected : ") << iRetCount << _W(" returned : ") << out.size() << std::endl;
                    throw ScilabError(os.str(), 999, e.getLocation());
                }

                setExpectedSize(iSaveExpectedSize);
                setResult(out);
                cleanIn(in, out);
                cleanOpt(opt);

                // In case a.b(), getResult contain pIT ("b").
                // If out == pIT, do not delete it.
                if (getResult() != pIT)
                {
                    pIT->killMe();
                }
            }
            else
            {
                std::wostringstream os;
                os << _W("Invalid index.\n");
                throw ast::ScilabError(os.str(), 999, (*e.getArgs().begin())->getLocation());
            }
        }
        catch (ScilabMessage & sm)
        {
            setExpectedSize(iSaveExpectedSize);
            clearResult();
            cleanInOut(in, out);
            cleanOpt(opt);
            pIT->killMe();

            throw sm;
        }
        catch (InternalAbort & ia)
        {
            setExpectedSize(iSaveExpectedSize);
            clearResult();
            cleanInOut(in, out);
            cleanOpt(opt);
            pIT->killMe();

            throw ia;
        }
        catch (ScilabError & se)
        {
            setExpectedSize(iSaveExpectedSize);
            clearResult();
            cleanInOut(in, out);
            cleanOpt(opt);
            pIT->killMe();

            throw se;
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
    e.getName().accept(execMeCell);

    if (execMeCell.getResult() != NULL)
    {
        //a{xxx} with a variable, extraction
        types::InternalType *pIT = NULL;

        pIT = execMeCell.getResult();

        if (pIT)
        {

            if (pIT->isCell() == false)
            {
                throw ast::ScilabError(_W("[error] Cell contents reference from a non-cell array object.\n"), 999, (*e.getArgs().begin())->getLocation());
            }
            //Create list of indexes
            types::typed_list *pArgs = GetArgumentList(e.getArgs());

            types::List* pList = pIT->getAs<types::Cell>()->extractCell(pArgs);

            if (pList == NULL)
            {
                delete pArgs;
                std::wostringstream os;
                os << _W("inconsistent row/column dimensions\n");
                //os << ((*e.args_get().begin())->getLocation()).getLocationString() << std::endl;
                throw ast::ScilabError(os.str(), 999, (*e.getArgs().begin())->getLocation());
            }

            if (pList->getSize() == 1)
            {
                setResult(pList->get(0));
            }
            else
            {
                setResult(pList);
            }

            //clean pArgs return by GetArgumentList
            for (int iArg = 0 ; iArg < (int)pArgs->size() ; iArg++)
            {
                (*pArgs)[iArg]->killMe();
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

