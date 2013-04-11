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
    try
    {
        /*getting what to assign*/
        e.left_get().accept(*this);
        InternalType *pITL = result_get();
        if (is_single_result() == false)
        {
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
            throw ScilabError(os.str(), 999, e.right_get().location_get());
        }

        /*getting what to assign*/
        e.right_get().accept(*this);
        InternalType *pITR = result_get();
        if (is_single_result() == false)
        {
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
            throw ScilabError(os.str(), 999, e.right_get().location_get());
        }

        if (pITL->getType() == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = pITL->getAs<ImplicitList>();
            if (pIL->isComputable())
            {
                pITL = pIL->extractFullMatrix();
            }
        }

        if (pITR->getType() == GenericType::RealImplicitList)
        {
            ImplicitList* pIR = pITR->getAs<ImplicitList>();
            if (pIR->isComputable())
            {
                pITR = pIR->extractFullMatrix();
            }
        }

        InternalType *pResult   = NULL;

        switch (e.oper_get())
        {
            case OpExp::plus :
            {
                pResult = GenericPlus(pITL, pITR);
                break;
            }
            case OpExp::unaryMinus :
            {
                pResult = GenericUnaryMinus(pITR);
                break;
            }
            case OpExp::minus :
            {
                pResult = GenericMinus(pITL, pITR);
                break;
            }
            case OpExp::times:
            {
                pResult = GenericTimes(pITL, pITR);
                break;
            }
            case OpExp::ldivide:
            {
                pResult = GenericLDivide(pITL, pITR);
                break;
            }
            case OpExp::dotldivide :
            {
                pResult = GenericDotLDivide(pITL, pITR);
                break;
            }
            case OpExp::rdivide:
            {
                pResult = GenericRDivide(pITL, pITR);
                break;
            }
            case OpExp::dotrdivide :
            {
                pResult = GenericDotRDivide(pITL, pITR);
                break;
            }
            case OpExp::dottimes :
            {
                pResult = GenericDotTimes(pITL, pITR);
                break;
            }
            case OpExp::dotpower :
            {
                pResult = GenericDotPower(pITL, pITR);
                break;
            }
            case OpExp::eq :
            {
                pResult = GenericComparisonEqual(pITL, pITR);
                break;
            }
            case OpExp::ne :
            {
                pResult = GenericComparisonNonEqual(pITL, pITR);
                break;
            }
            case OpExp::lt :
            {
                pResult = GenericLess(pITL, pITR);
                break;
            }
            case OpExp::le :
            {
                pResult = GenericLessEqual(pITL, pITR);
                break;
            }
            case OpExp::gt :
            {
                pResult = GenericGreater(pITL, pITR);
                break;
            }
            case OpExp::ge :
            {
                pResult = GenericGreaterEqual(pITL, pITR);
                break;
            }
            case OpExp::power :
            {
                pResult = GenericPower(pITL, pITR);
                break;
            }
            case OpExp::krontimes :
            {
                pResult = GenericKrontimes(pITL, pITR);
                break;
            }
            case OpExp::kronrdivide :
            {
                pResult = GenericKronrdivide(pITL, pITR);
                break;
            }
            case OpExp::kronldivide :
            {
                pResult = GenericKronldivide(pITL, pITR);
                break;
            }
            default :
                break;
        }

        //overloading
        if (pResult == NULL)
        {
            // We did not have any algorithm matching, so we try to call OverLoad
            pResult = callOverload(e.oper_get(), pITL, pITR);
        }

        result_set(pResult);

        //clear left and/or right operands
        if (pITL->isDeletable())
        {
            delete pITL;
        }

        if (pITR->isDeletable())
        {
            delete pITR;
        }
    }
    catch (ScilabError error)
    {
        result_clear();
        error.SetErrorLocation(e.location_get());
        throw error;
    }

}

void visitprivate(const LogicalOpExp &e)
{
    e.left_get().accept(*this);
    InternalType *pITL = result_get();
    InternalType *pITR = NULL;

    GenericType::RealType TypeL = pITL->getType();

    InternalType *pResult = NULL;

    if (TypeL == GenericType::RealBool)
    {
        Bool *pL = pITL->getAs<types::Bool>();
        switch (e.oper_get())
        {
            case LogicalOpExp::logicalShortCutOr :
            {
                int *piL	= pL->get();
                bool iL     = true;
                // Check if all values are true
                // true || <something is always true>
                for (int i = 0 ; i < pL->getSize() ; i++)
                {
                    if (piL[i] == false)
                    {
                        iL = false;
                        break;
                    }
                }

                if (iL)
                {
                    //we don't need to look at ohers exp
                    result_set(new Bool(true));
                    return;
                }
                // DO NOT break here, falling into normal Or if this can not be shotcutted.
            }
            case LogicalOpExp::logicalOr :
            {
                e.right_get().accept(*this);
                pITR = result_get();
                GenericType::RealType TypeR = pITR->getType();

                if (TypeR == GenericType::RealBool)
                {
                    Bool *pR = pITR->getAs<types::Bool>();
                    int* piR = pR->get();
                    int* piL = pL->get();
                    int* piB = NULL;

                    // M | scalar
                    if (pR->getSize() == 1)
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols(), &piB);
                        for (int i = 0 ; i < pL->getSize(); i++)
                        {
                            piB[i] = (piR[0] == 1) || (piL[i] == 1);
                        }
                    }
                    else if (pL->getSize() == 1)
                    {
                        // scalar | M
                        pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                        for (int i = 0 ; i < pR->getSize(); i++)
                        {
                            piB[i] = (piR[i] == 1) || (piL[0] == 1);
                        }
                    }
                    else if (pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        // M | N (generic case)
                        pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                        for (int i = 0 ; i < pL->getSize(); i++)
                        {
                            piB[i] = (piR[i] == 1) || (piL[i] == 1);
                        }
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
                    pResult = callOverload(e.oper_get(), pITL, pITR);
                }
                break;
            }
            case LogicalOpExp::logicalShortCutAnd :
            {
                int *piL	= pL->get();
                // Check if one value is false
                // false && <something> is always false
                for (int i = 0 ; i < pL->getSize() ; i++)
                {
                    if (piL[i] == false)
                    {
                        result_set(new Bool(false));
                        return;
                    }
                }
                // DO NOT break here, falling into normal And if this can not be shotcutted.
            }
            case LogicalOpExp::logicalAnd :
            {
                e.right_get().accept(*this);
                pITR = result_get();
                GenericType::RealType TypeR = pITR->getType();

                if (TypeR == GenericType::RealBool)
                {
                    Bool *pR = pITR->getAs<types::Bool>();
                    int* piR = pR->get();
                    int* piL = pL->get();
                    int* piB = NULL;

                    // M & scalar
                    if (pR->getSize() == 1)
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols(), &piB);
                        for (int i = 0 ; i < pL->getSize(); i++)
                        {
                            piB[i] = (piR[0] == 1) && (piL[i] == 1);
                        }
                    }
                    else if (pL->getSize() == 1)
                    {
                        // scalar & M
                        pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                        for (int i = 0 ; i < pR->getSize(); i++)
                        {
                            piB[i] = (piR[i] == 1) && (piL[0] == 1);
                        }
                    }
                    else if (pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        // M & N (generic case)
                        pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                        for (int i = 0 ; i < pL->getSize(); i++)
                        {
                            piB[i] = (piR[i] == 1) && (piL[i] == 1);
                        }
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
                    pResult = callOverload(e.oper_get(), pITL, pITR);
                }
                break;
            }

            default :
            {
                pResult = callOverload(e.oper_get(), pITL, pITR);
                break;
            }
        }
    }
    else if (pITL->isInt())
    {
        int iErr = 2;
        e.right_get().accept(*this);
        pITR = result_get();
        GenericType::RealType TypeR = pITR->getType();

        if (TypeL != TypeR)
        {
            pResult = callOverload(e.oper_get(), pITL, pITR);
        }
        else
        {
            if (e.oper_get() == LogicalOpExp::logicalOr)
            {
                iErr = bitwiseOrToIntAndInt(pITL, pITR, &pResult);
            }
            else if (e.oper_get() == LogicalOpExp::logicalAnd)
            {
                iErr = bitwiseAndToIntAndInt(pITL, pITR, &pResult);
            }

            if (iErr)
            {
                std::wostringstream os;
                switch (iErr)
                {
                    case 1:
                        os << _W("Inconsistent row/column dimensions.\n");
                        break;
                    case 2:
                        os << _W("Bad operator.\n");
                        break; // if the operator is not logicalOr or logicalAnd
                    case 3:
                        os << _W("Bad type.\n");
                        break;// should never be occured.
                }
                throw ScilabError(os.str(), 999, e.right_get().location_get());
            }
        }
    }
    else
    {
        e.right_get().accept(*this);
        pITR = result_get();
        switch (e.oper_get())
        {
            case LogicalOpExp::logicalShortCutOr :
            case LogicalOpExp::logicalOr :
            case LogicalOpExp::logicalShortCutAnd :
            case LogicalOpExp::logicalAnd :
                pResult = callOverload(e.oper_get(), pITL, pITR);
                break;
            default :
                break;
        }
    }

    if (pITL && pITL->isDeletable())
    {
        delete pITL;
    }

    if (pITR && pITR->isDeletable())
    {
        delete pITR;
    }

    result_set(pResult);
}

types::InternalType* callOverload(OpExp::Oper _oper, types::InternalType* _paramL, types::InternalType* _paramR)
{
    types::typed_list in;
    types::typed_list out;

    /*
    ** Special case for unary minus => will call %{type_s}
    */
    if (_oper == OpExp::unaryMinus)
    {
        _paramR->IncreaseRef();
        in.push_back(_paramR);
        Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, this);

        _paramR->DecreaseRef();
        return out[0];
    }
    _paramL->IncreaseRef();
    _paramR->IncreaseRef();
    in.push_back(_paramL);
    in.push_back(_paramR);

    Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, this);

    _paramL->DecreaseRef();
    _paramR->DecreaseRef();
    return out[0];
}
