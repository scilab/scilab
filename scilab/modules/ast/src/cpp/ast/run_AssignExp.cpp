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
        if (e.getLeftExp().isSimpleVar())
        {
            pVar = static_cast<SimpleVar*>(&e.getLeftExp());
        }

        /*get king of left hand*/
        if (pVar)
        {
            // x = ?
            /*getting what to assign*/
            InternalType *pIT = e.getRightVal();
            if (pIT == NULL)
            {
                setExpectedSize(1);
                e.getRightExp().accept(*this);

                if (getResultSize() != 1)
                {
                    std::wostringstream os;
                    os << _W("Can not assign multiple value in a single variable") << std::endl;
                    //os << ((Location)e.getRightExp().getLocation()).getLocationString() << std::endl;
                    throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
                }

                pIT = getResult();
                //reset result
                setResult(NULL);
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
                    symbol::Context::getInstance()->remove(pVar->getSymbol());
                }

                setResult(NULL);
                return;
            }

            if (e.getLeftExp().isReturnExp())
            {
                //ReturnExp so, put the value in the previous scope
                symbol::Context::getInstance()->putInPreviousScope(pVar->getStack(), pIT);
                ((AssignExp*)&e)->setReturn();
            }
            else
            {
                symbol::Context::getInstance()->put(pVar->getStack(), pIT);
            }

            if (e.isVerbose() && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << pVar->getSymbol().getName() << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pIT, pVar->getSymbol().getName().c_str());
            }
            return;
        }

        CellCallExp *pCell = dynamic_cast<CellCallExp*>(&e.getLeftExp());
        if (pCell)
        {
            InternalType *pOut	= NULL;

            /*getting what to assign*/
            InternalType* pITR = e.getRightVal();
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
                throw ast::ScilabError(os.str(), 999, e.getLeftExp().getLocation());
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
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            pOut = evaluateFields(pCell, fields, pITR);
            for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
            {
                delete *i;
            }

            if (pOut == NULL)
            {
                std::wostringstream os;
                os << _W("Fields evaluation failed.");
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            if (pOut != NULL)
            {
                if (e.isVerbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    const wchar_t* wcsVarName;
                    if (pVar)
                    {
                        ostr << pVar->getSymbol().getName() << L"  = " << std::endl;
                        wcsVarName = pVar->getSymbol().getName().c_str();
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
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            return;
        }

        CallExp *pCall = dynamic_cast<CallExp*>(&e.getLeftExp());
        if (pCall)
        {
            //x(?) = ?
            InternalType *pOut	= NULL;

            /*getting what to assign*/
            InternalType* pITR = e.getRightVal();
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
                throw ast::ScilabError(os.str(), 999, e.getLeftExp().getLocation());
            }

            std::list<ExpHistory*> fields;
            if (getFieldsFromExp(pCall, fields) == false)
            {
                for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                {
                    delete *i;
                }
                std::wostringstream os;
                os << _W("Get fields from expression failed.");
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            pOut = evaluateFields(pCall, fields, pITR);
            for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
            {
                delete *i;
            }

            if (pOut == NULL)
            {
                std::wostringstream os;
                os << _W("Fields evaluation failed.");
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            if (e.isVerbose() && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                const wchar_t* wcsVarName;

                ostr << *getStructNameFromExp(&pCall->getName()) << L"  = " << std::endl;
                wcsVarName = getStructNameFromExp(&pCall->getName())->c_str();

                ostr << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pOut, wcsVarName);
            }

            pITR->killMe();

            clearResult();
            return;
        }

        AssignListExp *pList = dynamic_cast<AssignListExp*>(&e.getLeftExp());
        if (pList)
        {
            //[x,y] = ?
            int iLhsCount = (int)pList->getExps().size();

            /*getting what to assign*/
            T exec;
            exec.setExpectedSize(iLhsCount);
            e.getRightExp().accept(exec);

            if (exec.getResultSize() != iLhsCount)
            {
                std::wostringstream os;
                os << _W("Incompatible assignation: trying to assign ") << exec.getResultSize();
                os << _W(" values in ") << iLhsCount << _W(" variables.") << std::endl;
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            std::list<Exp *>::const_reverse_iterator it;
            int i = (int)iLhsCount - 1;
            for (it = pList->getExps().rbegin() ; it != pList->getExps().rend() ; it++, i--)
            {
                //create a new AssignExp and run it
                types::InternalType* pIT = exec.getResult(i);
                //protect temporary result from delete
                pIT->IncreaseRef();
                AssignExp pAssign((*it)->getLocation(), *(*it), *const_cast<Exp*>(&e.getRightExp()), pIT);
                pAssign.setLrOwner(false);
                pAssign.setVerbose(e.isVerbose());
                pAssign.accept(*this);
                //unprotect temporary result
                pIT->DecreaseRef();
                //clear result to take care of [n,n]
                exec.setResult(i, NULL);
            }
            exec.clearResult();
            return;
        }

        FieldExp *pField = dynamic_cast<FieldExp*>(&e.getLeftExp());
        if (pField)
        {
            //a.b = x
            //a.b can be a struct or a tlist/mlist or a handle
            /*getting what to assign*/
            setExpectedSize(1);
            e.getRightExp().accept(*this);
            InternalType *pIT = getResult();
            if (pIT->isImplicitList())
            {
                if (pIT->getAs<ImplicitList>()->isComputable())
                {
                    InternalType *pTemp = pIT->getAs<ImplicitList>()->extractFullMatrix();
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
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            if (evaluateFields(pField, fields, pIT) == NULL)
            {
                for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
                {
                    delete *i;
                }
                std::wostringstream os;
                os << _W("Fields evaluation failed.");
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            for (std::list<ExpHistory*>::const_iterator i = fields.begin(), end = fields.end(); i != end; i++)
            {
                delete *i;
            }

            if (e.isVerbose() && ConfigVariable::isPromptShow())
            {
                const wstring *pstName = getStructNameFromExp(pField);

                types::InternalType* pPrint = symbol::Context::getInstance()->get(symbol::Symbol(*pstName));
                std::wostringstream ostr;
                ostr << *pstName << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pPrint, pstName->c_str());
            }

            clearResult();
            return;
        }

        std::wostringstream os;
        os << _W("unknow script form");
        //os << ((Location)e.getRightExp().getLocation()).getLocationString() << std::endl;
        throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
    }
    catch (ast::ScilabError error)
    {
        throw error;
    }
}
