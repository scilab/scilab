/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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
void RunVisitorT<T>::visitprivate(const AssignExp  &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    symbol::Context* ctx = symbol::Context::getInstance();
    /*Create local exec visitor*/
    try
    {
        SimpleVar * pVar = NULL;
        if (e.getLeftExp().isSimpleVar())
        {
            pVar = static_cast<SimpleVar*>(&e.getLeftExp());
        }

        /*get king of left hand*/
        if (pVar)
        {
            // x = ?
            /*getting what to assign*/
            types::InternalType *pIT = e.getRightVal();
            if (pIT == NULL)
            {
                setExpectedSize(1);
                e.getRightExp().accept(*this);

                if (getResultSize() != 1)
                {
                    std::wostringstream os;
                    os << _W("Can not assign multiple value in a single variable") << std::endl;
                    //os << ((Location)e.getRightExp().getLocation()).getLocationString() << std::endl;
                    throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
                }

                pIT = getResult();
                //reset result
                setResult(NULL);
            }

            if (pIT->isImplicitList())
            {
                if (pIT->getAs<types::ImplicitList>()->isComputable())
                {
                    types::InternalType *pTemp = pIT->getAs<types::ImplicitList>()->extractFullMatrix();
                    pIT->killMe();
                    pIT = pTemp;
                }
            }

            if (pIT->isAssignable() == false)
            {
                if (pIT->isListDelete())
                {
                    //used to delete a variable in current scope
                    symbol::Symbol sym = pVar->getSymbol();
                    if (ctx->isprotected(sym) == false)
                    {
                        ctx->remove(sym);
                    }
                    else
                    {
                        std::wostringstream os;
                        os << _W("Redefining permanent variable.\n");
                        throw ast::InternalError(os.str(), 999, e.getLeftExp().getLocation());
                    }
                }

                setResult(NULL);
                CoverageInstance::stopChrono((void*)&e);
                return;
            }

            if (pIT->isList() && pIT->getRef() > 0)
            {
                // Prevent modification of all scilab variable
                // which point to this container when it is used
                // in setfield scilab function.
                // A clone on a container will not clone what it contain.
                pIT = pIT->clone();
            }

            if (e.getRightExp().isReturnExp())
            {
                //ReturnExp so, put the value in the previous scope
                if (ctx->putInPreviousScope(pVar->getStack(), pIT) == false)
                {
                    char pstError[1024];
                    char* pstFuncName = wide_string_to_UTF8(pVar->getSymbol().getName().data());
                    os_sprintf(pstError, _("It is not possible to redefine the %s primitive this way (see clearfun).\n"), pstFuncName);
                    wchar_t* pwstError = to_wide_string(pstError);
                    std::wstring wstError(pwstError);
                    FREE(pstFuncName);
                    FREE(pwstError);
                    pIT->killMe();
                    CoverageInstance::stopChrono((void*)&e);
                    throw InternalError(wstError, 999, e.getLocation());
                }

                ((AssignExp*)&e)->setReturn();
            }
            else
            {
                if (ctx->isprotected(pVar->getStack()) == false)
                {
                    ctx->put(pVar->getStack(), pIT);
                }
                else
                {
                    std::wostringstream os;
                    os << _W("Redefining permanent variable.\n");
                    throw ast::InternalError(os.str(), 999, e.getLeftExp().getLocation());
                }
            }

            if (e.isVerbose() && ConfigVariable::isPrintOutput())
            {
                std::wstring wstrName = pVar->getSymbol().getName();
                std::wostringstream ostr;
                ostr << L" " << wstrName << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                std::wostringstream ostrName;
                ostrName << wstrName;
                VariableToString(pIT, ostrName.str().c_str());
            }
            CoverageInstance::stopChrono((void*)&e);
            return;
        }

        if (e.getLeftExp().isCellCallExp())
        {
            CellCallExp *pCell = static_cast<CellCallExp*>(&e.getLeftExp());
            types::InternalType *pOut = NULL;

            /*getting what to assign*/
            types::InternalType* pITR = e.getRightVal();
            if (pITR == NULL)
            {
                e.getRightExp().accept(*this);
                pITR = getResult();
                //reset result
                setResult(NULL);
            }

            if (pITR == NULL)
            {
                // if the right hand is NULL.
                std::wostringstream os;
                os << _W("Unable to extract right part expression.\n");
                throw ast::InternalError(os.str(), 999, e.getLeftExp().getLocation());
            }

            std::list<ExpHistory*> fields;
            if (getFieldsFromExp(pCell, fields) == false)
            {
                for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                {
                    delete *i;
                }
                std::wostringstream os;
                os << _W("Get fields from expression failed.");
                throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
            }

            try
            {
                pOut = evaluateFields(pCell, fields, pITR);
            }
            catch (const InternalError& error)
            {
                // catch error when call overload
                for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                {
                    (*i)->setDeleteCurrent(true);
                    delete *i;
                }

                pITR->killMe();
                throw error;
            }

            for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
            {
                delete *i;
            }

            pITR->killMe();

            if (pOut == NULL)
            {
                std::wostringstream os;
                os << _W("Fields evaluation failed.");
                throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
            }

            if (pOut != NULL)
            {
                if (e.isVerbose() && ConfigVariable::isPrintOutput())
                {
                    std::wostringstream ostr;
                    ostr << L" " << *getStructNameFromExp(pCell) << L"  = " << std::endl;
                    ostr << std::endl;
                    scilabWriteW(ostr.str().c_str());

                    VariableToString(pOut, ostr.str().c_str());
                }
            }
            else
            {
                //manage error
                std::wostringstream os;
                os << _W("Invalid Index.\n");
                throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
            }

            CoverageInstance::stopChrono((void*)&e);
            return;
        }

        if (e.getLeftExp().isCallExp())
        {
            CallExp *pCall = static_cast<CallExp*>(&e.getLeftExp());
            //x(?) = ?
            types::InternalType *pOut = NULL;

            if (e.getRightExp().isReturnExp())
            {
                // We can't put in the previous scope a variable create like that : a(2)=resume(1)
                std::wostringstream os;
                os << _W("Indexing not allowed for output arguments of resume.\n");
                throw ast::InternalError(os.str(), 79, e.getLeftExp().getLocation());
            }

            /*getting what to assign*/
            types::InternalType* pITR = e.getRightVal();
            if (pITR == NULL)
            {
                e.getRightExp().accept(*this);
                pITR = getResult();
                //reset result
                setResult(NULL);
            }

            if (pITR == NULL)
            {
                // if the right hand is NULL.
                std::wostringstream os;
                os << _W("Unable to extract right part expression.\n");
                throw ast::InternalError(os.str(), 999, e.getLeftExp().getLocation());
            }

            bool alreadyProcessed = false;
            //a(...) without fields or whatever on arrayof derived types
            if (pCall->getName().isSimpleVar())
            {
                ast::SimpleVar* var = pCall->getName().getAs<ast::SimpleVar>();
                types::InternalType* pIT = ctx->getCurrentLevel(var->getStack());
                if (pIT && pIT->isArrayOf())
                {
                    if (ctx->isprotected(var->getStack()))
                    {
                        std::wostringstream os;
                        os << _W("Redefining permanent variable.\n");
                        throw ast::InternalError(os.str(), 999, pCall->getLocation());
                    }

                    // prevent delete after extractFullMatrix
                    // called in insertionCall when pITR is an ImplicitList
                    pITR->IncreaseRef();

                    types::typed_list* currentArgs = GetArgumentList(pCall->getArgs());

                    try
                    {
                        pOut = insertionCall(e, currentArgs, pIT, pITR);
                    }
                    catch (const InternalError& error)
                    {
                        pITR->DecreaseRef();
                        // call killMe on all arguments
                        cleanOut(*currentArgs);
                        delete currentArgs;
                        // insertion have done, call killMe on pITR
                        pITR->killMe();
                        throw error;
                    }

                    pITR->DecreaseRef();

                    // call killMe on all arguments
                    cleanOut(*currentArgs);
                    delete currentArgs;

                    // insertion have done, call killMe on pITR
                    pITR->killMe();

                    if (pOut == NULL)
                    {
                        std::wostringstream os;
                        os << _W("Submatrix incorrectly defined.\n");
                        throw ast::InternalError(os.str(), 999, e.getLocation());
                    }


                    //update variable with new value
                    if (pOut != pIT)
                    {
                        ctx->put(var->getStack(), pOut);
                    }

                    alreadyProcessed = true;
                }
            }

            if (alreadyProcessed == false)
            {
                std::list<ExpHistory*> fields;
                if (getFieldsFromExp(pCall, fields) == false)
                {
                    for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                    {
                        delete *i;
                    }

                    std::wostringstream os;
                    os << _W("Instruction left hand side: waiting for a name.");
                    throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
                }

                // prevent delete after extractFullMatrix
                // called in evaluateFields when pITR is an ImplicitList
                pITR->IncreaseRef();

                try
                {
                    pOut = evaluateFields(pCall, fields, pITR);
                }
                catch (const InternalError& error)
                {
                    // catch error when call overload
                    for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                    {
                        delete *i;
                    }

                    pITR->DecreaseRef();
                    pITR->killMe();

                    throw error;
                }

                for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                {
                    delete *i;
                }

                pITR->DecreaseRef();
                pITR->killMe();

                if (pOut == NULL)
                {
                    std::wostringstream os;
                    os << _W("Fields evaluation failed.");
                    throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
                }
            }

            if (e.isVerbose() && ConfigVariable::isPrintOutput())
            {
                std::wostringstream ostr;
                ostr << L" " << *getStructNameFromExp(&pCall->getName()) << L"  = " << std::endl;
                ostr << std::endl;
                scilabWriteW(ostr.str().c_str());

                std::wostringstream ostrName;
                ostrName << *getStructNameFromExp(&pCall->getName());
                VariableToString(pOut, ostrName.str().c_str());
            }

            clearResult();
            CoverageInstance::stopChrono((void*)&e);

            return;
        }

        if (e.getLeftExp().isAssignListExp())
        {
            AssignListExp *pList = e.getLeftExp().getAs<AssignListExp>();
            //[x,y] = ?
            int iLhsCount = (int)pList->getExps().size();

            /*getting what to assign*/
            T exec;
            exec.setExpectedSize(iLhsCount);
            e.getRightExp().accept(exec);

            if (exec.getResultSize() < iLhsCount)
            {
                std::wostringstream os;
                os << _W("Incompatible assignation: trying to assign ") << exec.getResultSize();
                os << _W(" values in ") << iLhsCount << _W(" variables.") << std::endl;
                throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
            }

            exps_t::const_reverse_iterator it;
            exps_t exps = pList->getExps();
            types::InternalType** pIT = new types::InternalType*[iLhsCount];
            int i = 0;
            for (i = iLhsCount - 1; i >= 0; i--)
            {
                //create a new AssignExp and run it
                pIT[i] = exec.getResult(i);
                //protet rhs against removal [a,b] = (b,a);
                pIT[i]->IncreaseRef();
            }

            for (i = iLhsCount - 1, it = exps.rbegin(); it != exps.rend(); it++, i--)
            {
                Exp* pExp = e.getRightExp().clone();
                AssignExp pAssign((*it)->getLocation(), *(*it), *pExp, pIT[i]);
                pAssign.setLrOwner(false);
                pAssign.setVerbose(e.isVerbose());
                pAssign.accept(*this);
                //clear result to take care of [n,n]
                exec.setResult(i, NULL);
                delete pExp;
            }

            for (i = iLhsCount - 1; i >= 0; i--)
            {
                //unprotect rhs
                pIT[i]->DecreaseRef();
                pIT[i]->killMe();
            }

            delete[] pIT;
            exec.clearResult();
            CoverageInstance::stopChrono((void*)&e);
            return;
        }

        if (e.getLeftExp().isFieldExp())
        {
            FieldExp *pField = static_cast<FieldExp*>(&e.getLeftExp());
            types::InternalType *pIT = e.getRightVal();
            if (pIT == NULL)
            {
                //a.b = x
                //a.b can be a struct or a tlist/mlist or a handle
                /*getting what to assign*/
                setExpectedSize(1);
                e.getRightExp().accept(*this);
                pIT = getResult();
                setResult(NULL);
            }

            if (pIT->isImplicitList())
            {
                if (pIT->getAs<types::ImplicitList>()->isComputable())
                {
                    types::InternalType *pTemp = pIT->getAs<types::ImplicitList>()->extractFullMatrix();
                    delete pIT;
                    setResult(NULL);
                    pIT = pTemp;
                }
            }

            std::list<ExpHistory*> fields;
            if (getFieldsFromExp(pField, fields) == false)
            {
                for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                {
                    delete *i;
                }
                std::wostringstream os;
                os << _W("Get fields from expression failed.");
                throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
            }

            try
            {
                if (evaluateFields(pField, fields, pIT) == NULL)
                {
                    for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                    {
                        delete *i;
                    }
                    std::wostringstream os;
                    os << _W("Fields evaluation failed.");
                    throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
                }
            }
            catch (const InternalError& error)
            {
                for (auto i : fields)
                {
                    delete i;
                }

                throw error;
            }

            for (auto i : fields)
            {
                delete i;
            }

            if (e.isVerbose() && ConfigVariable::isPrintOutput())
            {
                const std::wstring *pstName = getStructNameFromExp(pField);

                types::InternalType* pPrint = ctx->get(symbol::Symbol(*pstName));
                std::wostringstream ostr;
                ostr << L" " << *pstName << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());

                std::wostringstream ostrName;
                ostrName << *pstName;
                VariableToString(pPrint, ostrName.str().c_str());
            }

            clearResult();
            CoverageInstance::stopChrono((void*)&e);
            return;
        }

        std::wostringstream os;
        os << _W("unknown script form");
        //os << ((Location)e.getRightExp().getLocation()).getLocationString() << std::endl;
        throw ast::InternalError(os.str(), 999, e.getRightExp().getLocation());
    }
    catch (const InternalError& error)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw error;
    }
}

} /* namespace ast */
