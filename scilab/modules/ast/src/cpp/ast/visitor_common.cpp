/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#include <string>
#include <numeric>
#include <iostream>
#include <fstream>
#include "visitor_common.hxx"
#include "exp.hxx"
#include "fieldexp.hxx"
#include "simplevar.hxx"
#include "callexp.hxx"
#include "struct.hxx"
#include "context.hxx"
#include "execvisitor.hxx"
#include "serializervisitor.hxx"
#include "deserializervisitor.hxx"
#include "localization.hxx"
#include "user.hxx"

#include "alltypes.hxx"

extern "C"
{
#include "storeCommand.h"
}

size_t ast::Ast::globalNodeNumber = 0;

/*
 * Generate destination variable from _poSource type and size parameters
 */
types::InternalType* allocDest(types::InternalType* _poSource, int _iRows, int _iCols)
{
    types::InternalType* poResult = NULL;
    switch (_poSource->getType())
    {
        case types::InternalType::ScilabDouble :
            poResult = new types::Double(_iRows, _iCols, false);
            break;
        case types::InternalType::ScilabBool :
            poResult = new types::Bool(_iRows, _iCols);
            break;
        case types::InternalType::ScilabInt8 :
            poResult = new types::Int8(_iRows, _iCols);
            break;
        case types::InternalType::ScilabUInt8 :
            poResult = new types::UInt8(_iRows, _iCols);
            break;
        case types::InternalType::ScilabInt16 :
            poResult = new types::Int16(_iRows, _iCols);
            break;
        case types::InternalType::ScilabUInt16 :
            poResult = new types::UInt16(_iRows, _iCols);
            break;
        case types::InternalType::ScilabInt32 :
            poResult = new types::Int32(_iRows, _iCols);
            break;
        case types::InternalType::ScilabUInt32 :
            poResult = new types::UInt32(_iRows, _iCols);
            break;
        case types::InternalType::ScilabInt64 :
            poResult = new types::Int64(_iRows, _iCols);
            break;
        case types::InternalType::ScilabUInt64 :
            poResult = new types::UInt64(_iRows, _iCols);
            break;
        case types::InternalType::ScilabString :
            poResult = new types::String(_iRows, _iCols);
            break;
        case types::InternalType::ScilabPolynom :
        {
            int* piRank = new int[_iRows * _iCols];
            memset(piRank, 0x00, _iRows * _iCols * sizeof(int));
            poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
            delete[] piRank;
            break;
        }
        case types::InternalType::ScilabImplicitList:
            poResult = new types::ImplicitList();
            break;
        default :
            // FIXME : What should we do here ??
            break;
    }
    return poResult;
}

types::InternalType* AddElementToVariableFromCol(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols, int *_piCols)
{
    types::InternalType *poResult = NULL;
    types::InternalType::ScilabType TypeSource = _poSource->getType();
    types::InternalType::ScilabType TypeDest = types::InternalType::ScilabInternal;

    if (_poDest == NULL)
    {
        //First call, alloc _poSource
        poResult = allocDest(_poSource, _iRows, _iCols);
        TypeDest = TypeSource;
    }
    else
    {
        TypeDest = _poDest->getType();
        poResult = _poDest;
    }

    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
    }
    else
    {
        switch (TypeDest)
        {
            case types::InternalType::ScilabDouble :
                if (poResult->getAs<types::Double>()->isComplex() == false && _poSource->getAs<types::Double>()->isComplex() == true)
                {
                    poResult->getAs<types::Double>()->setComplex(true);
                }

                poResult->getAs<types::Double>()->fillFromCol(*_piCols, _poSource->getAs<types::Double>());
                *_piCols += _poSource->getAs<types::Double>()->getCols();

                break;
            default:
                break;
        }
        return poResult;
    }
    return NULL;
}

types::InternalType* AddElementToVariableFromRow(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols, int *_piRows)
{
    types::InternalType *poResult = NULL;
    types::InternalType::ScilabType TypeSource = _poSource->getType();
    types::InternalType::ScilabType TypeDest = types::InternalType::ScilabInternal;

    if (_poDest == NULL)
    {
        //First call, alloc _poSource
        poResult = allocDest(_poSource, _iRows, _iCols);
        TypeDest = TypeSource;
    }
    else
    {
        TypeDest = _poDest->getType();
        poResult = _poDest;
    }


    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
    }
    else
    {
        switch (TypeDest)
        {
            case types::InternalType::ScilabDouble :
                if (poResult->getAs<types::Double>()->isComplex() == false && _poSource->getAs<types::Double>()->isComplex() == true)
                {
                    poResult->getAs<types::Double>()->setComplex(true);
                }

                poResult->getAs<types::Double>()->fillFromRow(*_piRows, _poSource->getAs<types::Double>());
                *_piRows += _poSource->getAs<types::Double>()->getRows();

                break;
            default:
                break;
        }
        return poResult;
    }
    return NULL;
}


/*
_iRows : Position if _poDest allready initialized else size of the matrix
_iCols : Position if _poDest allready initialized else size of the matrix
*/
types::InternalType* AddElementToVariable(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols)
{
    types::InternalType *poResult = NULL;
    bool isNew = true;
    types::InternalType::ScilabType TypeSource = _poSource->getType();
    types::InternalType::ScilabType TypeDest = types::InternalType::ScilabInternal;
    int iCurRow = _iRows;
    int iCurCol = _iCols;

    if (_poDest == NULL)
    {
        switch (TypeSource)
        {
            case types::InternalType::ScilabDouble :
                poResult = new types::Double(_iRows, _iCols);
                break;
            case types::InternalType::ScilabBool :
                poResult = new types::Bool(_iRows, _iCols);
                break;
            case types::InternalType::ScilabInt8 :
                poResult = new types::Int8(_iRows, _iCols);
                break;
            case types::InternalType::ScilabUInt8 :
                poResult = new types::UInt8(_iRows, _iCols);
                break;
            case types::InternalType::ScilabInt16 :
                poResult = new types::Int16(_iRows, _iCols);
                break;
            case types::InternalType::ScilabUInt16 :
                poResult = new types::UInt16(_iRows, _iCols);
                break;
            case types::InternalType::ScilabInt32 :
                poResult = new types::Int32(_iRows, _iCols);
                break;
            case types::InternalType::ScilabUInt32 :
                poResult = new types::UInt32(_iRows, _iCols);
                break;
            case types::InternalType::ScilabInt64 :
                poResult = new types::Int64(_iRows, _iCols);
                break;
            case types::InternalType::ScilabUInt64 :
                poResult = new types::UInt64(_iRows, _iCols);
                break;
            case types::InternalType::ScilabString :
                poResult = new types::String(_iRows, _iCols);
                break;
            case types::InternalType::ScilabSparse :
                poResult = new types::Sparse(_iRows, _iCols);
                break;
            case types::InternalType::ScilabSparseBool :
                poResult = new types::SparseBool(_iRows, _iCols);
                break;
            case types::InternalType::ScilabPolynom :
            {
                int* piRank = new int[_iRows * _iCols];
                memset(piRank, 0x00, _iRows * _iCols * sizeof(int));
                poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
                delete[] piRank;
                break;
            }
            case types::InternalType::ScilabImplicitList :
                poResult = new types::ImplicitList();
                break;
            case types::InternalType::ScilabHandle :
                poResult = new types::GraphicHandle(_iRows, _iCols);
                break;
            default :
                // FIXME What should we do here ...
                break;
        }
        iCurCol = 0;
        iCurRow = 0;
        TypeDest =    TypeSource;
    }
    else
    {
        TypeDest = _poDest->getType();
        poResult = _poDest;
        isNew = false;
    }


    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
        switch (TypeDest)
        {
            case types::InternalType::ScilabDouble :
                if (TypeSource == types::InternalType::ScilabPolynom)
                {
                    types::Double *poDest = _poDest->getAs<types::Double>();
                    types::Polynom* pPSource = _poSource->getAs<types::Polynom>();

                    //Convert Dest to ScilabPolynom
                    int iSize = poDest->getSize();
                    int *piRank = new int[iSize];
                    memset(piRank, 0x00, iSize * sizeof(int));
                    if (isNew && poResult)
                    {
                        poResult->killMe();
                    }
                    poResult = new types::Polynom(pPSource->getVariableName(), poDest->getRows(), poDest->getCols(), piRank);
                    delete[] piRank;

                    types::Polynom* pPResult = poResult->getAs<types::Polynom>();
                    pPResult->setComplex(poDest->isComplex());

                    double *pR = poDest->getReal();
                    types::SinglePoly** pSP = pPResult->get();

                    if (poDest->isComplex())
                    {
                        double *pI = poDest->getImg();
                        for (int i = 0 ; i < iSize; i++)
                        {
                            pSP[i]->set(0, pR[i]);
                            pSP[i]->setImg(0, pI[i]);
                        }
                    }
                    else
                    {
                        for (int i = 0 ; i < iSize; i++)
                        {
                            pSP[i]->set(0, pR[i]);
                        }
                    }

                    for (int i = 0 ; i < pPSource->getRows() ; i++)
                    {
                        for (int j = 0 ; j < pPSource->getCols() ; j++)
                        {
                            pPResult->set(iCurRow + i, iCurCol + j, pPSource->get(i, j));
                        }
                    }

                    return poResult;
                }
                break;
            case types::InternalType::ScilabPolynom :
                if (TypeSource == types::InternalType::ScilabDouble)
                {
                    //Add Source like coef of the new element
                    types::Double* pD = _poSource->getAs<types::Double>();
                    types::Polynom* pPolyOut = poResult->getAs<types::Polynom>();

                    if (pD->isComplex())
                    {
                        pPolyOut->setComplex(true);
                        for (int i = 0 ; i < pD->getRows() ; i++)
                        {
                            for (int j = 0 ; j < pD->getCols() ; j++)
                            {
                                types::SinglePoly* pSPOut = pPolyOut->get(iCurRow + i, iCurCol + j);

                                pSPOut->setRank(0);
                                double pDblR = pD->get(i, j);
                                double pDblI = pD->getImg(i, j);
                                pSPOut->setCoef(&pDblR, &pDblI);
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0 ; i < pD->getRows() ; i++)
                        {
                            for (int j = 0 ; j < pD->getCols() ; j++)
                            {
                                types::SinglePoly* pSPOut = pPolyOut->get(iCurRow + i, iCurCol + j);

                                pSPOut->setRank(0);
                                double pDbl = pD->get(i, j);
                                pSPOut->setCoef(&pDbl, NULL);
                            }
                        }
                    }

                    return poResult;
                }
                break;
            case types::InternalType::ScilabSparse :
                if (TypeSource == types::InternalType::ScilabDouble)
                {
                    types::Double* poSource = _poSource->getAs<types::Double>();
                    types::Sparse* spResult = poResult->getAs<types::Sparse>();

                    // Set complex the result if one of inputs is complex
                    if (poSource->isComplex())
                    {
                        if (spResult->isComplex() == false)
                        {
                            spResult->toComplex();
                        }
                    }

                    // Add poSource at the end of spResult
                    if (spResult->isComplex())
                    {
                        if (poSource->isComplex())
                        {
                            for (int i = 0; i < poSource->getRows(); i++)
                            {
                                for (int j = 0; j < poSource->getCols(); j++)
                                {
                                    double dbl = poSource->get(i, j);
                                    double dblImg = poSource->getImg(i, j);
                                    if (dbl != 0 || dblImg != 0)
                                    {
                                        spResult->set(i + iCurRow, j + iCurCol, std::complex<double>(dbl, dblImg));
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i < poSource->getRows(); i++)
                            {
                                for (int j = 0; j < poSource->getCols(); j++)
                                {
                                    double dbl = poSource->get(i, j);
                                    if (dbl != 0)
                                    {
                                        spResult->set(i + iCurRow, j + iCurCol, std::complex<double>(dbl, 0));
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < poSource->getRows(); i++)
                        {
                            for (int j = 0; j < poSource->getCols(); j++)
                            {
                                double dbl = poSource->get(i, j);
                                if (dbl != 0)
                                {
                                    spResult->set(i + iCurRow, j + iCurCol, dbl);
                                }
                            }
                        }
                    }

                    return poResult;
                }
                break;
            case types::InternalType::ScilabSparseBool :
                if (TypeSource == types::InternalType::ScilabBool)
                {
                    types::Bool* poSource = _poSource->getAs<types::Bool>();
                    types::SparseBool* spResult = poResult->getAs<types::SparseBool>();

                    // Add poSource at the end of spResult
                    for (int i = 0; i < poSource->getRows(); i++)
                    {
                        for (int j = 0; j < poSource->getCols(); j++)
                        {
                            bool bValue = poSource->get(i, j) != 0;
                            if (bValue)
                            {
                                spResult->set(i + iCurRow, j + iCurCol, true);
                            }
                        }
                    }

                    return poResult;
                }
                break;
            default:
                break;
        }
        // call overload
        return NULL;
    }
    else
    {
        //Just add the new value in the current item
        switch (TypeDest)
        {
            case types::InternalType::ScilabDouble :
                poResult->getAs<types::Double>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabPolynom :
                poResult->getAs<types::Polynom>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabBool:
                poResult->getAs<types::Bool>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabInt8 :
                poResult->getAs<types::Int8>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabUInt8 :
                poResult->getAs<types::UInt8>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabInt16 :
                poResult->getAs<types::Int16>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabUInt16 :
                poResult->getAs<types::UInt16>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabInt32 :
                poResult->getAs<types::Int32>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabUInt32 :
                poResult->getAs<types::UInt32>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabInt64 :
                poResult->getAs<types::Int64>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabUInt64 :
                poResult->getAs<types::UInt64>()->append(iCurRow, iCurCol, _poSource);
                break;
            case types::InternalType::ScilabSparse :
                poResult->getAs<types::Sparse>()->append(iCurRow, iCurCol, _poSource->getAs<types::Sparse>());
                break;
            case types::InternalType::ScilabSparseBool :
                poResult->getAs<types::SparseBool>()->append(iCurRow, iCurCol, _poSource->getAs<types::SparseBool>());
                break;
            case types::InternalType::ScilabString :
            {
                poResult->getAs<types::String>()->append(iCurRow, iCurCol, _poSource);
            }
            break;
            case types::InternalType::ScilabImplicitList :
            {
                types::ImplicitList* pIL = _poSource->getAs<types::ImplicitList>();
                types::ImplicitList* pOL = poResult->getAs<types::ImplicitList>();
                pOL->setStart(pIL->getStart());
                pOL->setStep(pIL->getStep());
                pOL->setEnd(pIL->getEnd());
                break;
            }
            case types::InternalType::ScilabHandle :
                poResult->getAs<types::GraphicHandle>()->append(iCurRow, iCurCol, _poSource);
                break;
            default:
                // call overload
                return NULL;
        }
        return poResult;
    }
}

const std::wstring* getStructNameFromExp(const ast::Exp* _pExp)
{
    const ast::FieldExp* pField =  dynamic_cast<const ast::FieldExp*>(_pExp);
    const ast::SimpleVar* pVar =  dynamic_cast<const ast::SimpleVar*>(_pExp);
    const ast::CallExp* pCall =  dynamic_cast<const ast::CallExp*>(_pExp);

    if (pField)
    {
        return getStructNameFromExp(pField->getHead());
    }
    else if (pVar)
    {
        return &(pVar->getSymbol().getName());
    }
    else if (pCall)
    {
        return getStructNameFromExp(&(pCall->getName()));
    }
    else
    {
        std::wostringstream os;
        os << _W("Unknown expression");
        //os << ((Location)e.getRightExp().getLocation()).getLocationString() << std::endl;
        throw ast::InternalError(os.str(), 999, _pExp->getLocation());
    }
    return NULL;
}

/*** overload insertion                 ||         extraction       ***/
//%x_i_x(i1, i2, ..., in, source, dest) || %x_e(i1, i2, ..., in, source, dest)
//i1, ..., in : indexes
//dest : variable where insert data     || NULL
//source : data to insert               || extract indexes from source
types::InternalType* callOverload(const ast::Exp& e, const std::wstring& _strType, types::typed_list* _pArgs, types::InternalType* _source, types::InternalType* _dest)
{
    types::Function::ReturnValue ret = types::Function::Error;
    types::InternalType* pITOut = NULL;
    types::typed_list in;
    types::typed_list out;

    std::wstring function_name;
    function_name = L"%" + _source->getShortTypeStr() + L"_" + _strType;

    for (int i = 0; i < (int)_pArgs->size(); i++)
    {
        (*_pArgs)[i]->IncreaseRef();
        in.push_back((*_pArgs)[i]);
    }

    _source->IncreaseRef();
    in.push_back(_source);

    if (_dest)
    {
        _dest->IncreaseRef();
        in.push_back(_dest);

        function_name += L"_" + _dest->getShortTypeStr();
    }

    types::InternalType* pFunc = symbol::Context::getInstance()->get(symbol::Symbol(function_name));
    // if %type_6 doesn't exist, call %l_6
    if (_dest == NULL && pFunc == NULL)
    {
        function_name = L"%l_" + _strType;
    }

    // For insertion in TList, call normal insertion if overload doesn't exits
    if ((_dest  && _dest->isTList() && pFunc == NULL) == false || _source->isListDelete())
    {
        try
        {
            ret = Overload::call(function_name, in, 1, out);
        }
        catch (const ast::InternalError& error)
        {
            // unprotect variables
            for (int i = 0; i < (int)_pArgs->size(); i++)
            {
                (*_pArgs)[i]->DecreaseRef();
            }

            _source->DecreaseRef();
            if (_dest)
            {
                _dest->DecreaseRef();
            }

            throw error;
        }

        // unprotect variables
        for (int i = 0; i < (int)_pArgs->size(); i++)
        {
            (*_pArgs)[i]->DecreaseRef();
        }

        _source->DecreaseRef();
        if (_dest)
        {
            _dest->DecreaseRef();
        }

        if (ret == types::Function::Error)
        {
            //manage error
            std::wostringstream os;
            os << _W("Error in overload function: ") << function_name << std::endl;
            throw ast::InternalError(os.str(), 999, e.getLocation());
        }
    }

    if (out.size() == 1)
    {
        pITOut = out[0];
    }
    else if (out.size() > 1)
    {
        types::List* pListOut = new types::List();
        for (int i = 0; i < (int)out.size(); i++)
        {
            pListOut->append(out[i]);
        }

        pITOut = pListOut;
    }

    return pITOut;
}


bool getFieldsFromExp(ast::Exp* _pExp, std::list<ExpHistory*>& fields)
{
    ast::FieldExp* pField      = dynamic_cast<ast::FieldExp*>(_pExp);
    ast::SimpleVar* pVar       = dynamic_cast<ast::SimpleVar*>(_pExp);
    ast::CallExp* pCall        = dynamic_cast<ast::CallExp*>(_pExp);
    ast::CellCallExp* pCell    = dynamic_cast<ast::CellCallExp*>(_pExp);

    if (pField)
    {
        if (getFieldsFromExp(pField->getHead(), fields))
        {
            return getFieldsFromExp(pField->getTail(), fields);
        }

        return false;
    }
    else if (pVar)
    {
        if (fields.empty())
        {
            fields.push_back(new ExpHistory(NULL, pVar));
        }
        else
        {
            ExpHistory * pEHParent = fields.back();
            ExpHistory * pEH = new ExpHistory(pEHParent, pVar);
            pEH->setLevel(pEHParent->getLevel() + 1);
            fields.push_back(pEH);
        }

        return true;
    }
    else if (pCall)
    {
        bool bArgList = false;
        types::List* pList = NULL;
        int iListIncr = 0;
        int iListSize = 0;

        ast::ExecVisitor execMe;
        ast::exps_t args = pCall->getArgs();
        types::typed_list* pCurrentArgs = execMe.GetArgumentList(args);

        if (getFieldsFromExp(&pCall->getName(), fields) == false)
        {
            return false;
        }

        // used to manage insertion with list in argument
        // a(list("field", 2)) = 2 as a.field(2)
        if (pCurrentArgs &&  pCurrentArgs->size() > 0 &&
                (*pCurrentArgs)[0]->isList() &&
                (*pCurrentArgs)[0]->isTList() == false &&
                (*pCurrentArgs)[0]->isMList() == false)
        {
            bArgList = true;
            pList = (*pCurrentArgs)[0]->getAs<types::List>();
            //pList->IncreaseRef();
            pCurrentArgs->clear();
            pCurrentArgs->push_back(pList->get(iListIncr));
            iListSize = pList->getSize();

        }

        do
        {
            if (pCurrentArgs &&
                    pCurrentArgs->size() == 1 &&
                    (*pCurrentArgs)[0]->isString() &&
                    (*pCurrentArgs)[0]->getAs<types::String>()->getSize() == 1)
            {
                // a("b") => a.b or a(x)("b") => a(x).b
                ExpHistory * pEHParent = fields.back();
                ast::SimpleVar* pFieldVar = new ast::SimpleVar(pCall->getLocation(), symbol::Symbol((*pCurrentArgs)[0]->getAs<types::String>()->get(0)));
                ExpHistory * pEH = new ExpHistory(pEHParent, pFieldVar);
                pEH->setLevel(pEHParent->getLevel() + 1);
                pEH->setExpOwner(true);

                (*pCurrentArgs)[0]->killMe();
                delete pCurrentArgs;
                pCurrentArgs = NULL;

                fields.push_back(pEH);
            }
            else if (fields.back()->getArgs())
            {
                // a(x)(y)(z)
                ExpHistory * pEHParent = fields.back();
                ExpHistory * pEH = new ExpHistory(pEHParent, pCurrentArgs);
                pEH->setLevel(pEHParent->getLevel() + 1);
                pEH->setArgsOwner(true);
                fields.push_back(pEH);
            }
            else
            {
                // a(x)
                fields.back()->setArgs(pCurrentArgs);
                fields.back()->setArgsOwner(true);
            }

            if (bArgList)
            {
                iListIncr++;
                if (iListIncr < iListSize)
                {
                    // create new args for next loop.
                    pCurrentArgs = new types::typed_list();
                    pCurrentArgs->push_back(pList->get(iListIncr)->clone());
                }
            }
        }
        while (iListIncr < iListSize);

        if (bArgList)
        {
            pList->killMe();
        }

        if (pCell)
        {
            // a{x}
            fields.back()->setCellExp();
        }

        return true;
    }
    else
    {
        return false;
    }
}

types::InternalType* evaluateFields(const ast::Exp* _pExp, std::list<ExpHistory*>& fields, types::InternalType* _pAssignValue)
{
    std::list<ExpHistory*> evalFields;
    std::list<ExpHistory*> workFields;

    bool bPutInCtx = false;
    types::InternalType* pITMain = NULL;

    try
    {
        //*** get main variable ***//
        std::list<ExpHistory*>::iterator iterFields = fields.begin();
        ExpHistory* pFirstField = *iterFields;
        symbol::Context* ctx = symbol::Context::getInstance();

        if (ctx->isprotected(pFirstField->getExp()->getSymbol()))
        {
            std::wostringstream os;
            os << _W("Redefining permanent variable.\n");
            throw ast::InternalError(os.str(), 999, _pExp->getLocation());
        }

        ast::SimpleVar* spMainExp = pFirstField->getExp();
        pITMain = ctx->getCurrentLevel(spMainExp->getSymbol());

        // looking for a callable
        if (pITMain == NULL)
        {
            pITMain = ctx->get(spMainExp->getSymbol());
            if (pITMain && pITMain->isCallable() == false)
            {
                pITMain = NULL;
            }
        }

        if (pITMain == NULL)
        {
            if (pFirstField->isCellExp())
            {
                // a{x}, where "a" doesn't exists
                pITMain = new types::Cell(1, 1);
                pITMain->IncreaseRef();
                bPutInCtx = true;
            }
            else if (fields.size() > 1)
            {
                // is a field exp
                //"a" does not exist or it is another type, create it with size 1,1 and return it
                //create new structure variable
                pITMain = new types::Struct(1, 1);
                pITMain->IncreaseRef();
                bPutInCtx = true;
            }
            // else
            // is a call exp
            // a(x) = "something" and a does not exist
            // a will be create in insertionCall
        }
        else if (pITMain->getRef() > 1 && pITMain->isHandle() == false)
        {
            bPutInCtx = true;
            pITMain = pITMain->clone();
            pITMain->IncreaseRef();
        }
        else if (pITMain == _pAssignValue)
        {
            // clone me before insert me in myself.
            // ie : a.b = 2; a.b.c.d = a;
            _pAssignValue = _pAssignValue->clone();
        }

        iterFields++;

        workFields.push_back(new ExpHistory(NULL,
                                            pFirstField->getExp(),
                                            pFirstField->getArgs(),
                                            pFirstField->getLevel(),
                                            pFirstField->isCellExp(),
                                            pITMain));

        //*** evaluate fields ***//
        while (iterFields != fields.end())
        {
            ExpHistory* pEH = workFields.front();
            evalFields.push_back(pEH);
            workFields.pop_front();

            types::InternalType* pITCurrent = pEH->getCurrent();

            if (pEH->isCellExp() && pITCurrent->isCell() == false)
            {
                std::wostringstream os;
                os << _W("Wrong insertion : use extraction with {} only on a Cell.");
                throw ast::InternalError(os.str(), 999, _pExp->getLocation());
            }

            if (pITCurrent->isStruct())
            {
                types::Struct* pStruct = pITCurrent->getAs<types::Struct>();
                std::wstring pwcsFieldname = (*iterFields)->getExpAsString();

                if (pEH->needResize())
                {
                    if (pEH->getArgsDims() == 1)
                    {
                        std::wostringstream os;
                        os << _W("Invalid index.");
                        throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                    }

                    // resize current struct
                    pStruct = pStruct->resize(pEH->getArgsDimsArray(), pEH->getArgsDims());
                    pEH->setCurrent(pStruct);
                }

                // create field in parent if it not exist
                if (pStruct->exists(pwcsFieldname) == false)
                {
                    pStruct = pStruct->addField(pwcsFieldname);
                    pEH->setCurrent(pStruct);
                }

                if (pEH->getArgs())
                {
                    types::InternalType* pIT = pStruct->extractWithoutClone(pEH->getArgs());
                    workFields.push_front(new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pIT));
                }
                else
                {
                    // check if the field x is the last field
                    std::list<ExpHistory*>::iterator iterFieldsNext(iterFields);
                    ++iterFieldsNext;
                    if (iterFieldsNext == fields.end() && (*iterFields)->getArgs() == NULL)
                    {
                        // create pArg with "x" and set it as argument of "a"
                        types::typed_list* args = new types::typed_list();
                        args->push_back(new types::String(pwcsFieldname.c_str()));
                        pEH->setArgs(args);

                        // a.x where x is the last field
                        // insert directly in x instead of extract then insert
                        ExpHistory* pEHNext = new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), _pAssignValue);
                        pEHNext->setReinsertion(true);
                        evalFields.push_back(pEHNext);
                        if (workFields.empty())
                        {
                            break;
                        }
                    }
                    else
                    {
                        // Avoid insertion in most of one element.
                        if (pStruct->isScalar() == false)
                        {
                            std::wostringstream os;
                            os << _W("Unable to insert multiple item in a Struct.");
                            throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                        }

                        // extract field x and append it to elements for next recursion.
                        types::List* pLOut = pStruct->extractFieldWithoutClone(pwcsFieldname);

                        // pStruct must be scalar because we cant insert most of one element in the same insertion
                        types::InternalType* pIT = pLOut->get(0);
                        if (pIT->getRef() > 2) //One for my own ref + 1 for "extractFieldWithoutClone" artificial ref
                        {
                            // clone element before modify it.
                            //pIT->DecreaseRef();
                            pIT = pIT->clone();
                            pStruct->get(0)->set(pwcsFieldname, pIT);
                        }

                        ExpHistory* pEHChield = new ExpHistory(pEH,
                                                               (*iterFields)->getExp(),
                                                               (*iterFields)->getArgs(),
                                                               (*iterFields)->getLevel(),
                                                               (*iterFields)->isCellExp(),
                                                               pIT);

                        pEHChield->setWhereReinsert(0);
                        workFields.push_back(pEHChield);

                        pLOut->killMe();
                    }
                }
            }
            else if (pITCurrent->isTList() || pITCurrent->isMList())
            {
                types::TList* pTL = pITCurrent->getAs<types::TList>();
                types::typed_list* pArgs = pEH->getArgs();
                if (pArgs)
                {
                    if (pArgs->size() > 1 || pITCurrent->isMList())
                    {
                        // call overload
                        types::InternalType* pExtract = callOverload(*pEH->getExp(), L"6", pArgs, pTL, NULL);
                        if ((*iterFields)->getExp() == NULL)
                        {
                            // a(x)(y)
                            // extract a(x) and push_BACK to extract y
                            workFields.push_back(new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                            workFields.back()->setReinsertion();
                        }
                        else
                        {
                            // a(x).b
                            // extract a(x) and push_FRONT to extract b
                            workFields.push_front(new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pExtract));
                            workFields.front()->setReinsertion();
                        }
                    }
                    else
                    {
                        // resize TList
                        int iNewSize = pEH->getSizeFromArgs();
                        if (pTL->getSize() < iNewSize)
                        {
                            pTL = pTL->set(iNewSize - 1, new types::ListUndefined());
                            pEH->setCurrent(pTL);
                        }

                        // update pArgs variables with new argument computed in getSizeFromArgs
                        pArgs = pEH->getArgs();

                        types::InternalType* pIT = pTL->extract(pArgs);
                        types::List* pList = pIT->getAs<types::List>();

                        if (pList->getSize() > 1)
                        {
                            pList->killMe();
                            std::wostringstream os;
                            os << _W("Unable to insert multiple item in a List.");
                            throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                        }

                        double* pdblArgs = (*pArgs)[0]->getAs<types::Double>()->get();
                        if ((*iterFields)->getExp() == NULL)
                        {
                            // a(x)(y)
                            // extract a(x) and push_BACK to extract y
                            ExpHistory* pEHExtract = new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pList->get(0));
                            pEHExtract->setWhereReinsert((int)(pdblArgs[0] - 1));
                            workFields.push_back(pEHExtract);
                        }
                        else
                        {
                            // a(x).b
                            // extract a(x) and push_FRONT to extract b
                            ExpHistory* pEHExtract = new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pList->get(0));
                            pEHExtract->setWhereReinsert((int)(pdblArgs[0] - 1));
                            workFields.push_front(pEHExtract);
                        }

                        //extract create a list to store items
                        pList->killMe();
                    }
                }
                else
                {
                    // get string "x" of a.x
                    types::InternalType* pExtract = NULL;
                    std::wstring pwcsFieldname = L"";
                    bool bReinsert = false;
                    ExpHistory* pEHChield = NULL;

                    pwcsFieldname = (*iterFields)->getExpAsString();

                    // check if the field x is the last field
                    std::list<ExpHistory*>::iterator iterFieldsNext(iterFields);
                    ++iterFieldsNext;
                    if (iterFieldsNext == fields.end() && (*iterFields)->getArgs() == NULL)
                    {
                        // create pArg with "x" and set it as argument of "a"
                        types::typed_list* args = new types::typed_list();
                        args->push_back(new types::String(pwcsFieldname.c_str()));
                        pEH->setArgs(args);

                        // a.x where x is the last field
                        // insert directly in x instead of extract then insert
                        ExpHistory* pEHNext = new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), _pAssignValue);
                        pEHNext->setReinsertion(true);
                        evalFields.push_back(pEHNext);
                        if (workFields.empty())
                        {
                            break;
                        }
                    }
                    else
                    {
                        // check if field exists
                        if (pTL->exists(pwcsFieldname) == false)
                        {
                            std::list<ExpHistory*>::iterator iterFieldsNext(iterFields);
                            ++iterFieldsNext;

                            if (iterFieldsNext != fields.end() || (*iterFields)->getArgs() != NULL)
                            {
                                // M=mlist(['MType','x','y'], ...
                                // M.rows1 = "somthing"
                                pArgs = new types::typed_list();
                                pArgs->push_back(new types::String(pwcsFieldname.c_str()));

                                // call overload
                                pExtract = callOverload(*pEH->getExp(), L"6", pArgs, pTL, NULL);
                                bReinsert = true;

                                delete pArgs;
                            }
                        }
                        else
                        {
                            // extract field x and append it to elements for next recursion.
                            pExtract = pTL->getField(pwcsFieldname);
                        }

                        pEHChield = new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract);
                        workFields.push_back(pEHChield);

                        if (bReinsert)
                        {
                            pEHChield->setReinsertion();
                        }
                    }
                }
            }
            else if (pITCurrent->isList())
            {
                types::List* pL = pITCurrent->getAs<types::List>();
                if (pEH->getParent() && pEH->getParent()->getLevel() == pEH->getLevel())
                {
                    std::wostringstream os;
                    os << _W("Wrong insertion.");
                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());

                    //                    // pITCurrent is an extraction of other Type
                    //                    for (int iLoop = 0; iLoop < pL->getSize(); iLoop++)
                    //                    {
                    //                        ExpHistory* pEHExtract = new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pL->get(iLoop));
                    //                        pEHExtract->setWhereReinsert(iLoop);
                    //                        workFields.push_front(pEHExtract);
                    //                    }
                }
                else
                {
                    // pITCurrent is a field
                    if (pEH->getArgs())
                    {
                        if (pEH->getArgs()->size() > 1)
                        {
                            // call overload
                            types::InternalType* pExtract = callOverload(*pEH->getExp(), L"6", pEH->getArgs(), pL, NULL);

                            if ((*iterFields)->getExp() == NULL)
                            {
                                // a(x)(y)
                                // extract a(x) and push_BACK to extract next level
                                workFields.push_back(new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                                workFields.back()->setReinsertion();
                            }
                            else
                            {
                                // a(x).b
                                // extract a(x) and push_FRONT to extract b from a(x)
                                workFields.push_front(new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pExtract));
                                workFields.front()->setReinsertion();
                            }
                        }
                        else
                        {
                            // resize List
                            int iNewSize = pEH->getSizeFromArgs();
                            if (pL->getSize() < iNewSize)
                            {
                                pL = pL->set(iNewSize - 1, new types::ListUndefined());
                                pEH->setCurrent(pL);
                            }

                            types::Double* pDblArgs = (*pEH->getArgs())[0]->getAs<types::Double>();
                            double* pdblArgs = pDblArgs->get();

                            if ((*iterFields)->getExp() == NULL)
                            {
                                // a(x)(y) => a.b(y)
                                // extract a(x) and push_BACK to extract next level
                                for (int iLoop = 0; iLoop < pDblArgs->getSize(); iLoop++)
                                {
                                    ExpHistory* pEHExtract = new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pL->get((int)pdblArgs[iLoop] - 1));
                                    pEHExtract->setWhereReinsert((int)(pdblArgs[iLoop] - 1));
                                    workFields.push_back(pEHExtract);
                                }
                            }
                            else
                            {
                                // a(x).b
                                // extract a(x) and push_FRONT to extract b from a(x)
                                for (int iLoop = 0; iLoop < pDblArgs->getSize(); iLoop++)
                                {
                                    ExpHistory* pEHExtract = new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pL->get((int)pdblArgs[iLoop] - 1));
                                    pEHExtract->setWhereReinsert((int)(pdblArgs[iLoop] - 1));
                                    workFields.push_front(pEHExtract);
                                }
                            }
                        }
                    }
                    else
                    {
                        // a.x, get string "x"
                        std::wstring pwcsFieldname = (*iterFields)->getExpAsString();

                        // create pArg with "x"
                        types::typed_list* args = new types::typed_list();
                        args->push_back(new types::String(pwcsFieldname.c_str()));
                        pEH->setArgs(args);

                        // check if the field x is the last field
                        std::list<ExpHistory*>::iterator iterFieldsNext(iterFields);
                        ++iterFieldsNext;
                        if (iterFieldsNext == fields.end() && (*iterFields)->getArgs() == NULL)
                        {
                            // a.x where x is the last field
                            // insert directly in x instead of extract then insert
                            ExpHistory* pEHNext = new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), _pAssignValue);
                            pEHNext->setReinsertion(true);
                            evalFields.push_back(pEHNext);
                            if (workFields.empty())
                            {
                                break;
                            }
                        }
                        else
                        {
                            // call overload
                            types::InternalType* pExtract = callOverload(*pEH->getExp(), L"6", args, pL, NULL);

                            // append extraction of a.x for next level.
                            workFields.push_back(new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                            workFields.back()->setReinsertion();
                        }
                    }
                }
            }
            else if (pITCurrent->isHandle())
            {
                types::typed_list* pArgs = pEH->getArgs();
                types::GraphicHandle* pGH = pITCurrent->getAs<types::GraphicHandle>();
                if (pArgs)
                {
                    types::InternalType* pExtract = NULL;

                    if (pArgs->size() == 1 && (*pArgs)[0]->isImplicitList() == false)
                    {
                        // call overload
                        pExtract = callOverload(*pEH->getExp(), L"e", pArgs, pITCurrent, NULL);
                    }
                    else
                    {
                        pExtract = pGH->extract(pArgs);
                    }

                    if (pExtract == NULL)
                    {
                        std::wostringstream os;
                        os << _W("Invalid index.");
                        throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                    }

                    if ((*iterFields)->getExp() == NULL)
                    {
                        // a(x)(y)
                        // extract a(x) and push_BACK to extract next level
                        workFields.push_back(new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                        workFields.back()->setReinsertion();
                    }
                    else
                    {
                        // a(x).b
                        // extract a(x) and push_FRONT to extract b from a(x)
                        workFields.push_front(new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pExtract));
                        workFields.front()->setReinsertion();
                    }
                }
                else
                {
                    // a.x, get string "x"
                    std::wstring pwcsFieldname = (*iterFields)->getExpAsString();

                    // create arg with next field
                    types::typed_list* args = new types::typed_list();
                    args->push_back(new types::String(pwcsFieldname.c_str()));
                    pEH->setArgs(args);

                    // check if the field x is the last field
                    std::list<ExpHistory*>::iterator iterFieldsNext(iterFields);
                    ++iterFieldsNext;
                    if (iterFieldsNext == fields.end() && (*iterFields)->getArgs() == NULL)
                    {
                        // a.x where x is the last field
                        // insert directly in x instead of extract then insert
                        ExpHistory* pEHNext = new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), _pAssignValue);
                        pEHNext->setReinsertion(true);
                        evalFields.push_back(pEHNext);
                        if (workFields.empty())
                        {
                            break;
                        }
                    }
                    else
                    {
                        // call overload
                        types::InternalType* pExtract = callOverload(*pEH->getExp(), L"e", args, pITCurrent, NULL);

                        // append extraction of a.x for next level.
                        workFields.push_back(new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                        workFields.front()->setReinsertion();
                    }
                }
            }
            else if (pITCurrent->isCell())
            {
                if (pEH->getArgs() && (*pEH->getArgs())[0]->isString() == false)
                {
                    if (pEH->isCellExp())
                    {
                        types::Cell* pCell = pITCurrent->getAs<types::Cell>();
                        // a{x} => extract like a(x){[1 2 ...]}
                        if (pEH->getParent() && pEH->getLevel() == pEH->getParent()->getLevel())
                        {
                            // extract each elements of a(x)
                            for (int iCell = 0; iCell < pCell->getSize(); iCell++)
                            {
                                types::InternalType* pIT = pCell->get(iCell);
                                if ((*iterFields)->getExp() == NULL)
                                {
                                    // a{x}(y)
                                    ExpHistory* pEHChield = new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pIT);
                                    pEHChield->setWhereReinsert(iCell);
                                    workFields.push_back(pEHChield);
                                }
                                else
                                {
                                    // a{x}.b
                                    ExpHistory* pEHChield = new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), false, pIT);
                                    pEHChield->setWhereReinsert(iCell);
                                    workFields.push_front(pEHChield);
                                }
                            }
                        }
                        else
                        {
                            types::GenericType* pCell = pITCurrent->getAs<types::GenericType>();
                            if (pEH->needResize())
                            {
                                if (pEH->getArgsDims() == 1)
                                {
                                    std::wostringstream os;
                                    os << _W("Invalid index.");
                                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                                }

                                // resize current Cell
                                pCell = pCell->resize(pEH->getArgsDimsArray(), pEH->getArgsDims());
                                pEH->setCurrent(pCell);
                            }

                            types::InternalType* pIT = pCell->extract(pEH->getArgs());
                            workFields.push_front(new ExpHistory(pEH, pEH->getExp(), pEH->getArgs(), pEH->getLevel(), pEH->isCellExp(), pIT));
                            workFields.front()->setReinsertion();
                        }
                    }
                    else
                    {
                        if ((*iterFields)->isCellExp())
                        {
                            types::GenericType* pCell = pITCurrent->getAs<types::GenericType>();

                            // a(x){y}
                            if (pEH->needResize())
                            {
                                if (pEH->getArgsDims() == 1)
                                {
                                    std::wostringstream os;
                                    os << _W("Invalid index.");
                                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                                }

                                // resize current Cell
                                pCell = pCell->resize(pEH->getArgsDimsArray(), pEH->getArgsDims())->getAs<types::Cell>();
                                pEH->setCurrent(pCell);
                            }

                            types::InternalType* pIT = pCell->extract(pEH->getArgs());
                            workFields.push_back(new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pIT));
                            workFields.front()->setReinsertion();
                        }
                        else
                        {
                            // only a(x)
                            std::wostringstream os;
                            os << _W("Wrong insertion in a Cell.");
                            throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                        }
                    }
                }
                else
                {
                    std::wostringstream os;
                    os << _W("Wrong insertion in a Cell.");
                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                }
            }
            else if (pITCurrent->isUserType()) // not a Scilab defined datatype, access field after field
            {
                // call userType extract method
                if (pEH->getArgs())
                {
                    // a(x)
                    types::InternalType* pExtract = pITCurrent->getAs<types::UserType>()->extract(pEH->getArgs());
                    if (pExtract == NULL)
                    {
                        // call overload
                        pExtract = callOverload(*pEH->getExp(), L"e", pEH->getArgs(), pITCurrent, NULL);
                    }

                    if ((*iterFields)->getExp() == NULL)
                    {
                        // a(x)(y)
                        // extract a(x) and push_BACK to extract next level
                        workFields.push_back(new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                        workFields.back()->setReinsertion();
                    }
                    else
                    {
                        // a(x).b
                        // extract a(x) and push_FRONT to extract b from a(x)
                        workFields.push_front(new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pExtract));
                        workFields.front()->setReinsertion();
                    }
                }
                else
                {
                    // a.x, get string "x"
                    std::wstring pwcsFieldname = (*iterFields)->getExpAsString();

                    // create arg with next field
                    types::typed_list* args = new types::typed_list();
                    args->push_back(new types::String(pwcsFieldname.c_str()));
                    pEH->setArgs(args);

                    // check if the field x is the last field
                    std::list<ExpHistory*>::iterator iterFieldsNext(iterFields);
                    ++iterFieldsNext;
                    if (iterFieldsNext == fields.end() && (*iterFields)->getArgs() == NULL)
                    {
                        // a.x where x is the last field
                        // insert directly in x instead of extract then insert
                        ExpHistory* pEHNext = new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), _pAssignValue);
                        pEHNext->setReinsertion(true);
                        evalFields.push_back(pEHNext);
                        if (workFields.empty())
                        {
                            break;
                        }
                    }
                    else
                    {
                        types::InternalType* pExtract = pITCurrent->getAs<types::UserType>()->extract(args);
                        if (pExtract == NULL)
                        {
                            // call overload
                            pExtract = callOverload(*pEH->getExp(), L"e", args, pITCurrent, NULL);
                        }

                        // append extraction of a.x for next level.
                        workFields.push_back(new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                        workFields.back()->setReinsertion();
                    }
                }
            }
            else if (pITCurrent->isCallable())
            {
                bool ret = false;
                types::typed_list out;
                types::typed_list in;
                types::optional_list opt;

                if (pEH->getArgs())
                {
                    in = *pEH->getArgs();
                }

                try
                {
                    ret = pITCurrent->invoke(in, opt, 1, out, *pEH->getExp());
                }
                catch (ast::InternalAbort& ia)
                {
                    throw ia;
                }
                catch (const ast::InternalError& ie)
                {
                    throw ie;
                }

                if (ret == false || out.size() != 1 || out[0]->isHandle() == false)
                {
                    char szError[bsiz];
                    char* strFName = wide_string_to_UTF8(pITCurrent->getAs<types::Callable>()->getName().c_str());
                    os_sprintf(szError, _("Wrong insertion: insertion in output of '%s' is not allowed.\n"), strFName);
                    FREE(strFName);

                    wchar_t* wError = to_wide_string(szError);
                    std::wstring err(wError);
                    FREE(wError);

                    throw ast::InternalError(err, 999, pEH->getExp()->getLocation());
                }

                pEH->setCurrent(out[0]);
                pEH->setArgs(NULL);
                pEH->resetReinsertion();
                workFields.push_front(pEH);
                evalFields.pop_back();
            }
            else
            {
                types::InternalType* pIT = new types::Struct(1, 1);
                pEH->setCurrent(pIT);
                pEH->setReinsertion();

                workFields.push_front(pEH);
                evalFields.pop_back();
            }

            if (workFields.front()->getLevel() == (*iterFields)->getLevel())
            {
                // go to next field
                iterFields++;
            }
        }

        //*** insert what we have to assign             ***//
        //*** in case where the last field is a CallExp ***//
        while (workFields.empty() == false)
        {
            ExpHistory* pEH = workFields.front();
            evalFields.push_back(pEH);
            workFields.pop_front();

            types::typed_list* pArgs = pEH->getArgs();

            // should never occured
            if (pArgs == NULL || pArgs->size() == 0)
            {
                std::wostringstream os;
                os << _W("Wrong insertion : Cannot insert without arguments.");
                throw ast::InternalError(os.str(), 999, _pExp->getLocation());
            }

            if (pEH->isCellExp())
            {
                types::Cell* pCell = pEH->getCurrent()->getAs<types::Cell>();
                // insert "something" in b{x}
                if ((*pArgs)[0]->isString())
                {
                    std::wostringstream os;
                    os << _W("Wrong insertion in a Cell.");
                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                }

                pCell->insertCell(pArgs, _pAssignValue);
            }
            else if (pEH->getCurrent() && pEH->getCurrent()->isCallable())
            {
                std::wostringstream os;
                os << _W("Unexpected redefinition of Scilab function.");
                throw ast::InternalError(os.str(), 999, _pExp->getLocation());
            }
            else
            {
                // insert "something" in b(x,y)
                types::InternalType* pIT = insertionCall(*_pExp, pArgs, pEH->getCurrent(), _pAssignValue);
                if (pIT == NULL)
                {
                    std::wostringstream os;
                    os << _W("Submatrix incorrectly defined.\n");
                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                }

                pEH->setCurrent(pIT);
            }
        }

        //*** update fields ***//
        while (evalFields.empty() == false)
        {
            ExpHistory* pEH = evalFields.back();
            if (pEH->reinsertMe())
            {
                ExpHistory* pEHParent = pEH->getParent();

                if (pEHParent == NULL)
                {
                    if (bPutInCtx)
                    {
                        pITMain->DecreaseRef();
                        pITMain->killMe();
                    }

                    bPutInCtx = true;
                    pITMain = pEH->getCurrent();
                    pITMain->IncreaseRef();
                    break;
                }

                types::typed_list* pParentArgs = pEHParent->getArgs();
                if (pParentArgs == NULL || pEH->getWhereReinsert() != -1)
                {
                    types::InternalType* pParent = pEHParent->getCurrent();
                    if (pParent->isStruct())
                    {
                        types::Struct* pStruct = pParent->getAs<types::Struct>();
                        pStruct->get(pEH->getWhereReinsert())->set(pEH->getExpAsString(), pEH->getCurrent());
                        evalFields.pop_back();
                        delete pEH;
                        continue;
                    }
                    else if (pParent->isTList() || pParent->isMList())
                    {
                        types::TList* pTL = pParent->getAs<types::TList>();
                        if (pParentArgs)
                        {
                            pTL = pTL->set(pEH->getWhereReinsert(), pEH->getCurrent());
                            pEHParent->setCurrent(pTL);
                            evalFields.pop_back();
                            delete pEH;
                            continue;
                        }
                        else
                        {
                            if (pTL->exists(pEH->getExpAsString()))
                            {
                                pTL = pTL->set(pEH->getExpAsString(), pEH->getCurrent());
                                pEHParent->setCurrent(pTL);
                                evalFields.pop_back();
                                delete pEH;
                                continue;
                            }

                            pParentArgs = new types::typed_list();
                            pParentArgs->push_back(new types::String(pEH->getExpAsString().c_str()));
                        }
                    }
                    else if (pParent->isCell())
                    {
                        types::Cell* pCell = pParent->getAs<types::Cell>();
                        if (pEHParent->isCellExp() && pEH->getWhereReinsert() != -1)
                        {
                            // a{x}.b => reinsert b in a{x}
                            pCell->set(pEH->getWhereReinsert(), pEH->getCurrent());
                            pEHParent->setReinsertion();
                            evalFields.pop_back();
                            delete pEH;
                            continue;
                        }
                    }
                }

                types::InternalType* pIT = insertionCall(*_pExp, pParentArgs, pEHParent->getCurrent(), pEH->getCurrent());
                if (pIT == NULL)
                {
                    std::wostringstream os;
                    os << _W("Submatrix incorrectly defined.\n");
                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                }

                pEHParent->setCurrent(pIT);
                if (pEHParent->getArgs() == NULL)
                {
                    delete pParentArgs;
                }
            }

            if (pEH->getCurrent())
            {
                pEH->getCurrent()->killMe();
            }

            evalFields.pop_back();
            delete pEH;
        }

        if (bPutInCtx)
        {
            pITMain->DecreaseRef();
            ctx->put(spMainExp->getStack(), pITMain);
        }

        if (!evalFields.empty())
        {
            for (std::list<ExpHistory*>::const_iterator i = evalFields.begin(), end = evalFields.end(); i != end; i++)
            {
                delete *i;
            }
        }

        return pITMain;
    }
    catch (const ast::InternalError error)
    {
        if (bPutInCtx)
        {
            pITMain->DecreaseRef();
        }

        for (std::list<ExpHistory*>::reverse_iterator i = workFields.rbegin(); i != workFields.rend(); ++i)
        {
            (*i)->setDeleteCurrent(true);
            delete *i;
        }

        for (std::list<ExpHistory*>::reverse_iterator i = evalFields.rbegin(); i != evalFields.rend(); ++i)
        {
            (*i)->setDeleteCurrent(true);
            delete *i;
        }

        throw error;
    }
}

types::InternalType* insertionCall(const ast::Exp& e, types::typed_list* _pArgs, types::InternalType* _pVar, types::InternalType* _pInsert)
{
    types::InternalType* pOut = NULL;
    types::InternalType *pIL = NULL;
    //fisrt extract implicit list
    if (_pInsert->isColon())
    {
        //double* pdbl = NULL;
        //_pInsert = new Double(-1, -1, &pdbl);
        //pdbl[0] = 1;
        pIL = types::Double::Identity(-1, -1);
        _pInsert->killMe();
        _pInsert = pIL;
    }
    else if (_pInsert->isImplicitList())
    {
        pIL = _pInsert->getAs<types::ImplicitList>()->extractFullMatrix();
        if (pIL && pIL->isDeletable())
        {
            _pInsert->killMe();
            _pInsert = pIL;
        }
    }
    else if (_pInsert->isContainer() && _pInsert->isRef())
    {
        //std::cout << "assign container type during insertion" << std::endl;
        //types::InternalType* pIL = _pInsert->clone();
        //_pInsert = pIL;
    }

    if (_pInsert->isDouble() && _pInsert->getAs<types::Double>()->isEmpty() && _pVar == NULL)
    {
        // l(x) = [] when l is not defined => create l = []
        pOut = types::Double::Empty();
    }
    else if (_pInsert->isDouble() && _pInsert->getAs<types::Double>()->isEmpty() && _pVar->isStruct() == false && _pVar->isList() == false)
    {
        //insert [] so deletion except for Struct and List which can insert []
        if (_pVar->isHandle())
        {
            types::GraphicHandle* pH = _pVar->getAs<types::GraphicHandle>();
            if ((*_pArgs)[0]->isString())
            {
                types::String *pS = (*_pArgs)[0]->getAs<types::String>();

                types::typed_list in;
                types::typed_list out;
                types::optional_list opt;

                in.push_back(pH);
                in.push_back(pS);
                in.push_back(_pInsert);

                types::Function* pCall = (types::Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"set"));
                types::Callable::ReturnValue ret = pCall->call(in, opt, 1, out);
                if (ret == types::Callable::OK)
                {
                    pOut = _pVar;
                }
            }
            else
            {
                pOut = pH->insert(_pArgs, _pInsert);
            }
        }
        else if (_pVar->isStruct())
        {
            pOut = _pVar->getAs<types::Struct>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUserType())
        {
            pOut = _pVar->getAs<types::UserType>()->insert(_pArgs, _pInsert);
        }
        else if (_pInsert->isGenericType() && (_pInsert->isTList() == false &&  _pInsert->isMList() == false))
        {
            pOut = _pVar->getAs<types::GenericType>()->remove(_pArgs);
        }
        else
        {
            //overload !
            pOut = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
        }
    }
    else if (_pVar == NULL || (_pVar->isDouble() && _pVar->getAs<types::Double>()->getSize() == 0))
    {
        //insert in a new variable or []
        //call static insert function
        //if _pVar == NULL and pArg is single string, it's a struct creation
        if ((*_pArgs)[0]->isString())
        {
            types::String *pS = (*_pArgs)[0]->getAs<types::String>();
            types::Struct* pStr = new types::Struct(1, 1);

            if (_pArgs->size() != 1 || pS->isScalar() == false)
            {
                if (pIL)
                {
                    pIL->killMe();
                }
                //manage error
                std::wostringstream os;
                os << _W("Invalid Index.\n");
                throw ast::InternalError(os.str(), 999, e.getLocation());
            }

            pStr->addField(pS->get(0));
            pStr->get(0)->set(pS->get(0), _pInsert);
            pOut = pStr;
        }
        else
        {
            if (_pInsert->isGenericType() && (_pInsert->isTList() == false &&  _pInsert->isMList() == false))
            {
                pOut = _pInsert->getAs<types::GenericType>()->insertNew(_pArgs);
            }
            else
            {
                // overload
                types::Double* pEmpty = types::Double::Empty();
                pOut = callOverload(e, L"i", _pArgs, _pInsert, pEmpty);
                pEmpty->killMe();
            }
        }
    }
    else
    {
        //call type insert function
        types::InternalType* pRet = NULL;

        // case m=x; m()=x;
        if (_pArgs == NULL || _pArgs->size() == 0)
        {
            std::wostringstream os;
            os << _W("Wrong insertion : Cannot insert without arguments.");
            throw ast::InternalError(os.str(), 999, e.getLocation());
        }

        //check types compatibilties
        if (_pVar->isDouble() && _pInsert->isSparse())
        {
            types::Sparse* pSp = _pInsert->getAs<types::Sparse>();
            types::Double* pD = new types::Double(pSp->getRows(), pSp->getCols(), pSp->isComplex());
            pSp->fill(*pD);
            pRet = _pVar->getAs<types::GenericType>()->insert(_pArgs, pD);
            delete pD;
        }
        else if (_pVar->isSparse() && _pInsert->isDouble())
        {
            pRet = _pVar->getAs<types::GenericType>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isSparseBool() && _pInsert->isBool())
        {
            pRet = _pVar->getAs<types::GenericType>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isDouble() && _pInsert->isPoly())
        {
            types::Double* pDest = _pVar->getAs<types::Double>();
            types::Polynom* pIns = _pInsert->getAs<types::Polynom>();
            int iSize = pDest->getSize();
            int* piRanks = new int[iSize];
            memset(piRanks, 0x00, iSize * sizeof(int));
            types::Polynom* pP = new types::Polynom(pIns->getVariableName(), pDest->getDims(), pDest->getDimsArray(), piRanks);
            delete[] piRanks;
            pP->setComplex(pDest->isComplex());

            if (pP->isComplex())
            {
                for (int idx = 0 ; idx < pP->getSize() ; idx++)
                {
                    double dblR = pDest->get(idx);
                    double dblI = pDest->getImg(idx);
                    pP->get(idx)->setCoef(&dblR, &dblI);
                }
            }
            else
            {
                for (int idx = 0 ; idx < pP->getSize() ; idx++)
                {
                    double dblR = pDest->get(idx);
                    pP->get(idx)->setCoef(&dblR, NULL);
                }
            }

            pRet = pP->insert(_pArgs, pIns);
        }
        else if (_pVar->isPoly() && _pInsert->isDouble())
        {
            types::Polynom* pDest = _pVar->getAs<types::Polynom>();
            types::Double* pIns = _pInsert->getAs<types::Double>();
            bool isComplexIns = pIns->isComplex();
            int iSize = pIns->getSize();
            int* piRanks = new int[iSize];
            memset(piRanks, 0x00, iSize * sizeof(int));

            //create a new polynom with Double to insert it into dest polynom
            types::Polynom* pP = new types::Polynom(pDest->getVariableName(), pIns->getDims(), pIns->getDimsArray(), piRanks);
            delete[] piRanks;

            if (isComplexIns)
            {
                double* pR = pIns->get();
                double* pI = pIns->getImg();
                types::SinglePoly** pSP = pP->get();
                for (int idx = 0 ; idx < pP->getSize() ; idx++)
                {
                    double dblR = pR[idx];
                    double dblI = pI[idx];
                    pSP[idx]->setComplex(true);
                    pSP[idx]->setCoef(&dblR, &dblI);
                }
            }
            else
            {
                double* pdblR = pIns->get();
                types::SinglePoly** pSP = pP->get();
                for (int idx = 0 ; idx < pP->getSize() ; idx++)
                {
                    double dblR = pdblR[idx];
                    pSP[idx]->setCoef(&dblR, NULL);
                }
            }

            pRet = pDest->insert(_pArgs, pP);
            pP->killMe();
        }
        else if (_pVar->isStruct())
        {
            types::Struct* pStruct = _pVar->getAs<types::Struct>();
            // insert something in a field of a struct
            if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
            {
                //s("x") = y
                types::String *pS = (*_pArgs)[0]->getAs<types::String>();
                if (pS->isScalar() == false)
                {
                    if (pIL)
                    {
                        pIL->killMe();
                    }
                    //manage error
                    std::wostringstream os;
                    os << _W("Invalid Index.\n");
                    throw ast::InternalError(os.str(), 999, e.getLocation());
                }

                if (_pInsert->isListDelete())
                {
                    /* Remove a field */
                    pStruct = pStruct->removeField(pS->get(0));
                }
                else
                {
                    /* Add a field */
                    pStruct = pStruct->addField(pS->get(0));
                    for (int i = 0; i < pStruct->getSize(); i++)
                    {
                        pStruct->get(i)->set(pS->get(0), _pInsert);
                    }
                }
                pRet = pStruct;
            }
            else // insert something in a struct
            {
                if (_pInsert->isStruct())
                {
                    types::String* pStrFieldsName = pStruct->getFieldNames();
                    types::Struct* pStructInsert = _pInsert->clone()->getAs<types::Struct>();
                    types::String* pStrInsertFieldsName = pStructInsert->getFieldNames();
                    types::Struct* pStructRet = NULL;

                    // if not an empty struct
                    if (pStrFieldsName)
                    {
                        // insert fields of pStruct in pStructInsert
                        for (int i = pStrFieldsName->getSize(); i > 0; i--)
                        {
                            if (pStructInsert->exists(pStrFieldsName->get(i - 1)) == false)
                            {
                                pStructInsert->addFieldFront(pStrFieldsName->get(i - 1));
                            }
                            else
                            {
                                std::wstring pwcsField = pStrFieldsName->get(i - 1);
                                types::List* pLExtract = pStructInsert->extractFieldWithoutClone(pwcsField);

                                for (int i = 0; i < pLExtract->getSize(); i++)
                                {
                                    // protect element wich are not cloned before call removeField.
                                    pLExtract->get(i)->IncreaseRef();
                                }

                                pStructInsert->removeField(pwcsField);
                                pStructInsert->addFieldFront(pwcsField);

                                for (int i = 0; i < pLExtract->getSize(); i++)
                                {
                                    // set elements in the new position
                                    pStructInsert->get(i)->set(pwcsField, pLExtract->get(i));
                                    pLExtract->get(i)->DecreaseRef();
                                }

                                pLExtract->killMe();
                            }
                        }

                        pStrFieldsName->killMe();
                    }

                    // insert elements in following pArgs
                    pRet = pStruct->insert(_pArgs, pStructInsert);
                    pStructRet = pRet->getAs<types::Struct>();

                    pStructInsert->killMe();

                    // insert fields of pStructInsert in pRet
                    for (int i = 0; i < pStrInsertFieldsName->getSize(); i++)
                    {
                        if (pStructRet->exists(pStrInsertFieldsName->get(i)) == false)
                        {
                            pStructRet->addField(pStrInsertFieldsName->get(i));
                        }
                    }

                    pStrInsertFieldsName->killMe();
                }
                else
                {
                    pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                }
            }
        }
        else if (_pVar->isTList() || _pVar->isMList())
        {
            types::TList* pTL = _pVar->getAs<types::TList>();
            if (_pArgs->size() == 1)
            {
                if ((*_pArgs)[0]->isString())
                {
                    //s("x") = y
                    types::String *pS = (*_pArgs)[0]->getAs<types::String>();
                    if (pS->isScalar() == false)
                    {
                        if (pIL)
                        {
                            pIL->killMe();
                        }

                        //manage error
                        std::wostringstream os;
                        os << _W("Invalid Index.\n");
                        throw ast::InternalError(os.str(), 999, e.getLocation());
                    }

                    if (_pInsert->isListDelete())
                    {
                        return callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                    }

                    if (pTL->exists(pS->get(0)))
                    {
                        pRet = pTL->set(pS->get(0), _pInsert);
                    }
                    else
                    {
                        return callOverload(e, L"i", _pArgs, _pInsert, _pVar);

                        //ExecVisitor exec;
                        //typed_list in;
                        //typed_list out;
                        //std::wstring function_name = L"%l_e";

                        //_pInsert->IncreaseRef();
                        //in.push_back(_pInsert);

                        //Overload::call(function_name, in, 1, out, &exec);
                        //_pInsert->DecreaseRef();

                        //if (out.size() != 0)
                        //{
                        //    pRet = in[0];
                        //}
                    }
                }
                else
                {
                    // s(x)
                    if (_pVar->isMList())
                    {
                        pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                    }
                    else
                    {
                        // In case where pTL is in several scilab variable,
                        // we have to clone it for keep the other variables unchanged.
                        if (pTL->getRef() > 1)
                        {
                            pTL = pTL->clone()->getAs<types::TList>();
                        }

                        pRet = pTL->insert(_pArgs, _pInsert);

                        // If we have inserted something else than a String
                        // in the first element, the TList have to be a List.
                        if (pTL->get(0)->isString() == false)
                        {
                            types::List* pL = new types::List();
                            for (int i = 0; i < pTL->getSize(); i++)
                            {
                                pL->append(pTL->get(i));
                            }

                            pTL->killMe();
                            pRet = pL;
                        }
                    }
                }
            }
            else
            {
                if (_pVar->isMList())
                {
                    pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                }
                else
                {
                    // call the overload if it exists.
                    pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                    if (pRet == NULL)
                    {
                        // else normal insert
                        pRet = pTL->insert(_pArgs, _pInsert);
                    }
                }
            }
        }
        else if (_pVar->isList())
        {
            types::List* pL = NULL;
            // In case where pL is in several scilab variable,
            // we have to clone it for keep the other variables unchanged.
            if (_pVar->getRef() > 1)
            {
                pL = _pVar->clone()->getAs<types::List>();
                pRet = pL->insert(_pArgs, _pInsert);
                if (pRet == NULL)
                {
                    pL->killMe();
                    // call overload
                    pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                }
            }
            else
            {
                pL = _pVar->getAs<types::List>();
                pRet = pL->insert(_pArgs, _pInsert);
                if (pRet == NULL)
                {
                    // call overload
                    pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                }
            }
        }
        else if (_pVar->isHandle())
        {
            if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
            {
                //s(["x"])
                types::GraphicHandle* pH = _pVar->getAs<types::GraphicHandle>();
                types::String *pS = (*_pArgs)[0]->getAs<types::String>();
                types::typed_list in;
                types::typed_list out;
                types::optional_list opt;

                in.push_back(pH);
                in.push_back(pS);
                in.push_back(_pInsert);

                types::Function* pCall = (types::Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"set"));
                if (pCall)
                {
                    types::Callable::ReturnValue ret = pCall->call(in, opt, 1, out);
                    if (ret == types::Callable::OK)
                    {
                        pRet = _pVar;
                    }
                    else
                    {
                        throw ast::InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
                    }
                }
            }
            else
            {
                pRet = _pVar->getAs<types::GraphicHandle>()->insert(_pArgs, _pInsert);
            }
        }
        else if (_pVar->isUserType())
        {
            for (int i = 0; i < _pArgs->size(); i++)
            {
                if ((*_pArgs)[i]->isImplicitList())
                {
                    types::ImplicitList* pIL = (*_pArgs)[i]->getAs<types::ImplicitList>();
                    if (pIL->isComputable())
                    {
                        types::InternalType* pIT = pIL->extractFullMatrix();
                        (*_pArgs)[i]->killMe();
                        (*_pArgs)[i] = pIT;
                    }
                }
            }

            pRet = _pVar->getAs<types::UserType>()->insert(_pArgs, _pInsert);
            if (pRet == NULL)
            {
                pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
            }
        }
        else if (_pVar->getType() == _pInsert->getType())
        {
            pRet = _pVar->getAs<types::GenericType>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isCell())
        {
            if (_pInsert->isCell() == false)
            {
                //manage error
                std::wostringstream os;
                os << _W("Wrong insertion: A Cell expected: use {...} instead of (...).\n");
                throw ast::InternalError(os.str(), 999, e.getLocation());
            }
        }
        else
        {
            // overload
            pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
        }

        pOut = pRet;
    }

    if (pIL)
    {
        pIL->killMe();
    }

    return pOut;
}

void callOnPrompt(void)
{
    static symbol::Variable* onPrompt = NULL;
    if (onPrompt == NULL)
    {
        onPrompt = symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"%onprompt"));
    }

    types::InternalType* pOnPrompt = NULL;
    pOnPrompt = onPrompt->get();
    if (pOnPrompt != NULL && pOnPrompt->isCallable())
    {
        StoreConsoleCommand("%onprompt()", 1);
    }
}

ast::Exp* callTyper(ast::Exp* _tree, std::wstring _msg)
{
    ast::Exp* newTree = NULL;
    unsigned char *newast = NULL;
    ast::SerializeVisitor* s = new ast::SerializeVisitor(_tree);
    ast::DeserializeVisitor* d = NULL;

    if (_msg.empty())
    {
        unsigned char* astbin = s->serialize();
        //call ocamlpro typer
        //char *newast = ocamlpro_typer(astbin);
        //free(astbin);

        //for debug
        newast = astbin;

        d = new ast::DeserializeVisitor(newast);
        newTree = d->deserialize();
    }
    else
    {
        std::wstring msgS(_msg + L" serialize");
        std::wstring msgD(_msg + L" deserialize");

        Timer timer;
        timer.start();
        unsigned char* astbin = s->serialize();
        timer.check(msgS.c_str());

        //call ocamlpro typer
        //char *newast = ocamlpro_typer(astbin);
        //free(astbin);

        //for debug
        newast = astbin;

        timer.start();
        d = new ast::DeserializeVisitor(newast);
        newTree = d->deserialize();
        timer.check(msgD.c_str());
    }

    free(newast);
    delete s;
    delete d;
    return newTree;
}

std::string printExp(std::ifstream& _File, ast::Exp* _pExp, const std::string& _stPrompt, int* _piLine /* in/out */, int* _piCol /* in/out */, std::string& _stPreviousBuffer)
{
    Location loc = _pExp->getLocation();

    //get current line
    //for multiple expression on same line
    //_stPreviousBuffer will not be updated

    //bypass previous lines
    for (int i = *_piLine; i < loc.first_line - 1; i++)
    {

        (*_piLine)++;
        if ((*_piLine) != (loc.first_line - 1))
        {
            //empty line
            if (ConfigVariable::isPrintCompact() == false)
            {
                printLine("", "", true);
            }
        }
        std::getline(_File, _stPreviousBuffer);
    }

    if (loc.first_line == loc.last_line)
    {
        //expression on 1-line
        int iStart = loc.first_column - 1;
        int iEnd = loc.last_column - 1;
        int iLen = iEnd - iStart;

        int iCopyLen = iEnd - *_piCol;
        std::string strLastLine(_stPreviousBuffer.c_str() + *_piCol, iCopyLen);
        int iExpLen = iLen;
        int iLineLen = (int)_stPreviousBuffer.size();
        bool bStart = iStart == 0 || *_piCol == 0;
        bool bEnd = iStart + iExpLen == iLineLen;

        //begin of line
        if (bStart)
        {
            if (bEnd)
            {
                printLine(_stPrompt, strLastLine, true);
                *_piCol = 0;
            }
            else
            {
                printLine(_stPrompt, strLastLine, false);
                *_piCol = loc.last_column - 1;
            }
        }
        else //middle of line
        {
            if (bEnd)
            {
                printLine("", strLastLine, true);
                *_piCol = 0;
            }
            else
            {
                printLine("", strLastLine, false);
                *_piCol = loc.last_column - 1;
            }
        }
    }
    else
    {
        //multiline
        int iStart = loc.first_column - 1;
        bool bStart = iStart == 0 || *_piCol == 0;

        std::string strLastLine(_stPreviousBuffer.c_str() + *_piCol);

        //begin of line
        if (bStart)
        {
            printLine(_stPrompt, strLastLine, true);
        }
        else
        {
            printLine("", strLastLine, true);
        }

        bool isCompact = ConfigVariable::isPrintCompact();
        ConfigVariable::setPrintCompact(true);

        //full lines
        for (int i = loc.first_line; i < (loc.last_line - 1); i++)
        {
            (*_piLine)++;
            std::getline(_File, _stPreviousBuffer);
            // dont print empty line of function body
            if (_stPreviousBuffer.size() != 0)
            {
                printLine(_stPrompt, _stPreviousBuffer.c_str(), true);
            }
        }

        //last line
        std::getline(_File, _stPreviousBuffer);
        (*_piLine)++;

        int iSize = loc.last_column - 1;
        std::string stLastLine(_stPreviousBuffer.c_str(), iSize);
        int iLineLen = (int)_stPreviousBuffer.size();
        if (iLineLen == iSize)
        {
            printLine(_stPrompt, stLastLine, true);
            *_piCol = 0;
        }
        else
        {
            printLine(_stPrompt, stLastLine, false);
            *_piCol = loc.last_column - 1;
        }

        ConfigVariable::setPrintCompact(isCompact);
    }

    return _stPreviousBuffer;
}

void printLine(const std::string& _stPrompt, const std::string& _stLine, bool _bLF)
{
    std::string st;
    int size = _stPrompt.size();
    if (size && ConfigVariable::isPrintCompact() == false)
    {
        st = "\n";
    }

    st += _stPrompt;

    st += _stLine;
    if (_bLF)
    {
        st += "\n";
    }

    scilabWrite(st.c_str());
}
/*--------------------------------------------------------------------------*/
