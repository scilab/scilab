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
void RunVisitorT<T>::visitprivate(const OpExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    types::InternalType * pITL = NULL, *pITR = NULL, *pResult = NULL;
    try
    {
        /*getting what to assign*/
        e.getLeft().accept(*this);
        if (isSingleResult() == false)
        {
            clearResult();
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
            throw ast::InternalError(os.str(), 999, e.getRight().getLocation());
        }
        pITL = getResult();

        /*getting what to assign*/
        e.getRight().accept(*this);
        if (isSingleResult() == false)
        {
            clearResult();
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
            throw ast::InternalError(os.str(), 999, e.getRight().getLocation());
        }
        pITR = getResult();

        if (pITL->getType() == types::InternalType::ScilabImplicitList)
        {
            types::ImplicitList* pIL = pITL->getAs<types::ImplicitList>();
            if (pIL->isComputable())
            {
                pITL = pIL->extractFullMatrix();
                pIL->killMe();
            }
        }

        if (pITR->getType() == types::InternalType::ScilabImplicitList)
        {
            types::ImplicitList* pIR = pITR->getAs<types::ImplicitList>();
            if (pIR->isComputable())
            {
                pITR = pIR->extractFullMatrix();
                pIR->killMe();
            }
        }

        switch (e.getOper())
        {
            case OpExp::plus:
            {
                pResult = GenericPlus(pITL, pITR);
                break;
            }
            case OpExp::unaryMinus:
            {
                pResult = GenericUnaryMinus(pITR);
                break;
            }
            case OpExp::minus:
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
            case OpExp::dotldivide:
            {
                pResult = GenericDotLDivide(pITL, pITR);
                break;
            }
            case OpExp::rdivide:
            {
                pResult = GenericRDivide(pITL, pITR);
                break;
            }
            case OpExp::dotrdivide:
            {
                pResult = GenericDotRDivide(pITL, pITR);
                break;
            }
            case OpExp::dottimes:
            {
                pResult = GenericDotTimes(pITL, pITR);
                break;
            }
            case OpExp::dotpower:
            {
                pResult = GenericDotPower(pITL, pITR);
                break;
            }
            case OpExp::eq:
            {
                pResult = GenericComparisonEqual(pITL, pITR);
                break;
            }
            case OpExp::ne:
            {
                pResult = GenericComparisonNonEqual(pITL, pITR);
                break;
            }
            case OpExp::lt:
            {
                pResult = GenericLess(pITL, pITR);
                break;
            }
            case OpExp::le:
            {
                pResult = GenericLessEqual(pITL, pITR);
                break;
            }
            case OpExp::gt:
            {
                pResult = GenericGreater(pITL, pITR);
                break;
            }
            case OpExp::ge:
            {
                pResult = GenericGreaterEqual(pITL, pITR);
                break;
            }
            case OpExp::power:
            {
                pResult = GenericPower(pITL, pITR);
                break;
            }
            case OpExp::krontimes:
            {
                pResult = GenericKrontimes(pITL, pITR);
                break;
            }
            case OpExp::kronrdivide:
            {
                pResult = GenericKronrdivide(pITL, pITR);
                break;
            }
            case OpExp::kronldivide:
            {
                pResult = GenericKronldivide(pITL, pITR);
                break;
            }
            default:
                break;
        }

        //overloading
        if (pResult == NULL)
        {
            // We did not have any algorithm matching, so we try to call OverLoad
            pResult = callOverloadOpExp(e.getOper(), pITL, pITR);
        }

        setResult(pResult);

        //clear left and/or right operands
        if (pResult != pITL)
        {
            pITL->killMe();
        }

        if (pResult != pITR)
        {
            pITR->killMe();
        }
    }
    catch (ast::InternalError& error)
    {
        setResult(NULL);
        if (pResult)
        {
            pResult->killMe();
        }
        if (pITL && (pITL != pResult))
        {
            pITL->killMe();
        }
        if (pITR && (pITR != pResult))
        {
            pITR->killMe();
        }

        error.SetErrorLocation(e.getLocation());
        CoverageInstance::stopChrono((void*)&e);
        throw error;
    }

    CoverageInstance::stopChrono((void*)&e);
    /*if (e.getDecorator().res.isConstant())
    {

    }*/
}

template<class T>
void RunVisitorT<T>::visitprivate(const LogicalOpExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    types::InternalType *pITR = NULL; //assign only in non shortcut operations.
    types::InternalType *pITL = NULL;
    types::InternalType *pResult = NULL;

    try
    {

        /*getting what to assign*/
        e.getLeft().accept(*this);
        pITL = getResult();
        if (isSingleResult() == false)
        {
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
            throw ast::InternalError(os.str(), 999, e.getRight().getLocation());
        }

        setResult(NULL);

        if (pITL->getType() == types::InternalType::ScilabImplicitList)
        {
            types::ImplicitList* pIL = pITL->getAs<types::ImplicitList>();
            if (pIL->isComputable())
            {
                pITL = pIL->extractFullMatrix();
                pIL->killMe();
            }
        }

        switch (e.getOper())
        {
            case LogicalOpExp::logicalShortCutAnd:
            {
                pResult = GenericShortcutAnd(pITL);
                if (pResult)
                {
                    break;
                }

                //Continue to logicalAnd
            }
            case LogicalOpExp::logicalAnd:
            {
                /*getting what to assign*/
                e.getRight().accept(*this);
                pITR = getResult();
                if (isSingleResult() == false)
                {
                    std::wostringstream os;
                    os << _W("Incompatible output argument.\n");
                    //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
                    throw ast::InternalError(os.str(), 999, e.getRight().getLocation());
                }

                if (pITR->getType() == types::InternalType::ScilabImplicitList)
                {
                    types::ImplicitList* pIR = pITR->getAs<types::ImplicitList>();
                    if (pIR->isComputable())
                    {
                        pITR = pIR->extractFullMatrix();
                        pIR->killMe();
                    }
                }
                pResult = GenericLogicalAnd(pITL, pITR);
                break;
            }
            case LogicalOpExp::logicalShortCutOr:
            {
                pResult = GenericShortcutOr(pITL);
                if (pResult)
                {
                    break;
                }

                //Continue to logicalAnd
            }
            case LogicalOpExp::logicalOr:
            {
                /*getting what to assign*/
                e.getRight().accept(*this);
                pITR = getResult();
                if (isSingleResult() == false)
                {
                    std::wostringstream os;
                    os << _W("Incompatible output argument.\n");
                    //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
                    throw ast::InternalError(os.str(), 999, e.getRight().getLocation());
                }

                if (pITR->getType() == types::InternalType::ScilabImplicitList)
                {
                    types::ImplicitList* pIR = pITR->getAs<types::ImplicitList>();
                    if (pIR->isComputable())
                    {
                        pITR = pIR->extractFullMatrix();
                    }
                }
                pResult = GenericLogicalOr(pITL, pITR);
                break;
            }

            default:
                break;
        }
        //overloading
        if (pResult == NULL)
        {
            // We did not have any algorithm matching, so we try to call OverLoad
            e.getRight().accept(*this);
            pITR = getResult();
            if (isSingleResult() == false)
            {
                clearResult();
                std::wostringstream os;
                os << _W("Incompatible output argument.\n");
                //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
                throw ast::InternalError(os.str(), 999, e.getRight().getLocation());
            }

            if (pITR->getType() == types::InternalType::ScilabImplicitList)
            {
                types::ImplicitList* pIR = pITR->getAs<types::ImplicitList>();
                if (pIR->isComputable())
                {
                    pITR = pIR->extractFullMatrix();
                }
            }
            pResult = callOverloadOpExp(e.getOper(), pITL, pITR);
        }

        setResult(pResult);

        // protect pResult in case where pITL or pITR equal pResult
        pResult->IncreaseRef();

        //clear left and/or right operands
        pITL->killMe();
        if (pITR)
        {
            pITR->killMe();
        }

        // unprotect pResult
        pResult->DecreaseRef();
    }
    catch (ast::InternalError& error)
    {
        setResult(NULL);
        if (pResult)
        {
            pResult->killMe();
        }
        if (pITL && (pITL != pResult))
        {
            pITL->killMe();
        }
        if (pITR && (pITR != pResult))
        {
            pITR->killMe();
        }
        error.SetErrorLocation(e.getLocation());
        CoverageInstance::stopChrono((void*)&e);
        throw error;
    }

    CoverageInstance::stopChrono((void*)&e);
}

template<class T>
types::InternalType* RunVisitorT<T>::callOverloadOpExp(OpExp::Oper _oper, types::InternalType* _paramL, types::InternalType* _paramR)
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
        try
        {
            Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, true);
        }
        catch (const ast::InternalError& e)
        {
            _paramR->DecreaseRef();
            throw e;
        }

        _paramR->DecreaseRef();
        return out[0];
    }

    _paramL->IncreaseRef();
    _paramR->IncreaseRef();
    in.push_back(_paramL);
    in.push_back(_paramR);

    try
    {
        Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, true);
    }
    catch (const ast::InternalError& e)
    {
        _paramL->DecreaseRef();
        _paramR->DecreaseRef();
        throw e;
    }

    _paramL->DecreaseRef();
    _paramR->DecreaseRef();
    return out[0];
}

} /* namespace ast */
