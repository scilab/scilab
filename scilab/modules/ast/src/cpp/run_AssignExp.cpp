/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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
void RunVisitorT<T>::visitprivate(const AssignExp  &e)
{
    /*Create local exec visitor*/
    try
    {
        SimpleVar * pVar = NULL;
        if (e.left_exp_get().is_simple_var())
        {
            pVar = static_cast<SimpleVar*>(&e.left_exp_get());
        }

        /*get king of left hand*/
        if (pVar)
        {
            // x = ?
            /*getting what to assign*/
            InternalType *pIT = e.right_val_get();
            if (pIT == NULL)
            {
                expected_setSize(1);
                e.right_exp_get().accept(*this);

                if (result_getSize() != 1)
                {
                    std::wostringstream os;
                    os << _W("Can not assign multiple value in a single variable") << std::endl;
                    //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                    throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
                }

                pIT = result_get();
                //reset result
                result_set(NULL);
            }

            if (pIT->isImplicitList())
            {
                if (pIT->getAs<ImplicitList>()->isComputable())
                {
                    InternalType *pTemp = pIT->getAs<ImplicitList>()->extractFullMatrix();
                    delete pIT;
                    pIT = pTemp;
                }
            }

            if (pIT->isAssignable() == false)
            {
                if (pIT->isListDelete())
                {
                    //used to delete a variable in current scope
                    symbol::Context::getInstance()->remove(pVar->name_get());
                }

                result_set(NULL);
                return;
            }

            if (e.right_exp_get().is_return_exp())
            {
                //ReturnExp so, put the value in the previous scope
                symbol::Context::getInstance()->putInPreviousScope(pVar->stack_get(), pIT);
                ((AssignExp*)&e)->return_set();
            }
            else
            {
                symbol::Context::getInstance()->put(pVar->stack_get(), pIT);
            }

            if (e.is_verbose() && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << pVar->name_get().name_get() << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pIT, pVar->name_get().name_get().c_str());
            }
            return;
        }

        CellCallExp *pCell = dynamic_cast<CellCallExp*>(&e.left_exp_get());
        if (pCell)
        {
            InternalType *pOut	= NULL;

            /*getting what to assign*/
            InternalType* pITR = e.right_val_get();
            if (pITR == NULL)
            {
                e.right_exp_get().accept(*this);
                pITR = result_get();
                //reset result
                result_set(NULL);
            }

            if (pITR == NULL)
            {
                // if the right hand is NULL.
                std::wostringstream os;
                os << _W("Unable to extract right part expression.\n");
                throw ast::ScilabError(os.str(), 999, e.left_exp_get().location_get());
            }

            std::list<ExpHistory*> fields;
            if (getFieldsFromExp(pCell, fields) == false)
            {
                std::wostringstream os;
                os << _W("Get fields from expression failed.");
                throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            pOut = evaluateFields(pCell, fields, pITR);

            if (pOut == NULL)
            {
                std::wostringstream os;
                os << _W("Fields evaluation failed.");
                throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            if (pOut != NULL)
            {
                if (e.is_verbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    const wchar_t* wcsVarName;
                    if (pVar)
                    {
                        ostr << pVar->name_get().name_get() << L"  = " << std::endl;
                        wcsVarName = pVar->name_get().name_get().c_str();
                    }
                    else
                    {
                        ostr << L"???" << L"  = " << std::endl;
                        wcsVarName = L"???";
                    }

                    ostr << std::endl;
                    VariableToString(pOut, wcsVarName);
                }
            }
            else
            {
                //manage error
                std::wostringstream os;
                os << _W("Invalid Index.\n");
                throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            return;
        }

        CallExp *pCall = dynamic_cast<CallExp*>(&e.left_exp_get());
        if (pCall)
        {
            //x(?) = ?
            InternalType *pOut	= NULL;

            /*getting what to assign*/
            InternalType* pITR = e.right_val_get();
            if (pITR == NULL)
            {
                e.right_exp_get().accept(*this);
                pITR = result_get();
                //reset result
                result_set(NULL);
            }

            if (pITR == NULL)
            {
                // if the right hand is NULL.
                std::wostringstream os;
                os << _W("Unable to extract right part expression.\n");
                throw ast::ScilabError(os.str(), 999, e.left_exp_get().location_get());
            }

            std::list<ExpHistory*> fields;
            if (getFieldsFromExp(pCall, fields) == false)
            {
                std::wostringstream os;
                os << _W("Get fields from expression failed.");
                throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            pOut = evaluateFields(pCall, fields, pITR);

            if (pOut == NULL)
            {
                std::wostringstream os;
                os << _W("Fields evaluation failed.");
                throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            if (e.is_verbose() && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                const wchar_t* wcsVarName;

                ostr << *getStructNameFromExp(&pCall->name_get()) << L"  = " << std::endl;
                wcsVarName = getStructNameFromExp(&pCall->name_get())->c_str();

                ostr << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pOut, wcsVarName);
            }

            if (pITR->isDeletable())
            {
                delete pITR;
            }

            result_clear();
            return;
        }

        AssignListExp *pList = dynamic_cast<AssignListExp*>(&e.left_exp_get());
        if (pList)
        {
            //[x,y] = ?
            int iLhsCount = (int)pList->exps_get().size();

            /*getting what to assign*/
            T exec;
            exec.expected_setSize(iLhsCount);
            e.right_exp_get().accept(exec);

            if (exec.result_getSize() != iLhsCount)
            {
                std::wostringstream os;
                os << _W("Incompatible assignation: trying to assign ") << exec.result_getSize();
                os << _W(" values in ") << iLhsCount << _W(" variables.") << std::endl;
                throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            std::list<Exp *>::const_reverse_iterator it;
            int i = (int)iLhsCount - 1;
            for (it = pList->exps_get().rbegin() ; it != pList->exps_get().rend() ; it++, i--)
            {
                //create a new AssignExp and run it
                types::InternalType* pIT = exec.result_get(i);
                pIT->IncreaseRef();
                AssignExp* pAssign = new AssignExp((*it)->location_get(), *(*it), *const_cast<Exp*>(&e.right_exp_get()), pIT);
                pAssign->set_verbose(e.is_verbose());
                pAssign->accept(*this);
                pIT->DecreaseRef();
                //clear result to take care of [n,n]
                exec.result_set(i, NULL);
            }
            exec.result_clear();
            return;
        }

        FieldExp *pField = dynamic_cast<FieldExp*>(&e.left_exp_get());
        if (pField)
        {
            //a.b = x
            //a.b can be a struct or a tlist/mlist or a handle
            /*getting what to assign*/
            expected_setSize(1);
            e.right_exp_get().accept(*this);
            InternalType *pIT = result_get();
            if (pIT->isImplicitList())
            {
                if (pIT->getAs<ImplicitList>()->isComputable())
                {
                    InternalType *pTemp = pIT->getAs<ImplicitList>()->extractFullMatrix();
                    delete pIT;
                    result_set(NULL);
                    pIT = pTemp;
                }
            }

            std::list<ExpHistory*> fields;
            if (getFieldsFromExp(pField, fields) == false)
            {
                std::wostringstream os;
                os << _W("Get fields from expression failed.");
                throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            if (evaluateFields(pField, fields, pIT) == NULL)
            {
                std::wostringstream os;
                os << _W("Fields evaluation failed.");
                throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            if (e.is_verbose() && ConfigVariable::isPromptShow())
            {
                const wstring *pstName = getStructNameFromExp(pField);

                types::InternalType* pPrint = symbol::Context::getInstance()->get(symbol::Symbol(*pstName));
                std::wostringstream ostr;
                ostr << *pstName << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pPrint, pstName->c_str());
            }

            result_clear();
            return;
        }

        std::wostringstream os;
        os << _W("unknow script form");
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ast::ScilabError(os.str(), 999, e.right_exp_get().location_get());
    }
    catch (ast::ScilabError error)
    {
        throw error;
    }
}
