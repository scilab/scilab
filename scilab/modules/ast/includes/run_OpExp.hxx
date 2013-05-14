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
    try
    {
        InternalType *pITR = NULL; //assign only in non shortcut operations.

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

        if (pITL->getType() == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = pITL->getAs<ImplicitList>();
            if (pIL->isComputable())
            {
                pITL = pIL->extractFullMatrix();
            }
        }

        InternalType *pResult   = NULL;

        switch (e.oper_get())
        {
            case LogicalOpExp::logicalShortCutAnd :
            {
                pResult = GenericShortcutAnd(pITL);
                if (pResult)
                {
                    break;
                }

                //Continue to logicalAnd
            }
            case LogicalOpExp::logicalAnd :
            {
                /*getting what to assign*/
                e.right_get().accept(*this);
                pITR = result_get();
                if (is_single_result() == false)
                {
                    std::wostringstream os;
                    os << _W("Incompatible output argument.\n");
                    //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, e.right_get().location_get());
                }

                if (pITR->getType() == GenericType::RealImplicitList)
                {
                    ImplicitList* pIR = pITR->getAs<ImplicitList>();
                    if (pIR->isComputable())
                    {
                        pITR = pIR->extractFullMatrix();
                    }
                }
                pResult = GenericLogicalAnd(pITL, pITR);
                break;
            }
            case LogicalOpExp::logicalShortCutOr :
            {
                pResult = GenericShortcutOr(pITL);
                if (pResult)
                {
                    break;
                }

                //Continue to logicalAnd
            }
            case LogicalOpExp::logicalOr :
            {
                /*getting what to assign*/
                e.right_get().accept(*this);
                pITR = result_get();
                if (is_single_result() == false)
                {
                    std::wostringstream os;
                    os << _W("Incompatible output argument.\n");
                    //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, e.right_get().location_get());
                }

                if (pITR->getType() == GenericType::RealImplicitList)
                {
                    ImplicitList* pIR = pITR->getAs<ImplicitList>();
                    if (pIR->isComputable())
                    {
                        pITR = pIR->extractFullMatrix();
                    }
                }
                pResult = GenericLogicalOr(pITL, pITR);
                break;
            }
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

        if (pITR && pITR->isDeletable())
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
        Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, this, true);

        _paramR->DecreaseRef();
        return out[0];
    }
    _paramL->IncreaseRef();
    _paramR->IncreaseRef();
    in.push_back(_paramL);
    in.push_back(_paramR);

    Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, this, true);

    _paramL->DecreaseRef();
    _paramR->DecreaseRef();
    return out[0];
}
