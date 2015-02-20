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
namespace ast {

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

            if(pIT->isList() && pIT->getRef() > 0)
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
                symbol::Context::getInstance()->putInPreviousScope(pVar->getStack(), pIT);
                ((AssignExp*)&e)->setReturn();
            }
            else
            {
                symbol::Context::getInstance()->put(pVar->getStack(), pIT);
            }

            if (e.isVerbose() && ConfigVariable::isPromptShow())
            {
                std::wstring wstrName = pVar->getSymbol().getName();
                std::wostringstream ostr;
                ostr << wstrName << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                std::wostringstream ostrName;
                ostrName << wstrName;
                VariableToString(pIT, ostrName.str().c_str());
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

            try
            {
                pOut = evaluateFields(pCell, fields, pITR);
            }
            catch (ast::ScilabError error)
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
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            if (pOut != NULL)
            {
                if (e.isVerbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    ostr << *getStructNameFromExp(pCell) << L"  = " << std::endl;
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
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            return;
        }

        CallExp *pCall = dynamic_cast<CallExp*>(&e.getLeftExp());
        if (pCall)
        {
            //x(?) = ?
            InternalType *pOut = NULL;

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
                os << _W("Instruction left hand side: waiting for a name.");
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            // prevent delete after extractFullMatrix
            // called in evaluateFields when pITR is an ImplicitList
            pITR->IncreaseRef();

            try
            {
                pOut = evaluateFields(pCall, fields, pITR);
            }
            catch (ast::ScilabError error)
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
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            if (e.isVerbose() && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << *getStructNameFromExp(&pCall->getName()) << L"  = " << std::endl;
                ostr << std::endl;
                scilabWriteW(ostr.str().c_str());

                std::wostringstream ostrName;
                ostrName << *getStructNameFromExp(&pCall->getName());
                VariableToString(pOut, ostrName.str().c_str());
            }

            clearResult();
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

            if (exec.getResultSize() != iLhsCount)
            {
                std::wostringstream os;
                os << _W("Incompatible assignation: trying to assign ") << exec.getResultSize();
                os << _W(" values in ") << iLhsCount << _W(" variables.") << std::endl;
                throw ast::ScilabError(os.str(), 999, e.getRightExp().getLocation());
            }

            exps_t::const_reverse_iterator it;
            exps_t exps = pList->getExps();
            types::InternalType** pIT = new types::InternalType*[iLhsCount];
            int i = 0;
            for (i = iLhsCount - 1; i >= 0; i--)
            {
                //create a new AssignExp and run it
                pIT[i] = (exec.getResult(i))->clone();
            }

            for (i = iLhsCount - 1, it = exps.rbegin(); it != exps.rend(); it++, i--)
            {
                AssignExp pAssign((*it)->getLocation(), *(*it), *e.getRightExp().clone(), pIT[i]);
                pAssign.setLrOwner(false);
                pAssign.setVerbose(e.isVerbose());
                pAssign.accept(*this);
                //clear result to take care of [n,n]
                exec.setResult(i, NULL);
            }
            exec.clearResult();
            return;
        }

        FieldExp *pField = dynamic_cast<FieldExp*>(&e.getLeftExp());
        if (pField)
        {
            InternalType *pIT = e.getRightVal();
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

                std::wostringstream ostrName;
                ostrName << *pstName;
                VariableToString(pPrint, ostrName.str().c_str());
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

} /* namespace ast */
