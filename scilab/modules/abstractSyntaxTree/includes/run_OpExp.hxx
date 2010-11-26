/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
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

void visitprivate(const OpExp &e)
{
    T execMeL;
    T execMeR;

    try
    {

        /*getting what to assign*/
        e.left_get().accept(execMeL);
        /*getting what to assign*/
        e.right_get().accept(execMeR);
        GenericType::RealType TypeL = execMeL.result_get()->getType();
        GenericType::RealType TypeR = execMeR.result_get()->getType();

        if(TypeL == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = execMeL.result_get()->getAsImplicitList();
            if(pIL->computable())
            {
                InternalType *pIT = pIL->extract_matrix();
                execMeL.result_set(pIT);
                TypeL = pIT->getType();
            }
        }

        if(TypeR == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = execMeR.result_get()->getAsImplicitList();
            if(pIL->computable())
            {
                InternalType *pIT = pIL->extract_matrix();
                execMeR.result_set(pIT);
                TypeR = pIT->getType();
            }
        }

        InternalType *pResult = NULL;
        switch(e.oper_get())
        {
        case OpExp::plus :
            {
                if((TypeR == GenericType::RealDouble || TypeR == GenericType::RealSparse)
                   && (TypeL == GenericType::RealDouble)
                   || (TypeL == GenericType::RealSparse))
                {
                    int iResult = 0;

                    // FIXME : Addiction between double and sparse should be reflexive ???

                    // Left operand is double
                    if( TypeL == GenericType::RealDouble )
                    {
                        if( TypeR == GenericType::RealDouble )
                        {
                            iResult=  AddDoubleToDouble( execMeL.result_get()->getAsDouble(),  execMeR.result_get()->getAsDouble(), (Double**)&pResult);
                        }
                        else
                        {
                            iResult= AddDoubleToSparse( execMeL.result_get()->getAsDouble(),  execMeR.result_get()->getAsSparse(), (GenericType**)&pResult);
                        }
                    }
                    // Left operand is Sparse
                    else
                    {
                        if(TypeR == GenericType::RealDouble)
                        {
                            iResult= AddSparseToDouble( execMeL.result_get()->getAsSparse(),  execMeR.result_get()->getAsDouble(), (GenericType**)&pResult);
                        }
                        else
                        {
                            iResult= AddSparseToSparse( execMeL.result_get()->getAsSparse(),  execMeR.result_get()->getAsSparse(), (GenericType**)&pResult);
                        }
                    }


                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << L"inconsistent row/column dimensions.\n";
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
                {
                    //nothing to do, all in macro : %b_+_b
                }
                else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
                {
                    String *pL = execMeL.result_get()->getAsString();
                    String *pR = execMeR.result_get()->getAsString();

                    int iResult = AddStringToString(pL, pR, (String**)&pResult);

                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << L"inconsistent row/column dimensions\n";
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealInt && TypeR == GenericType::RealInt)
                {
                }
                else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
                {
                    Double *pL				= execMeL.result_get()->getAsDouble();
                    MatrixPoly *pR		= execMeR.result_get()->getAsPoly();

                    int iResult = AddDoubleToPoly(pR, pL, (MatrixPoly**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << L"inconsistent row/column dimensions\n";
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
                {
                    Double *pR				= execMeR.result_get()->getAsDouble();
                    MatrixPoly *pL		= execMeL.result_get()->getAsPoly();

                    int iResult = AddDoubleToPoly(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << L"inconsistent row/column dimensions\n";
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
                {
                    MatrixPoly *pL	= execMeL.result_get()->getAsPoly();
                    MatrixPoly *pR	= execMeR.result_get()->getAsPoly();

                    int iResult = AddPolyToPoly(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult != 0)
                    {
                        if(iResult == 1)
                        {
                            std::wostringstream os;
                            os << L"inconsistent row/column dimensions\n";
                            //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                            throw ScilabError(os.str(), 999, e.right_get().location_get());
                        }
                        else if(iResult == 2)
                        {
                            std::wostringstream os;
                            os << L"variables don't have the same formal variable";
                            //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                            throw ScilabError(os.str(), 999, e.right_get().location_get());
                        }
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealString)
                {
                    if(execMeL.result_get()->getAsDouble()->size_get() == 0)
                    {//[] + "" -> ""
                        result_set(execMeR.result_get()->clone());
                    }
                    else
                    {
                        // Don't know how to manage this Addition : Call Overloading
                        result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                    }
                }
                else if(TypeL == GenericType::RealString && TypeR == GenericType::RealDouble)
                {
                    if(execMeR.result_get()->getAsDouble()->size_get() == 0)
                    {//"text" + [] -> ""
                        result_set(execMeL.result_get()->clone());
                    }
                    else
                    {
                        // Don't know how to manage this Addition : Call Overloading
                        result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                    }
                }
                else
                {
                    // Don't know how to manage this Addition : Call Overloading
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::minus :
            {
                if((TypeR == GenericType::RealDouble || TypeR == GenericType::RealSparse)
                   && (TypeL == GenericType::RealDouble)
                   ||(TypeL == GenericType::RealSparse))
                {
                    int iResult = 0;
                    if(TypeL == GenericType::RealDouble)
                    {
                        if(TypeR == GenericType::RealDouble)
                        {
                            iResult= SubstractDoubleToDouble( execMeL.result_get()->getAsDouble(),  execMeR.result_get()->getAsDouble(), (Double**)(&pResult));
                        }
                        else
                        {
                            iResult= SubstractDoubleToSparse( execMeL.result_get()->getAsDouble(),  execMeR.result_get()->getAsSparse(), (GenericType**)(&pResult));
                        }
                    }
                    else
                    {
                        if(TypeR == GenericType::RealDouble)
                        {
                            iResult= SubstractSparseToDouble( execMeL.result_get()->getAsSparse(),  execMeR.result_get()->getAsDouble(), (GenericType**)(&pResult));
                        }
                        else
                        {
                            iResult= SubstractSparseToSparse( execMeL.result_get()->getAsSparse(),  execMeR.result_get()->getAsSparse(), (GenericType**)(&pResult));
                        }
                    }
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
                {
                    Double *pL					= execMeL.result_get()->getAsDouble();
                    MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

                    int iResult = SubstractPolyToDouble(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
                {
                    MatrixPoly *pL			= execMeL.result_get()->getAsPoly();
                    Double *pR					= execMeR.result_get()->getAsDouble();

                    int iResult = SubstractDoubleToPoly(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
                {
                    MatrixPoly *pL			= execMeL.result_get()->getAsPoly();
                    MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

                    int iResult = SubstractPolyToPoly(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }

                break;
            }
        case OpExp::times:
            {
                if((TypeR == GenericType::RealDouble || TypeR == GenericType::RealSparse)
                   && (TypeL == GenericType::RealDouble)
                   ||(TypeL == GenericType::RealSparse))
                {
                    int iResult = 0;
                    if(TypeL == GenericType::RealDouble)
                    {
                        if(TypeR == GenericType::RealDouble)
                        {
                            iResult= MultiplyDoubleByDouble( execMeL.result_get()->getAsDouble(),  execMeR.result_get()->getAsDouble(), (Double**)(&pResult));
                        }
                        else
                        {
                            iResult= MultiplyDoubleBySparse( execMeL.result_get()->getAsDouble(),  execMeR.result_get()->getAsSparse(), (GenericType**)(&pResult));
                        }
                    }
                    else
                    {
                        if(TypeR == GenericType::RealDouble)
                        {
                            iResult= MultiplySparseByDouble( execMeL.result_get()->getAsSparse(),  execMeR.result_get()->getAsDouble(), (GenericType**)(&pResult));
                        }
                        else
                        {
                            iResult= MultiplySparseBySparse( execMeL.result_get()->getAsSparse(),  execMeR.result_get()->getAsSparse(), (Sparse**)(&pResult));
                        }
                    }
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else if(TypeL == InternalType::RealDouble && TypeR == InternalType::RealPoly)
                {
                    Double *pL			    = execMeL.result_get()->getAsDouble();
                    MatrixPoly *pR	    = execMeR.result_get()->getAsPoly();

                    int iResult = MultiplyDoubleByPoly(pL, pR, (MatrixPoly**)&pResult);

                    if(iResult)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealDouble)
                {
                    MatrixPoly *pL	    = execMeL.result_get()->getAsPoly();
                    Double *pR			    = execMeR.result_get()->getAsDouble();

                    int iResult = MultiplyPolyByDouble(pL, pR, (MatrixPoly**)&pResult);

                    if(iResult)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealPoly)
                {
                    MatrixPoly *pL	    = execMeL.result_get()->getAsPoly();
                    MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

                    int iResult = MultiplyPolyByPoly(pL, pR, (MatrixPoly**)&pResult);

                    if(iResult)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::rdivide:
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    int iResult = DivideDoubleByDouble(pL, pR, (Double**)&pResult);
                    if(iResult)
                    {//manage errors
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
                {
                    MatrixPoly *pL	= execMeL.result_get()->getAsPoly();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    int iResult = DividePolyByDouble(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult)
                    {//manage errors
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    MatrixPoly *pR	= execMeR.result_get()->getAsPoly();

                    int iResult = DivideDoubleByPoly(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult)
                    {//manage errors
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::dottimes :
            {

                if((TypeR == GenericType::RealDouble || TypeR == GenericType::RealSparse)
                   && (TypeL == GenericType::RealDouble)
                   || (TypeL == GenericType::RealSparse))
                {
                    int iResult = 0;
                    if( TypeL == GenericType::RealDouble )
                    {
                        if( TypeR == GenericType::RealDouble )
                        {
                            iResult=  DotMultiplyDoubleByDouble( execMeL.result_get()->getAsDouble(), execMeR.result_get()->getAsDouble(), (Double**)&pResult);
                        }
                        else
                        {
                            iResult= DotMultiplyDoubleBySparse( execMeL.result_get()->getAsDouble(), execMeR.result_get()->getAsSparse(), (GenericType**)&pResult);
                        }
                    }
                    else
                    {
                        if(TypeR == GenericType::RealDouble)
                        {
                            iResult= DotMultiplySparseByDouble( execMeL.result_get()->getAsSparse(),  execMeR.result_get()->getAsDouble(), (GenericType**)&pResult);
                        }
                        else
                        {
                            iResult= DotMultiplySparseBySparse( execMeL.result_get()->getAsSparse(),  execMeR.result_get()->getAsSparse(), (Sparse**)&pResult);
                        }
                    }
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::eq :
            {
                if( (TypeL == GenericType::RealDouble && execMeL.result_get()->getAsDouble()->size_get() == 0)
                    || (TypeL == GenericType::RealSparse && execMeL.result_get()->getAsSparse()->size_get() == 0))
                {//[] == xx
                    if(TypeR != TypeL)
                    {
                        result_set(new Bool(false));
                        return;
                    }
                }

                if( (TypeR == GenericType::RealDouble && execMeR.result_get()->getAsDouble()->size_get() == 0)
                    || (TypeR == GenericType::RealSparse && execMeR.result_get()->getAsSparse()->size_get() == 0))
                {//xx == []
                    if(TypeL != TypeR)
                    {
                        result_set(new Bool(false));
                        return;
                    }
                }

                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    if(pR->size_get() == 0 && pL->size_get() == 0)
                    {
                        pResult = new Bool(true);
                    }
                    else if(pL->size_get() == 0  || pR->size_get() == 0)
                    {
                        pResult = new Bool(false);
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->real_get(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) == dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->real_get(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef == pR->real_get(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) == pR->real_get(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
                {
                    String *pL			= execMeL.result_get()->getAsString();
                    String *pR			= execMeR.result_get()->getAsString();

                    if(pL->size_get() == 1)
                    {
                        pResult = new Bool(pR->rows_get(), pR->cols_get());

                        wchar_t* pstL = pL->string_get()[0];
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                wchar_t* pstR = pR->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        wchar_t* pstR = pR->string_get()[0];
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                wchar_t* pstL = pL->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                wchar_t* pstR = pR->string_get(i,j);
                                wchar_t* pstL = pL->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
                {
                    Bool *pL			= execMeL.result_get()->getAsBool();
                    Bool *pR			= execMeR.result_get()->getAsBool();

                    if(pL->size_get() == 1)
                    {
                        pResult = new Bool(pR->rows_get(), pR->cols_get());

                        int iL = pL->bool_get()[0];
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                if(iL == pR->bool_get(i,j))
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        int iR = pR->bool_get()[0];
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                if(iR == pL->bool_get(i,j))
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                if(pL->bool_get(i,j) == pR->bool_get(i,j))
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
                {
                    result_set(execMeL.result_get()->getAsSparse()->newEqualTo(*(execMeR.result_get()->getAsSparse())));
                }
                else if(TypeL == GenericType::RealSparseBool && TypeR == GenericType::RealSparseBool)
                {
                    result_set(execMeL.result_get()->getAsSparseBool()->newEqualTo(*(execMeR.result_get()->getAsSparseBool())));
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::ne :
            {
                if(TypeL == GenericType::RealDouble && execMeL.result_get()->getAsDouble()->size_get() == 0)
                {//[] <> xx
                    if(TypeR != InternalType::RealDouble)
                    {
                        result_set(new Bool(true));
                        return;
                    }
                }

                if(TypeR == GenericType::RealDouble && execMeR.result_get()->getAsDouble()->size_get() == 0)
                {//xx <> []
                    if(TypeL != InternalType::RealDouble)
                    {
                        result_set(new Bool(true));
                        return;
                    }
                }

                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    if(pR->size_get() == 0 && pL->size_get() == 0)
                    {
                        pResult = new Bool(false);
                    }
                    else if(pL->size_get() == 0  || pR->size_get() == 0)
                    {
                        pResult = new Bool(true);
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->real_get(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) != dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->real_get(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef != pR->real_get(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) != pR->real_get(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
                {
                    String *pL			= execMeL.result_get()->getAsString();
                    String *pR			= execMeR.result_get()->getAsString();

                    if(pL->size_get() == 1)
                    {
                        pResult = new Bool(pR->rows_get(), pR->cols_get());

                        wchar_t* pstL = pL->string_get()[0];
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                wchar_t* pstR = pR->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                            }
                        }
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        wchar_t* pstR = pR->string_get()[0];
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                wchar_t* pstL = pL->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                            }
                        }
                    }
                    else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                wchar_t* pstR = pR->string_get(i,j);
                                wchar_t* pstL = pL->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                            }
                        }
                    }
                    else if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
                    {
                        result_set(execMeL.result_get()->getAsSparse()->newNotEqualTo(*(execMeR.result_get()->getAsSparse())));
                    }
                    else if(TypeL == GenericType::RealSparseBool && TypeR == GenericType::RealSparseBool)
                    {
                        result_set(execMeL.result_get()->getAsSparseBool()->newNotEqualTo(*(execMeR.result_get()->getAsSparseBool())));
                    }
                    else
                    {
                        pResult = new Bool(true);
                    }
                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::lt :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->real_get(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) < dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->real_get(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef < pR->real_get(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) < pR->real_get(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
                {
                    result_set(execMeL.result_get()->getAsSparse()->newLessThan(*(execMeR.result_get()->getAsSparse())));
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::le :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->real_get(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) <= dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->real_get(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef <= pR->real_get(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) <= pR->real_get(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
                {
                    result_set(execMeL.result_get()->getAsSparse()->newLessOrEqual(*(execMeR.result_get()->getAsSparse())));
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::gt :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->real_get(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) > dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->real_get(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef > pR->real_get(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());

                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) > pR->real_get(i, j));
                            }
                        }
                    }
                    else if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
                    {
                        result_set(execMeL.result_get()->getAsSparse()->newGreaterThan(*(execMeR.result_get()->getAsSparse())));
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::ge :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->real_get(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) >= dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->real_get(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef >= pR->real_get(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) >= pR->real_get(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
                {
                    result_set(execMeL.result_get()->getAsSparse()->newGreaterOrEqual(*(execMeR.result_get()->getAsSparse())));
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::power :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= execMeL.result_get()->getAsDouble();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    int iResult = PowerDoubleByDouble(pL, pR, (Double**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
                {
                    MatrixPoly *pL	= execMeL.result_get()->getAsPoly();
                    Double *pR			= execMeR.result_get()->getAsDouble();

                    int iResult = PowerPolyByDouble(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        default :
            // By default call overload if we do not know this operator ...
            result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            break;
        }
    }
    catch(ScilabError error)
    {
        error.SetErrorLocation(e.location_get());
        throw error;
    }
}

void visitprivate(const LogicalOpExp &e)
{
    T execMeL;
    T execMeR;

    e.left_get().accept(execMeL);
    GenericType::RealType TypeL = execMeL.result_get()->getType();
    Bool *pL	= NULL;

    if(TypeL == GenericType::RealBool)
    {
        pL	= execMeL.result_get()->getAsBool();
    }
    else
    {
        //TODO YaSp : Overloading %*_oper_*
        e.right_get().accept(execMeR);
        GenericType::RealType TypeR = execMeR.result_get()->getType();
        switch(e.oper_get())
        {
        case LogicalOpExp::logicalShortCutOr :
        case LogicalOpExp::logicalOr :
            if(TypeL == GenericType::RealSparseBool && TypeR == GenericType::RealSparseBool)
            {
                result_set(execMeL.result_get()->getAsSparseBool()->newLogicalOr(*(execMeR.result_get()->getAsSparseBool())));
            }
            else
            {
                result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            }
            break;
        case LogicalOpExp::logicalShortCutAnd :
        case LogicalOpExp::logicalAnd :
            if(TypeL == GenericType::RealSparseBool && TypeR == GenericType::RealSparseBool)
            {
                result_set(execMeL.result_get()->getAsSparseBool()->newLogicalAnd(*(execMeR.result_get()->getAsSparseBool())));
            }
            else
            {
                result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            }
            break;
        }
        return;
    }

    InternalType *pResult = NULL;
    switch(e.oper_get())
    {
    case LogicalOpExp::logicalShortCutOr :
        {
            Bool *pL	= execMeL.result_get()->getAsBool();
            int *piL	= pL->bool_get();
            bool iL     = true;
            // Check if all values are true
            // true || <something is always true>
            for(int i = 0 ; i < pL->size_get() ; i++)
            {
                if(piL[i] == false)
                {
                    iL = false;
                    break;
                }
            }

            if(iL)
            {//we don't need to look at ohers exp
                pResult = new Bool(true);
                result_set(pResult);
                return;
            }
            // DO NOT break here, falling into normal Or if this can not be shotcutted.
        }
    case LogicalOpExp::logicalOr :
        {
            e.right_get().accept(execMeR);
            GenericType::RealType TypeR = execMeR.result_get()->getType();

            if(TypeR == GenericType::RealBool)
            {
                Bool *pR	= execMeR.result_get()->getAsBool();
                int* piR = pR->bool_get();
                int* piL = pL->bool_get();
                int* piB	= NULL;

                // M | scalar
                if(pR->size_get() == 1)
                {
                    pResult = new Bool(pL->rows_get(), pL->cols_get(), &piB);
                    for(int i = 0 ; i < pL->size_get(); i++)
                    {
                        piB[i] = (piR[0] == 1) || (piL[i] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // scalar | M
                if(pL->size_get() == 1)
                {
                    pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                    for(int i = 0 ; i < pR->size_get(); i++)
                    {
                        piB[i] = (piR[i] == 1) || (piL[0] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // M | N (generic case)
                if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                {
                    int* piB	= NULL;
                    int* piR	= pR->bool_get();
                    int* piL	= pL->bool_get();

                    pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                    for(int i = 0 ; i < pL->size_get(); i++)
                    {
                        piB[i] = (piR[i] == 1) || (piL[i] == 1);
                    }
                    result_set(pResult);
                }
                else
                {
                    std::wostringstream os;
                    os << _W("Inconsistent row/column dimensions.\n");
                    //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                    throw ScilabError(os.str(), 999, e.right_get().location_get());
                }
            }
            else
            {
                result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            }
            break;
        }
    case LogicalOpExp::logicalShortCutAnd :
        {
            Bool *pL	= execMeL.result_get()->getAsBool();
            int *piL	= pL->bool_get();
            // Check if one value is false
            // false && <something> is always false
            for(int i = 0 ; i < pL->size_get() ; i++)
            {
                if(piL[i] == false)
                {
                    pResult = new Bool(false);
                    result_set(pResult);
                    return;
                }
            }
            // DO NOT break here, falling into normal And if this can not be shotcutted.
        }
    case LogicalOpExp::logicalAnd :
        {
            e.right_get().accept(execMeR);
            GenericType::RealType TypeR = execMeR.result_get()->getType();

            if(TypeR == GenericType::RealBool)
            {
                Bool *pR	= execMeR.result_get()->getAsBool();
                int* piR = pR->bool_get();
                int* piL = pL->bool_get();
                int* piB	= NULL;

                // M & scalar
                if(pR->size_get() == 1)
                {
                    pResult = new Bool(pL->rows_get(), pL->cols_get(), &piB);
                    for(int i = 0 ; i < pL->size_get(); i++)
                    {
                        piB[i] = (piR[0] == 1) && (piL[i] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // scalar & M
                if(pL->size_get() == 1)
                {
                    pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                    for(int i = 0 ; i < pR->size_get(); i++)
                    {
                        piB[i] = (piR[i] == 1) && (piL[0] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // M & N (generic case)
                if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                {
                    int* piB	= NULL;
                    int* piR	= pR->bool_get();
                    int* piL	= pL->bool_get();

                    pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                    for(int i = 0 ; i < pL->size_get(); i++)
                    {
                        piB[i] = (piR[i] == 1) && (piL[i] == 1);
                    }
                    result_set(pResult);
                }
                else
                {
                    std::wostringstream os;
                    os << _W("Inconsistent row/column dimensions.\n");
                    //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                    throw ScilabError(os.str(), 999, e.right_get().location_get());
                }
            }
            else
            {
                result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            }
            break;
        }
    }
}

InternalType* callOverload(OpExp::Oper _oper, T *_paramL, T *_paramR)
{
    T execMe;
    types::typed_list in;
    types::typed_list out;
    _paramL->result_get()->IncreaseRef();
    _paramR->result_get()->IncreaseRef();
    in.push_back(_paramL->result_get());
    in.push_back(_paramR->result_get());

    Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, &execMe);
    if(out[0] != NULL)
    {
        out[0]->IncreaseRef();
    }
    _paramL->result_get()->DecreaseRef();
    _paramR->result_get()->DecreaseRef();
    return out[0];
}
