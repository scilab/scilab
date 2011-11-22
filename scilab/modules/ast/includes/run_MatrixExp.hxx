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

// This code is separated in run_MatrixExp.hxx
// but will be inlined in runvisitor.hxx
// using #include with RunVisitorT class declaration.
//
// If you need additionnal headers, please add it in runvisitor.hxx

/*
  [1,2;3,4] with/without special character $ and :
*/
void visitprivate(const MatrixExp &e)
{
    try
    {
        std::list<MatrixLineExp *>::const_iterator row;
        std::list<Exp *>::const_iterator col;
        InternalType *poResult = NULL;
        list<InternalType*> rowList;
        int iRows = 0;
        int iCols = 0;

        if(e.lines_get().size() == 0)
        {
            result_set(Double::Empty());
            return;
        }

        //do all [x,x]
        for(row = e.lines_get().begin() ; row != e.lines_get().end() ; row++)
        {
            InternalType* poRow = NULL;
            for(col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; col++)
            {
                int iCurCol = 0;
                //poResult = [poResult, col]

                (*col)->accept(*this);

                if(result_get()->isGenericType() == false)
                {
                    std::wostringstream os;
                    os << L"unable to concatenate\n";
                    throw ScilabError(os.str(), 999, (*col)->location_get());
                }

                InternalType *pIT = result_get();
                GenericType* pGT = pIT->getAs<GenericType>();
                if(pGT->isDouble() && pGT->getAs<Double>()->isEmpty())
                {
                    continue;
                }

                if(poRow == NULL)
                {//first loop
                    poRow = pGT;
                    continue;
                }

                if(pGT->isImplicitList() && pGT->getAs<ImplicitList>()->isComputable())
                {
                    ImplicitList *pIL = pGT->getAs<ImplicitList>();
                    InternalType* pIT = pIL->extractFullMatrix();
                    if(pGT->isDeletable())
                    {
                        delete pGT;
                    }
                    pGT = pIT->getAs<GenericType>();
                }

                GenericType* pGTResult = poRow->getAs<GenericType>();
                //check dimension
                if(pGT->getDims() != 2 || pGT->getRows() != pGTResult->getRows())
                {
                    std::wostringstream os;
                    os << L"inconsistent row/column dimensions\n";
                    throw ScilabError(os.str(), 999, (*row)->location_get());
                }

                InternalType *p = AddElementToVariable(NULL, poRow, pGTResult->getRows(), pGTResult->getCols() + pGT->getCols());
                p = AddElementToVariable(p, pGT, 0, pGTResult->getCols());
                if(poRow->isDeletable())
                {
                    delete poRow;
                }

                if(pGT->isDeletable())
                {
                    delete pGT;
                }
                poRow = p;
            }

            GenericType* pGT = poRow->getAs<GenericType>();
            if(poResult == NULL)
            {
                poResult = pGT;
                continue;
            }

            //check dimension
            GenericType* pGTResult = poResult->getAs<GenericType>();
            //check dimension
            if(pGT->getCols() != pGTResult->getCols())
            {
                std::wostringstream os;
                os << L"inconsistent row/column dimensions\n";
                throw ScilabError(os.str(), 999, (*e.lines_get().begin())->location_get());
            }

            InternalType *p = AddElementToVariable(NULL, poResult, pGTResult->getRows() + pGT->getRows(), pGT->getCols());
            p = AddElementToVariable(p, pGT, pGTResult->getRows(), 0);
            if(poResult->isDeletable())
            {
                delete poResult;
            }

            if(pGT->isDeletable())
            {
                delete pGT;
            }
            
            poResult = p;
        }

        if(poResult)
        {
            result_set(poResult);
        }
        else
        {
            result_set(Double::Empty());
        }


////			int iRows = e.lines_get().size();
////			int iCols	= -1;
//        int iRows = 0;
//        int iCols = -1;
//        int iCurRow = -1;
//        int iCurCol = 0;
//        InternalType *poResult = NULL;
//
//        std::list<MatrixLineExp *>::const_iterator	row;
//        std::list<Exp *>::const_iterator	col;
//        //store all element after evaluation
//        if(e.lines_get().size() == 0)
//        {
//            poResult = Double::Empty();
//        }
//        else
//        {
//            list<list<InternalType*> > MatrixList;
//            for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
//            {
//                list<InternalType*> RowList;
//                for (col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; ++col)
//                {
//                    (*col)->accept(*this);
//
//                    if(result_get()->isImplicitList())
//                    {
//                        ImplicitList* pIL = result_get()->getAs<ImplicitList>();
//                        if(pIL->isComputable() == true)
//                        {
//                            InternalType* pIT = pIL->extractFullMatrix();
//                            if(result_get()->isDeletable())
//                            {
//                                delete result_get();
//                            }
//                            result_set(pIT);
//                            iCurCol += ((GenericType*)pIL)->getCols();
//                        }
//                        else
//                        {
//                            iCurCol++;
//                        }
//                    }
//                    else if(result_get() == NULL)
//                    {
//                        result_set(Double::Empty());
//                    }
//                    else
//                    {
//                        iCurCol += ((GenericType*)result_get())->getCols();
//                    }
//
//                    if(iCurRow == -1)
//                    {
//                        iCurRow = ((GenericType*)result_get())->getRows();
//                        if(iCurRow == 0)
//                        {//to manage [] in MatrixExp
//                            iCurRow = -1;
//                        }
//                    }
//                    else if(iCurRow != ((GenericType*)result_get())->getRows() && ((GenericType*)result_get())->getRows() != 0)
//                    {
//                        std::wostringstream os;
//                        os << L"inconsistent row/column dimensions\n";
//                        //os << ((Location)(*row)->location_get()).location_getString() << std::endl;
//                        throw ScilabError(os.str(), 999, (*row)->location_get());
//                    }
//
//                    RowList.push_back(result_get());
//                }
//
//                //if empty row, ignore it
//                if(iCurCol != 0)
//                {
//                    if(iCols == -1)
//                    {
//                        iCols = iCurCol;
//                        if(iCols == 0)
//                        {//manage []
//                            iCols = -1;
//                        }
//                    }
//                    else if(iCols != 0 && iCols != iCurCol)
//                    {
//                        std::wostringstream os;
//                        os << L"inconsistent row/column dimensions\n";
//                        //os << ((Location)(*row)->location_get()).location_getString() << std::endl;
//                        throw ScilabError(os.str(), 999, (*row)->location_get());
//                    }
//
//                    iRows += iCurRow;
//                    if(iRows == -1)
//                    {//manage  []
//                        iRows = 0;
//                    }
//                    iCurCol = 0;
//                    iCurRow = -1;
//                    MatrixList.push_back(RowList);
//                }
//            }
//
//            list<list<InternalType*> >::const_iterator it_ML;
//            list<InternalType*>::const_iterator it_RL;
//
//            int iAddRow = 0;
//            iCurRow			= 0;
//            iCurCol			= 0;
//            for(it_ML = MatrixList.begin() ; it_ML != MatrixList.end() ; it_ML++)
//            {
//                int iAddCol = 0;
//                for(it_RL = (*it_ML).begin() ; it_RL != (*it_ML).end() ; it_RL++)
//                {
//                    if((*it_RL)->isDouble() && (*it_RL)->getAs<Double>()->isEmpty())
//                    {//manage []
//                        continue;
//                    }
//
//                    if(poResult == NULL)
//                    {
//                        poResult = AddElementToVariable(poResult, *it_RL, iRows, iCols, &iAddRow, &iAddCol);
//                    }
//                    else
//                    {
//                        poResult = AddElementToVariable(poResult, *it_RL, iCurRow, iCurCol, &iAddRow, &iAddCol);
//                    }
//                    
//                    if((*it_RL)->isDeletable() == true)
//                    {
//                        delete (*it_RL);
//                    }
//
//                    iCurCol += iAddCol;
//                }
//                iCurRow += iAddRow;
//                iCurCol = 0;
//            }
//        }
//
//        if (poResult == NULL)
//        {
//            poResult = Double::Empty();
//        }
//
//        result_set(poResult);
    }
    catch(ScilabError error)
    {
        throw error;
    }
}
