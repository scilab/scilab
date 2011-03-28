/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

        InternalType *pITL          = execMeL.result_get();
        InternalType *pITR          = execMeR.result_get();
        GenericType::RealType TypeL = pITL->getType();
        GenericType::RealType TypeR = pITR->getType();

        if(TypeL == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = pITL->getAsImplicitList();
            if(pIL->isComputable())
            {
                pITL = pIL->extractFullMatrix();
                TypeL = pITL->getType();
            }
        }

        if(TypeR == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = pITR->getAsImplicitList();
            if(pIL->isComputable())
            {
                pITR = pIL->extractFullMatrix();
                TypeR = pITR->getType();
            }
        }

        InternalType *pResult   = NULL;

        switch(e.oper_get())
        {
        case OpExp::plus :
            {
                try
                {
                    pResult = GenericPlus(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), &execMeL, &execMeR);

                }

                result_set(pResult);
                break;
            }
        case OpExp::minus :
            {
                try
                {
                    pResult = GenericMinus(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), &execMeL, &execMeR);

                }

                result_set(pResult);
                break;
            }
        case OpExp::times:
            {
                try
                {
                    pResult = GenericTimes(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), &execMeL, &execMeR);

                }
                result_set(pResult);
                break;
            }
        case OpExp::rdivide:
            {
                try
                {
                    pResult = GenericRDivide(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), &execMeL, &execMeR);

                }
                result_set(pResult);
                break;
            }
        case OpExp::dottimes :
            {
                try
                {
                    pResult = GenericDotTimes(execMeL.result_get(), execMeR.result_get());
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), &execMeL, &execMeR);

                }
                result_set(pResult);
                break;
            }
        case OpExp::eq :
            {
                if(TypeL == GenericType::RealDouble && pITL->getAs<Double>()->getSize() == 0)
                {//[] == xx
                    if(TypeR != InternalType::RealDouble)
                    {
                        result_set(new Bool(false));
                        return;
                    }
                }

                if(TypeR == GenericType::RealDouble && pITR->getAs<Double>()->getSize() == 0)
                {//xx == []
                    if(TypeL != InternalType::RealDouble)
                    {
                        result_set(new Bool(false));
                        return;
                    }
                }

                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 0 && pL->getSize() == 0)
                    {
                        pResult = new Bool(true);
                    }
                    else if(pL->getSize() == 0  || pR->getSize() == 0)
                    {
                        pResult = new Bool(false);
                    }
                    else if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) == dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef == pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) == pR->getReal(i, j));
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
                    String *pL			= pITL->getAs<types::String>();
                    String *pR			= pITR->getAs<types::String>();

                    if(pL->getSize() == 1)
                    {
                        pResult = new Bool(pR->getRows(), pR->getCols());

                        wchar_t* pstL = pL->get()[0];
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                wchar_t* pstR = pR->get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pR->getSize() == 1)
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols());

                        wchar_t* pstR = pR->get()[0];
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                wchar_t* pstL = pL->get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols());

                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                wchar_t* pstR = pR->get(i,j);
                                wchar_t* pstL = pL->get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
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
                    Bool *pL			= pITL->getAs<types::Bool>();
                    Bool *pR			= pITR->getAs<types::Bool>();

                    if(pL->getSize() == 1)
                    {
                        pResult = new Bool(pR->getRows(), pR->getCols());

                        int iL = pL->get()[0];
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                if(iL == pR->get(i,j))
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pR->getSize() == 1)
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols());

                        int iR = pR->get()[0];
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                if(iR == pL->get(i,j))
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols());

                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                if(pL->get(i,j) == pR->get(i,j))
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
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
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::ne :
            {
                if(TypeL == GenericType::RealDouble && pITL->getAs<Double>()->getSize() == 0)
                {//[] <> xx
                    if(TypeR != InternalType::RealDouble)
                    {
                        result_set(new Bool(true));
                        return;
                    }
                }

                if(TypeR == GenericType::RealDouble && pITR->getAs<Double>()->getSize() == 0)
                {//xx <> []
                    if(TypeL != InternalType::RealDouble)
                    {
                        result_set(new Bool(true));
                        return;
                    }
                }

                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 0 && pL->getSize() == 0)
                    {
                        pResult = new Bool(false);
                    }
                    else if(pL->getSize() == 0  || pR->getSize() == 0)
                    {
                        pResult = new Bool(true);
                    }
                    else if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) != dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef != pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) != pR->getReal(i, j));
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
                    String *pL			= pITL->getAs<types::String>();
                    String *pR			= pITR->getAs<types::String>();

                    if(pL->getSize() == 1)
                    {
                        pResult = new Bool(pR->getRows(), pR->getCols());

                        wchar_t* pstL = pL->get()[0];
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                wchar_t* pstR = pR->get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                            }
                        }
                    }
                    else if(pR->getSize() == 1)
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols());

                        wchar_t* pstR = pR->get()[0];
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                wchar_t* pstL = pL->get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                            }
                        }
                    }
                    else if(pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols());

                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                wchar_t* pstR = pR->get(i,j);
                                wchar_t* pstL = pL->get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAs<types::Bool>()->set(i,j,true);
                                }
                            }
                        }
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
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) < dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef < pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) < pR->getReal(i, j));
                            }
                        }
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
        case OpExp::le :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) <= dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef <= pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) <= pR->getReal(i, j));
                            }
                        }
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
        case OpExp::gt :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) > dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef > pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());

                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) > pR->getReal(i, j));
                            }
                        }
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
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) >= dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef >= pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) >= pR->getReal(i, j));
                            }
                        }
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
        case OpExp::power :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    int iResult = PowerDoubleByDouble(pL, pR, (Double**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
                {
                    Polynom *pL	= pITL->getAs<types::Polynom>();
                    Double *pR			= pITR->getAs<Double>();

                    int iResult = PowerPolyByDouble(pL, pR, (Polynom**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
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
    InternalType *pITL          = execMeL.result_get();
    InternalType *pITR          = NULL;

    GenericType::RealType TypeL = pITL->getType();
    Bool *pL	= NULL;

    if(TypeL == GenericType::RealBool)
    {
        pL	= pITL->getAs<types::Bool>();
    }
    else
    {
        //TODO YaSp : Overloading %*_oper_*
        e.right_get().accept(execMeR);
        switch(e.oper_get())
        {
        case LogicalOpExp::logicalShortCutOr :
        case LogicalOpExp::logicalOr :
            result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            break;
        case LogicalOpExp::logicalShortCutAnd :
        case LogicalOpExp::logicalAnd :
            result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            break;
        default :
            break;
        }
        return;
    }

    InternalType *pResult = NULL;
    switch(e.oper_get())
    {
    case LogicalOpExp::logicalShortCutOr :
        {
            Bool *pB	= pITL->getAs<types::Bool>();
            int *piL	= pB->get();
            bool iL     = true;
            // Check if all values are true
            // true || <something is always true>
            for(int i = 0 ; i < pB->getSize() ; i++)
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
            pITR = execMeR.result_get();
            GenericType::RealType TypeR = pITR->getType();

            if(TypeR == GenericType::RealBool)
            {
                Bool *pR	= pITR->getAs<types::Bool>();
                int* piR = pR->get();
                int* piL = pL->get();
                int* piB	= NULL;

                // M | scalar
                if(pR->getSize() == 1)
                {
                    pResult = new Bool(pL->getRows(), pL->getCols(), &piB);
                    for(int i = 0 ; i < pL->getSize(); i++)
                    {
                        piB[i] = (piR[0] == 1) || (piL[i] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // scalar | M
                if(pL->getSize() == 1)
                {
                    pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                    for(int i = 0 ; i < pR->getSize(); i++)
                    {
                        piB[i] = (piR[i] == 1) || (piL[0] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // M | N (generic case)
                if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                {
                    pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                    for(int i = 0 ; i < pL->getSize(); i++)
                    {
                        piB[i] = (piR[i] == 1) || (piL[i] == 1);
                    }
                    result_set(pResult);
                }
                else
                {
                    std::wostringstream os;
                    os << _W("Inconsistent row/column dimensions.\n");
                    //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
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
            Bool *pB	= pITL->getAs<types::Bool>();
            int *piL	= pB->get();
            // Check if one value is false
            // false && <something> is always false
            for(int i = 0 ; i < pB->getSize() ; i++)
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
            pITR = execMeR.result_get();
            GenericType::RealType TypeR = pITR->getType();

            if(TypeR == GenericType::RealBool)
            {
                Bool *pR	= pITR->getAs<types::Bool>();
                int* piR = pR->get();
                int* piL = pL->get();
                int* piB	= NULL;

                // M & scalar
                if(pR->getSize() == 1)
                {
                    pResult = new Bool(pL->getRows(), pL->getCols(), &piB);
                    for(int i = 0 ; i < pL->getSize(); i++)
                    {
                        piB[i] = (piR[0] == 1) && (piL[i] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // scalar & M
                if(pL->getSize() == 1)
                {
                    pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                    for(int i = 0 ; i < pR->getSize(); i++)
                    {
                        piB[i] = (piR[i] == 1) && (piL[0] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // M & N (generic case)
                if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                {
                    pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                    for(int i = 0 ; i < pL->getSize(); i++)
                    {
                        piB[i] = (piR[i] == 1) && (piL[i] == 1);
                    }
                    result_set(pResult);
                }
                else
                {
                    std::wostringstream os;
                    os << _W("Inconsistent row/column dimensions.\n");
                    //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, e.right_get().location_get());
                }
            }
            else
            {
                result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            }
            break;
        }

    default :
        {
            result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
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
