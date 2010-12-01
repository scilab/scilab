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

// This code is separated in run_AssignExp.hxx
// but will be inlined in runvisitor.hxx
// using #include with RunVisitorT class declaration.
//
// If you need additionnal headers, please add it in runvisitor.hxx

void visitprivate(const AssignExp  &e)
{
    symbol::Context *pcontext = symbol::Context::getInstance();
    /*Create local exec visitor*/
    T execMeR;
    try
    {
        /*get king of left hand*/
        const SimpleVar *pVar				= dynamic_cast<const SimpleVar*>(&e.left_exp_get());
        const FieldExp *pField				= dynamic_cast<const FieldExp*>(&e.left_exp_get());
        const AssignListExp *pList	        = dynamic_cast<const AssignListExp*>(&e.left_exp_get());
        const CallExp *pCall				= dynamic_cast<const CallExp*>(&e.left_exp_get());
        const CellCallExp *pCell    		= dynamic_cast<const CellCallExp*>(&e.left_exp_get());

        if(pCell)
        {
            T execVar;
            InternalType *pIT;
            bool bSeeAsVector   = false;
            bool bRet           = true;
            bool bNew           = false;
            int iProductElem    = (int)pCell->args_get().size();

            pVar = dynamic_cast<const SimpleVar*>(&pCell->name_get());
            if(pVar == NULL)
            {//manage a.b{1} = x
                pCell->name_get().accept(execVar);

                if(execVar.result_get() != NULL && execVar.result_get()->isCell())
                {
                    pIT = execVar.result_get();
                }
                else
                {//never append ?
                    std::wostringstream os;
                    os << _W("Unable to extract left part expression.\n");
                    //os << ((Location)e.left_exp_get().location_get()).location_string_get() << std::endl;
                    throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
                }
            }
            else
            {
                pIT = symbol::Context::getInstance()->get(pVar->name_get());
            }

            bSeeAsVector	= iProductElem == 1;

            /*getting what to assign*/
            e.right_exp_get().accept(execMeR);
            if(pIT == NULL)
            {//Var doesn't exist, create it with good dimensions
                bNew = true;
            }
            else
            {
                if(pIT->isRef(1) == true)
                {
                    pIT = pIT->clone();
                    bNew = true;
                }
            }

            int *piIndexSeq     = NULL;
            int *piMaxDim       = NULL;
            int *piDimSize      = new int[iProductElem];
            int iTotalCombi		= GetIndexList(pIT, pCall->args_get(), &piIndexSeq, &piMaxDim, pIT, piDimSize);
            /*We have the indexlist expanded and the max index*/

            //check we don't have bad indexes like "< 1"
            for(int i = 0 ; i < iTotalCombi * iProductElem; i++)
            {
                if(piIndexSeq[i] < 1)
                {
                    //manage error
                    std::wostringstream os;
                    os << _W("Indexes must be positive .\n");
                    //os << ((Location)e.left_exp_get().location_get()).location_string_get() << std::endl;
                    throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
                }
            }

            InternalType *pOut	= NULL;

            //fisrt extract implicit list
            if(execMeR.result_get()->isImplicitList())
            {
                InternalType *pIL = execMeR.result_get()->getAsImplicitList()->extract_matrix();
                execMeR.result_set(pIL);
            }
            else if(execMeR.result_get()->isContainer())
            {
                InternalType* pIL = execMeR.result_get()->clone();
                execMeR.result_set(pIL);
            }

            if(pIT == NULL)
            {//call static insert function
                pOut = Cell::insert_new(iTotalCombi, piIndexSeq, piMaxDim, dynamic_cast<GenericType*>(execMeR.result_get()), bSeeAsVector);
            }
            else
            {//call type insert function
                bRet = pIT->getAsCell()->insert_cell(iTotalCombi, piIndexSeq, piMaxDim, dynamic_cast<GenericType*>(execMeR.result_get()), bSeeAsVector);
                pOut = pIT;
            }

            if(pOut != NULL && bRet == true)
            {
                if(bNew)
                {
                    symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pOut));
                }

                if(e.is_verbose())
                {
                    std::wostringstream ostr;
                    if(pVar)
                    {
                        ostr << pVar->name_get() << L" = " << std::endl;
                    }
                    else
                    {
                        ostr << L"???" << L" = " << std::endl;
                    }
                    ostr << std::endl;
                    ostr << pOut->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
                    YaspWriteW(ostr.str().c_str());
                }
            }
            else
            {
                //manage error
                std::wostringstream os;
                os << _W("Submatrix incorrectly defined.\n");
                //os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }
            delete piMaxDim;
            delete[] piDimSize;
        }
        else if(pCall)
        {//x(?) = ?
            T execVar;
            InternalType *pIT;
            bool bNew           = false;
            bool bSeeAsVector   = false;
            int iProductElem    = (int)pCall->args_get().size();



            pVar = dynamic_cast<const SimpleVar*>(&pCall->name_get());
            if(pVar == NULL)
            {//manage a.b(1) = x
                pCall->name_get().accept(execVar);

                if(execVar.result_get() != NULL)
                {
                    pIT = execVar.result_get();
                }
                else
                {//never append ?
                    std::wostringstream os;
                    os << _W("Unable to extract left part expression.\n");
                    //os << ((Location)e.left_exp_get().location_get()).location_string_get() << std::endl;
                    throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
                }
            }
            else
            {
                pIT = symbol::Context::getInstance()->get(pVar->name_get());
            }

            bSeeAsVector    = iProductElem == 1;

            /*getting what to assign*/
            e.right_exp_get().accept(execMeR);
            if(pIT == NULL)
            {//Var doesn't exist, create it with good dimensions
                bNew = true;
            }
            else
            {
                if(pIT->isRef(1) == true)
                {
                    pIT = pIT->clone();
                    bNew = true;
                }
            }

            int *piIndexSeq     = NULL;
            int *piMaxDim       = NULL;
            int *piDimSize      = new int[iProductElem];
            int iTotalCombi		= GetIndexList(pIT, pCall->args_get(), &piIndexSeq, &piMaxDim, pIT, piDimSize);

            if(iTotalCombi == 0)
            {//nothing to do
                return;
            }
            /*We have the indexlist expanded and the max index*/

            //check we don't have bad indexes like "< 1"
            for(int i = 0 ; i < iTotalCombi * iProductElem; i++)
            {
                if(piIndexSeq[i] < 1)
                {
                    //manage error
                    std::wostringstream os;
                    os << _W("Indexes must be positive .\n");
                    //os << ((Location)e.left_exp_get().location_get()).location_string_get() << std::endl;
                    throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
                }
            }

            InternalType *pOut	= NULL;

            //fisrt extract implicit list
            if(execMeR.result_get()->isImplicitList())
            {
                InternalType *pIL = execMeR.result_get()->getAsImplicitList()->extract_matrix();
                execMeR.result_set(pIL);
            }
            else if(execMeR.result_get()->isContainer())
            {
                //std::cout << "assign container type during insertion" << std::endl;
                InternalType* pIL = execMeR.result_get()->clone();
                execMeR.result_set(pIL);
            }

            if(pIT == NULL || (pIT->isDouble() && pIT->getAsDouble()->size_get() == 0))
            {//call static insert function
                if(pIT->isDouble() && pIT->getAsDouble()->size_get() == 0)
                {
                    bNew = true;
                }

                switch(execMeR.result_get()->getType())
                {
                case InternalType::RealDouble : 
                    pOut = Double::insert_new(iTotalCombi, piIndexSeq, piMaxDim, execMeR.result_get()->getAsDouble(), bSeeAsVector);
                    break;
                case InternalType::RealBool : 
                    pOut = Bool::insert_new(iTotalCombi, piIndexSeq, piMaxDim, execMeR.result_get()->getAsBool(), bSeeAsVector);
                    break;
                case InternalType::RealString : 
                    pOut = String::insert_new(iTotalCombi, piIndexSeq, piMaxDim, execMeR.result_get()->getAsString(), bSeeAsVector);
                    break;
                case InternalType::RealInt : 
                    pOut = Int::insert_new(iTotalCombi, piIndexSeq, piMaxDim, execMeR.result_get()->getAsInt(), bSeeAsVector);
                    break;
                case InternalType::RealList : 
                    //never occur !
                    break;
                case InternalType::RealCell : 
                    //never occur !
                    break;
                default : 
                    //TOTO YaSp : overlaoding insertion
                    break;
                }
            }
            else
            {//call type insert function
                InternalType* pRet = NULL;
                switch(pIT->getType())
                {
                case InternalType::RealDouble : 
                    pRet = pIT->getAsDouble()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR.result_get(), bSeeAsVector);
                    break;
                case InternalType::RealBool : 
                    pRet = pIT->getAsBool()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR.result_get(), bSeeAsVector);
                    break;
                case InternalType::RealString : 
                    pRet = pIT->getAsString()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR.result_get(), bSeeAsVector);
                    break;
                case InternalType::RealInt : 
                    pRet = pIT->getAsInt()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR.result_get(), bSeeAsVector);
                    break;
                case InternalType::RealList : 
                    pRet = pIT->getAsList()->insert(iTotalCombi, piIndexSeq, piMaxDim, execMeR.result_list_get(), bSeeAsVector);
                    break;
                case InternalType::RealTList : 
                    pRet = pIT->getAsTList()->insert(iTotalCombi, piIndexSeq, piMaxDim, execMeR.result_list_get(), bSeeAsVector);
                    break;
                case InternalType::RealCell : 
                    if(execMeR.result_get()->isCell() == true)
                    {
                        pRet = pIT->getAsCell()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR.result_get(), bSeeAsVector);
                    }
                    else
                    {
                        //manage error
                        std::wostringstream os;
                        os << _W("Right hand argument must be a cell.\n");
                        //os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
                    }
                    break;
                default : 
                    //TODO YaSp : overlaoding insertion
                    break;
                }

                if(pRet && pRet != pIT)
                {
                    //variable change
                    pIT->DecreaseRef();
                    if(pIT->isDeletable())
                    {
                        delete pIT;
                    }
                    bNew = true;
                }
                
                pOut = pRet;
            }

            if(pOut != NULL)
            {
                if(bNew)
                {
                    symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pOut));
                }

                if(e.is_verbose())
                {
                    std::wostringstream ostr;
                    if(pVar)
                    {
                        ostr << pVar->name_get() << L" = " << std::endl;
                    }
                    else
                    {
                        ostr << L"???" << L" = " << std::endl;
                    }
                    ostr << std::endl;
                    ostr << pOut->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
                    YaspWriteW(ostr.str().c_str());
                }
            }
            else
            {
                //manage error
                std::wostringstream os;
                os << _W("Submatrix incorrectly defined.\n");
                //os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }
            delete piMaxDim;
            delete[] piDimSize;
        }
        else if(pVar)
        {// x = ?
            /*getting what to assign*/
            execMeR.expected_size_set(1);
            e.right_exp_get().accept(execMeR);

            if(execMeR.result_size_get() != 1)
            {
                std::wostringstream os;
                os << L"Lhs != Rhs";
                //os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }
				
            InternalType *pIT	=	execMeR.result_get();
            if(pIT->isImplicitList())
            {
                if(pIT->getAsImplicitList()->computable())
                {
                    InternalType *pTemp = pIT->getAsImplicitList()->extract_matrix();
                    delete pIT;
                    execMeR.result_set(NULL);
                    pIT = pTemp;
                }
            }

            const ReturnExp *pReturn = dynamic_cast<const ReturnExp*>(&e.right_exp_get());
            if(pReturn)
            {//ReturnExp so, put the value in the previous scope
                symbol::Context::getInstance()->put_in_previous_scope(pVar->name_get(), *((GenericType*)pIT));
                ((AssignExp*)&e)->break_set();
            }
            else
            {
                symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pIT));
            }

            if(e.is_verbose())
            {
                std::wostringstream ostr;
                ostr << pVar->name_get() << L" = " << std::endl << std::endl;
                ostr << pIT->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
                YaspWriteW(ostr.str().c_str());
            }
        }
        else if(pList)
        {//[x,y] = ?

            int iLhsCount = (int)pList->exps_get().size();

            /*getting what to assign*/
            execMeR.expected_size_set(iLhsCount);
            e.right_exp_get().accept(execMeR);

            if(execMeR.result_size_get() != execMeR.expected_size_get())
            {
                std::wostringstream os;
                os << L"Lhs != Rhs";
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }


            std::list<Exp *>::const_reverse_iterator it;
            int i = (int)iLhsCount - 1;
            for(it = pList->exps_get().rbegin() ; it != pList->exps_get().rend() ; it++)
            {
                const SimpleVar *pListVar	= dynamic_cast<const SimpleVar*>((*it));
                symbol::Context::getInstance()->put(pListVar->name_get(), *((GenericType*)execMeR.result_get(i)));
                if(e.is_verbose())
                {
                    std::wostringstream ostr;
                    ostr << pListVar->name_get() << L" = " << std::endl;
                    ostr << std::endl;
                    ostr << execMeR.result_get(i)->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
                    YaspWriteW(ostr.str().c_str());
                }
                i--;
            }
        }
        else if(pField)
        {//a.b = x
            //a.b can be a struct or a tlist/mlist
            InternalType *pHead = NULL;
            Struct* pStr = getStructFromExp(pField->head_get());
            if(pStr != NULL)
            {
                pHead = pStr;
            }
            else
            {
                //a is not a struct
                const SimpleVar* pVar =  dynamic_cast<const SimpleVar*>(pField->head_get());
                if(pVar == NULL)
                {
                    std::cout << "Houston ..." << std::endl;
                }
                pHead = symbol::Context::getInstance()->get(pVar->name_get());
            }

            //if a is already assign, make a copy and replace it
            if(pHead->isRef(1) == true)
            {
                pHead = pHead->clone();
                const wstring *pstName = getStructNameFromExp(pField);
                symbol::Context::getInstance()->put(*pstName, *pHead);
            }
            /*getting what to assign*/
            execMeR.expected_size_set(1);
            e.right_exp_get().accept(execMeR);

            //we can assign only one value
            if(execMeR.result_size_get() != 1)
            {
                std::wostringstream os;
                os << L"Lhs != Rhs";
                //os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            InternalType *pIT = execMeR.result_get();
            if(pIT->isImplicitList())
            {
                if(pIT->getAsImplicitList()->computable())
                {
                    InternalType *pTemp = pIT->getAsImplicitList()->extract_matrix();
                    delete pIT;
                    execMeR.result_set(NULL);
                    pIT = pTemp;
                }
            }
            else if(pIT->isContainer())
            {//if assigned value is a container, copy it before assign.
                //std::cout << "assign container type to field" << std::endl;
                pIT = pIT->clone();
            }

            //assign result to new field
            const SimpleVar* pTail =  dynamic_cast<const SimpleVar*>(pField->tail_get());

            if(pHead->isStruct())
            {
                pHead->getAsStruct()->add(pTail->name_get(), pIT);
            }
            else if(pHead->isTList())
            {
                TList* pT = pHead->getAsTList();
                if(pT->exists(pTail->name_get()))
                {
                    pT->set(pTail->name_get(), pIT);
                }
                else
                {
                    std::wostringstream os;
                    os << L"Field must be exist";
                    throw ScilabError(os.str(), 999, pVar->location_get());
                }
            }
            else if(pHead->isMList())
            {
                //TODO:
            }

            if(e.is_verbose())
            {
                const wstring *pstName = getStructNameFromExp(pField);

                std::wostringstream ostr;
                ostr << *pstName << L" = " << std::endl << std::endl;
                ostr << symbol::Context::getInstance()->get(*pstName)->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
                YaspWriteW(ostr.str().c_str());
            }
        }
        else
        {//Houston ...
            std::wostringstream os;
            os << L"unknow script form";
            //os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
            throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
        }
    }
    catch(ScilabError error)
    {
        throw error;
    }
}
