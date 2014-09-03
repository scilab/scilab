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

    e.name_get().accept(*this);

    if (result_get() != NULL && result_get()->isInvokable())
    {
        //function call
        types::InternalType* pIT = result_get();
        types::typed_list out;
        types::typed_list in;
        types::optional_list opt;

        int iRetCount = expected_getSize();
        int iSaveExpectedSize = iRetCount;

        // manage case [a,b]=foo() where foo is defined as a=foo()
        if (pIT->getInvokeNbOut() != -1 && pIT->getInvokeNbOut() < iRetCount)
        {
            result_clear();
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
                    result_clear();
                    clean_opt(opt);
                    clean_in(in, out);

                    std::wostringstream os;
                    os << _W("left side of optional parameter must be a variable") << std::endl;
                    throw ast::ScilabError(os.str(), 999, e.location_get());
                }

                SimpleVar* pVar = static_cast<SimpleVar*>(pL);
                Exp* pR = &pAssign->right_exp_get();
                pR->accept(*this);
                InternalType* pITR = result_get();
                // IncreaseRef to protect opt argument of scope_end delete
                // It will be deleted by clear_opt
                pITR->IncreaseRef();

                if (pIT->hasInvokeOption())
                {
                    opt.push_back(std::pair<std::wstring, InternalType*>(pVar->name_get().name_get(), pITR));
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

                result_clear();
                continue;
            }

            int iSize = expected_getSize();
            expected_setSize(-1);
            (*itExp)->accept(*this);
            expected_setSize(iSize);

            if (result_get() == NULL)
            {
                //special case for empty extraction of list ( list()(:) )
                continue;
            }

            InternalType * pITArg = result_get();
            if (pITArg->isImplicitList())
            {
                types::ImplicitList* pIL = pITArg->getAs<types::ImplicitList>();
                if (pIL->isComputable())
                {
                    result_set(pIL->extractFullMatrix());
                    pITArg->killMe();
                }
            }

            if (is_single_result())
            {
                in.push_back(result_get());
                result_get()->IncreaseRef();
                result_clear();
            }
            else
            {
                for (int i = 0 ; i < result_getSize() ; i++)
                {
                    InternalType * pITArg = result_get(i);
                    pITArg->IncreaseRef();
                    in.push_back(pITArg);
                }

                result_clear();
            }
        }

        try
        {
            expected_setSize(iSaveExpectedSize);
            iRetCount = std::max(1, iRetCount);
            if (pIT->invoke(in, opt, iRetCount, out, *this, e))
            {
                if (iSaveExpectedSize != -1 && iSaveExpectedSize > out.size())
                {
                    std::wostringstream os;
                    os << _W("bad lhs, expected : ") << iRetCount << _W(" returned : ") << out.size() << std::endl;
                    throw ScilabError(os.str(), 999, e.location_get());
                }

                expected_setSize(iSaveExpectedSize);
                result_set(out);
                clean_in(in, out);
                clean_opt(opt);

                // In case a.b(), result_get contain pIT ("b").
                // If out == pIT, do not delete it.
                if (result_get() != pIT)
                {
                    pIT->killMe();
                }
            }
            else
            {
                std::wostringstream os;
                os << _W("Invalid index.\n");
                throw ast::ScilabError(os.str(), 999, (*e.args_get().begin())->location_get());
            }
        }
        catch (ScilabMessage & sm)
        {
            expected_setSize(iSaveExpectedSize);
            result_clear();
            clean_in_out(in, out);
            clean_opt(opt);
            pIT->killMe();

            throw sm;
        }
        catch (InternalAbort & ia)
        {
            expected_setSize(iSaveExpectedSize);
            result_clear();
            clean_in_out(in, out);
            clean_opt(opt);
            pIT->killMe();

            throw ia;
        }
        catch (ScilabError & se)
        {
            expected_setSize(iSaveExpectedSize);
            result_clear();
            clean_in_out(in, out);
            clean_opt(opt);
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

