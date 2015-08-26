/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string>
#include <numeric>
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
InternalType* allocDest(InternalType* _poSource, int _iRows, int _iCols)
{
    InternalType* poResult = NULL;
    switch (_poSource->getType())
    {
        case GenericType::ScilabDouble :
            poResult = new Double(_iRows, _iCols, false);
            break;
        case GenericType::ScilabBool :
            poResult = new Bool(_iRows, _iCols);
            break;
        case GenericType::ScilabInt8 :
            poResult = new Int8(_iRows, _iCols);
            break;
        case GenericType::ScilabUInt8 :
            poResult = new UInt8(_iRows, _iCols);
            break;
        case GenericType::ScilabInt16 :
            poResult = new Int16(_iRows, _iCols);
            break;
        case GenericType::ScilabUInt16 :
            poResult = new UInt16(_iRows, _iCols);
            break;
        case GenericType::ScilabInt32 :
            poResult = new Int32(_iRows, _iCols);
            break;
        case GenericType::ScilabUInt32 :
            poResult = new UInt32(_iRows, _iCols);
            break;
        case GenericType::ScilabInt64 :
            poResult = new Int64(_iRows, _iCols);
            break;
        case GenericType::ScilabUInt64 :
            poResult = new UInt64(_iRows, _iCols);
            break;
        case GenericType::ScilabString :
            poResult = new String(_iRows, _iCols);
            break;
        case GenericType::ScilabPolynom :
        {
            int* piRank = new int[_iRows * _iCols];
            memset(piRank, 0x00, _iRows * _iCols * sizeof(int));
            poResult = new Polynom(_poSource->getAs<Polynom>()->getVariableName(), _iRows, _iCols, piRank);
            delete[] piRank;
            break;
        }
        case InternalType::ScilabImplicitList :
            poResult = new ImplicitList();
            break;
        default :
            // FIXME : What should we do here ??
            break;
    }
    return poResult;
}

InternalType* AddElementToVariableFromCol(InternalType* _poDest, InternalType* _poSource, int _iRows, int _iCols, int *_piCols)
{
    InternalType *poResult = NULL;
    InternalType::ScilabType TypeSource = _poSource->getType();
    InternalType::ScilabType TypeDest = InternalType::ScilabInternal;

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
            case GenericType::ScilabDouble :
                if (poResult->getAs<Double>()->isComplex() == false && _poSource->getAs<Double>()->isComplex() == true)
                {
                    poResult->getAs<Double>()->setComplex(true);
                }

                poResult->getAs<Double>()->fillFromCol(*_piCols, _poSource->getAs<Double>());
                *_piCols += _poSource->getAs<Double>()->getCols();

                break;
            default:
                break;
        }
        return poResult;
    }
    return NULL;
}

InternalType* AddElementToVariableFromRow(InternalType* _poDest, InternalType* _poSource, int _iRows, int _iCols, int *_piRows)
{
    InternalType *poResult = NULL;
    InternalType::ScilabType TypeSource = _poSource->getType();
    InternalType::ScilabType TypeDest = InternalType::ScilabInternal;

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
            case GenericType::ScilabDouble :
                if (poResult->getAs<Double>()->isComplex() == false && _poSource->getAs<Double>()->isComplex() == true)
                {
                    poResult->getAs<Double>()->setComplex(true);
                }

                poResult->getAs<Double>()->fillFromRow(*_piRows, _poSource->getAs<Double>());
                *_piRows += _poSource->getAs<Double>()->getRows();

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
InternalType* AddElementToVariable(InternalType* _poDest, InternalType* _poSource, int _iRows, int _iCols)
{
    InternalType *poResult = NULL;
    bool isNew = true;
    InternalType::ScilabType TypeSource = _poSource->getType();
    InternalType::ScilabType TypeDest = InternalType::ScilabInternal;
    int iCurRow = _iRows;
    int iCurCol = _iCols;

    if (_poDest == NULL)
    {
        switch (TypeSource)
        {
            case GenericType::ScilabDouble :
                poResult = new Double(_iRows, _iCols);
                break;
            case GenericType::ScilabBool :
                poResult = new Bool(_iRows, _iCols);
                break;
            case GenericType::ScilabInt8 :
                poResult = new Int8(_iRows, _iCols);
                break;
            case GenericType::ScilabUInt8 :
                poResult = new UInt8(_iRows, _iCols);
                break;
            case GenericType::ScilabInt16 :
                poResult = new Int16(_iRows, _iCols);
                break;
            case GenericType::ScilabUInt16 :
                poResult = new UInt16(_iRows, _iCols);
                break;
            case GenericType::ScilabInt32 :
                poResult = new Int32(_iRows, _iCols);
                break;
            case GenericType::ScilabUInt32 :
                poResult = new UInt32(_iRows, _iCols);
                break;
            case GenericType::ScilabInt64 :
                poResult = new Int64(_iRows, _iCols);
                break;
            case GenericType::ScilabUInt64 :
                poResult = new UInt64(_iRows, _iCols);
                break;
            case GenericType::ScilabString :
                poResult = new String(_iRows, _iCols);
                break;
            case GenericType::ScilabSparse :
                poResult = new Sparse(_iRows, _iCols);
                break;
            case GenericType::ScilabSparseBool :
                poResult = new SparseBool(_iRows, _iCols);
                break;
            case GenericType::ScilabPolynom :
            {
                int* piRank = new int[_iRows * _iCols];
                memset(piRank, 0x00, _iRows * _iCols * sizeof(int));
                poResult = new Polynom(_poSource->getAs<Polynom>()->getVariableName(), _iRows, _iCols, piRank);
                delete[] piRank;
                break;
            }
            case InternalType::ScilabImplicitList :
                poResult = new ImplicitList();
                break;
            case GenericType::ScilabHandle :
                poResult = new GraphicHandle(_iRows, _iCols);
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
            case GenericType::ScilabDouble :
                if (TypeSource == GenericType::ScilabPolynom)
                {
                    Double *poDest = _poDest->getAs<Double>();
                    Polynom* pPSource = _poSource->getAs<Polynom>();

                    //Convert Dest to ScilabPolynom
                    int iSize = poDest->getSize();
                    int *piRank = new int[iSize];
                    memset(piRank, 0x00, iSize * sizeof(int));
                    if (isNew && poResult)
                    {
                        poResult->killMe();
                    }
                    poResult = new Polynom(pPSource->getVariableName(), poDest->getRows(), poDest->getCols(), piRank);
                    delete[] piRank;

                    Polynom* pPResult = poResult->getAs<Polynom>();
                    pPResult->setComplex(poDest->isComplex());

                    double *pR = poDest->getReal();
                    SinglePoly** pSP = pPResult->get();

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
            case GenericType::ScilabPolynom :
                if (TypeSource == GenericType::ScilabDouble)
                {
                    //Add Source like coef of the new element
                    Double* pD = _poSource->getAs<Double>();
                    Polynom* pPolyOut = poResult->getAs<Polynom>();

                    if (pD->isComplex())
                    {
                        pPolyOut->setComplex(true);
                        for (int i = 0 ; i < pD->getRows() ; i++)
                        {
                            for (int j = 0 ; j < pD->getCols() ; j++)
                            {
                                SinglePoly* pSPOut = pPolyOut->get(iCurRow + i, iCurCol + j);

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
                                SinglePoly* pSPOut = pPolyOut->get(iCurRow + i, iCurCol + j);

                                pSPOut->setRank(0);
                                double pDbl = pD->get(i, j);
                                pSPOut->setCoef(&pDbl, NULL);
                            }
                        }
                    }

                    return poResult;
                }
                break;
            case GenericType::ScilabSparse :
                if (TypeSource == GenericType::ScilabDouble)
                {
                    Double* poSource = _poSource->getAs<Double>();
                    Sparse* spResult = poResult->getAs<Sparse>();

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
            case GenericType::ScilabSparseBool :
                if (TypeSource == GenericType::ScilabBool)
                {
                    Bool* poSource = _poSource->getAs<Bool>();
                    SparseBool* spResult = poResult->getAs<SparseBool>();

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
            case GenericType::ScilabDouble :
                poResult->getAs<Double>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabPolynom :
                poResult->getAs<Polynom>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabBool:
                poResult->getAs<Bool>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabInt8 :
                poResult->getAs<Int8>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabUInt8 :
                poResult->getAs<UInt8>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabInt16 :
                poResult->getAs<Int16>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabUInt16 :
                poResult->getAs<UInt16>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabInt32 :
                poResult->getAs<Int32>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabUInt32 :
                poResult->getAs<UInt32>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabInt64 :
                poResult->getAs<Int64>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabUInt64 :
                poResult->getAs<UInt64>()->append(iCurRow, iCurCol, _poSource);
                break;
            case GenericType::ScilabSparse :
                poResult->getAs<Sparse>()->append(iCurRow, iCurCol, _poSource->getAs<Sparse>());
                break;
            case GenericType::ScilabSparseBool :
                poResult->getAs<SparseBool>()->append(iCurRow, iCurCol, _poSource->getAs<SparseBool>());
                break;
            case GenericType::ScilabString :
            {
                poResult->getAs<String>()->append(iCurRow, iCurCol, _poSource);
            }
            break;
            case GenericType::ScilabImplicitList :
            {
                ImplicitList* pIL = _poSource->getAs<ImplicitList>();
                ImplicitList* pOL = poResult->getAs<ImplicitList>();
                pOL->setStart(pIL->getStart());
                pOL->setStep(pIL->getStep());
                pOL->setEnd(pIL->getEnd());
                break;
            }
            case GenericType::ScilabHandle :
                poResult->getAs<GraphicHandle>()->append(iCurRow, iCurCol, _poSource);
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
        os << _W("Unknow expression");
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
InternalType* callOverload(const ast::Exp& e, std::wstring _strType, typed_list* _pArgs, InternalType* _source, InternalType* _dest)
{
    Function::ReturnValue ret = Function::Error;
    InternalType* pITOut = NULL;
    typed_list in;
    typed_list out;

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

    InternalType* pFunc = symbol::Context::getInstance()->get(symbol::Symbol(function_name));
    // if %type_6 doesn't exist, call %l_6
    if (_dest == NULL && pFunc == NULL)
    {
        function_name = L"%l_" + _strType;
    }

    // For insertion in TList, call normal insertion if overload doesn't exits
    if ((_dest  && _dest->isTList() && pFunc == NULL) == false || _source->isListDelete())
    {
        ast::ExecVisitor exec;

        try
        {
            ret = Overload::call(function_name, in, 1, out, &exec);
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

        if (ret == Function::Error)
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
        List* pListOut = new List();
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
        List* pList = NULL;
        int iListIncr = 0;
        int iListSize = 0;

        ast::ExecVisitor execMe;
        ast::exps_t args = pCall->getArgs();
        typed_list* pCurrentArgs = execMe.GetArgumentList(args);

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
            pList = (*pCurrentArgs)[0]->getAs<List>();
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
                    (*pCurrentArgs)[0]->getAs<String>()->getSize() == 1)
            {
                // a("b") => a.b or a(x)("b") => a(x).b
                ExpHistory * pEHParent = fields.back();
                ast::SimpleVar* pFieldVar = new ast::SimpleVar(pCall->getLocation(), symbol::Symbol((*pCurrentArgs)[0]->getAs<String>()->get(0)));
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
                    pCurrentArgs = new typed_list();
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

InternalType* evaluateFields(const ast::Exp* _pExp, std::list<ExpHistory*>& fields, InternalType* _pAssignValue)
{
    std::list<ExpHistory*> evalFields;
    std::list<ExpHistory*> workFields;

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

        InternalType* pIT = ctx->getCurrentLevel(pFirstField->getExp()->getSymbol());

        if (pIT == NULL)
        {
            // check if we not redefined a protected variable. (ie: sin(2) = 12 without redefine sin before)
            symbol::Variable* var = ctx->getOrCreate(pFirstField->getExp()->getSymbol());
            if (var->empty() == false && var->top()->m_iLevel == 0)
            {
                std::wostringstream os;
                os << _W("Unexpected redefinition of Scilab function or variable.");
                throw ast::InternalError(os.str(), 999, _pExp->getLocation());
            }

            if (pFirstField->isCellExp())
            {
                // a{x}, where "a" doesn't exists
                pIT = new Cell(1, 1);
                ctx->put(pFirstField->getExp()->getStack(), pIT);
            }
            else if (fields.size() > 1)
            {
                // is a field exp
                //"a" does not exist or it is another type, create it with size 1,1 and return it
                //create new structure variable
                pIT = new Struct(1, 1);
                ctx->put(pFirstField->getExp()->getStack(), pIT);
            }
            // else
            // is a call exp
            // a(x) = "something" and a does not exist
            // a will be create in insertionCall
        }
        else if (pIT->getRef() > 1 && pIT->isHandle() == false)
        {
            pIT = pIT->clone();
            ctx->put(pFirstField->getExp()->getStack(), pIT);
        }
        else if (pIT == _pAssignValue)
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
                                            pIT));

        //*** evaluate fields ***//
        while (iterFields != fields.end())
        {
            ExpHistory* pEH = workFields.front();
            evalFields.push_back(pEH);
            workFields.pop_front();

            InternalType* pITCurrent = pEH->getCurrent();

            if (pEH->isCellExp() && pITCurrent->isCell() == false)
            {
                std::wostringstream os;
                os << _W("Wrong insertion : use extraction with {} only on a Cell.");
                throw ast::InternalError(os.str(), 999, _pExp->getLocation());
            }

            if (pITCurrent->isStruct())
            {
                Struct* pStruct = pITCurrent->getAs<Struct>();
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
                    pStruct->resize(pEH->getArgsDimsArray(), pEH->getArgsDims());
                }

                // create field in parent if it not exist
                if (pStruct->exists(pwcsFieldname) == false)
                {
                    pStruct->addField(pwcsFieldname);
                }

                if (pEH->getArgs())
                {
                    InternalType* pIT = pStruct->extractWithoutClone(pEH->getArgs());
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
                        typed_list* args = new typed_list();
                        args->push_back(new String(pwcsFieldname.c_str()));
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
                        List* pLOut = pStruct->extractFieldWithoutClone(pwcsFieldname);

                        // pStruct must be scalar because we cant insert most of one element in the same insertion
                        InternalType* pIT = pLOut->get(0);
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
                TList* pTL = pITCurrent->getAs<TList>();
                typed_list* pArgs = pEH->getArgs();
                if (pArgs)
                {
                    if (pArgs->size() > 1 || pITCurrent->isMList())
                    {
                        // call overload
                        InternalType* pExtract = callOverload(*pEH->getExp(), L"6", pArgs, pTL, NULL);
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
                            pTL->set(iNewSize - 1, new ListUndefined());
                        }

                        // update pArgs variables with new argument computed in getSizeFromArgs
                        pArgs = pEH->getArgs();

                        InternalType* pIT = pTL->extract(pArgs);
                        List* pList = pIT->getAs<List>();

                        if (pList->getSize() > 1)
                        {
                            pList->killMe();
                            std::wostringstream os;
                            os << _W("Unable to insert multiple item in a List.");
                            throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                        }

                        double* pdblArgs = (*pArgs)[0]->getAs<Double>()->get();
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
                    InternalType* pExtract = NULL;
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
                        typed_list* args = new typed_list();
                        args->push_back(new String(pwcsFieldname.c_str()));
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
                                pArgs = new typed_list();
                                pArgs->push_back(new String(pwcsFieldname.c_str()));

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
                List* pL = pITCurrent->getAs<List>();
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
                            InternalType* pExtract = callOverload(*pEH->getExp(), L"6", pEH->getArgs(), pL, NULL);

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
                                pL->set(iNewSize - 1, new ListUndefined());
                            }

                            Double* pDblArgs = (*pEH->getArgs())[0]->getAs<Double>();
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
                        typed_list* args = new typed_list();
                        args->push_back(new String(pwcsFieldname.c_str()));
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
                            InternalType* pExtract = callOverload(*pEH->getExp(), L"6", args, pL, NULL);

                            // append extraction of a.x for next level.
                            workFields.push_back(new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                            workFields.back()->setReinsertion();
                        }
                    }
                }
            }
            else if (pITCurrent->isHandle())
            {
                typed_list* pArgs = pEH->getArgs();
                GraphicHandle* pGH = pITCurrent->getAs<GraphicHandle>();
                if (pArgs)
                {
                    InternalType* pExtract = NULL;

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
                    typed_list* args = new typed_list();
                    args->push_back(new String(pwcsFieldname.c_str()));
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
                        InternalType* pExtract = callOverload(*pEH->getExp(), L"e", args, pITCurrent, NULL);

                        // append extraction of a.x for next level.
                        workFields.push_back(new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                        workFields.front()->setReinsertion();
                    }
                }
            }
            else if (pITCurrent->isCell())
            {
                Cell* pCell = pITCurrent->getAs<Cell>();
                if (pEH->getArgs() && (*pEH->getArgs())[0]->isString() == false)
                {
                    if (pEH->isCellExp())
                    {
                        // a{x} => extract like a(x){[1 2 ...]}
                        if (pEH->getParent() && pEH->getLevel() == pEH->getParent()->getLevel())
                        {
                            // extract each elements of a(x)
                            for (int iCell = 0; iCell < pCell->getSize(); iCell++)
                            {
                                InternalType* pIT = pCell->get(iCell);
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
                            if (pEH->needResize())
                            {
                                if (pEH->getArgsDims() == 1)
                                {
                                    std::wostringstream os;
                                    os << _W("Invalid index.");
                                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                                }

                                // resize current Cell
                                pCell->resize(pEH->getArgsDimsArray(), pEH->getArgsDims());
                            }

                            InternalType* pIT = pCell->extract(pEH->getArgs());
                            workFields.push_front(new ExpHistory(pEH, pEH->getExp(), pEH->getArgs(), pEH->getLevel(), pEH->isCellExp(), pIT));
                            workFields.front()->setReinsertion();
                        }
                    }
                    else
                    {
                        if ((*iterFields)->isCellExp())
                        {
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
                                pCell->resize(pEH->getArgsDimsArray(), pEH->getArgsDims());
                            }

                            InternalType* pIT = pCell->extract(pEH->getArgs());
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
                    InternalType* pExtract = pITCurrent->getAs<UserType>()->extract(pEH->getArgs());
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
                    typed_list* args = new typed_list();
                    args->push_back(new String(pwcsFieldname.c_str()));
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
                        InternalType* pExtract = pITCurrent->getAs<UserType>()->extract(args);
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
                std::wostringstream os;
                os << _W("Wrong insertion : function or macro are not expected.");
                throw ast::InternalError(os.str(), 999, _pExp->getLocation());
            }
            else
            {
                InternalType* pIT = new Struct(1, 1);
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

            typed_list* pArgs = pEH->getArgs();

            // should never occured
            if (pArgs == NULL)
            {
                std::wostringstream os;
                os << _W("evaluateFields : Cannot insert without arguments.");
                throw ast::InternalError(os.str(), 999, _pExp->getLocation());
            }

            if (pEH->isCellExp())
            {
                Cell* pCell = pEH->getCurrent()->getAs<Cell>();
                // insert "something" in b{x}
                if ((*pArgs)[0]->isString())
                {
                    std::wostringstream os;
                    os << _W("Wrong insertion in a Cell.");
                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                }

                pCell->insertCell(pArgs, _pAssignValue);
            }
            else
            {
                // insert "something" in b(x,y)
                InternalType* pIT = insertionCall(*_pExp, pArgs, pEH->getCurrent(), _pAssignValue);
                if (pIT == NULL)
                {
                    std::wostringstream os;
                    os << _W("Submatrix incorrectly defined.\n");
                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                }

                if (pEH->setCurrent(pIT))
                {
                    pEH->setReinsertion();
                }
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
                    ctx->put(pEH->getExp()->getStack(), pEH->getCurrent());
                    break;
                }

                typed_list* pParentArgs = pEHParent->getArgs();
                if (pParentArgs == NULL || pEH->getWhereReinsert() != -1)
                {
                    InternalType* pParent = pEHParent->getCurrent();
                    if (pParent->isStruct())
                    {
                        Struct* pStruct = pParent->getAs<Struct>();
                        pStruct->get(pEH->getWhereReinsert())->set(pEH->getExpAsString(), pEH->getCurrent());
                        evalFields.pop_back();
                        delete pEH;
                        continue;
                    }
                    else if (pParent->isTList() || pParent->isMList())
                    {
                        TList* pTL = pParent->getAs<TList>();
                        if (pParentArgs)
                        {
                            // In case where pTL is in several scilab variable,
                            // we have to clone it for keep the other variables unchanged.
                            if (pTL->getRef() > 1)
                            {
                                pTL = pTL->clone()->getAs<TList>();
                            }

                            pTL->set(pEH->getWhereReinsert(), pEH->getCurrent());

                            if (pEH->getParent()->setCurrent(pTL))
                            {
                                pEH->getParent()->setReinsertion();
                                pEH->resetReinsertion();
                            }

                            evalFields.pop_back();
                            delete pEH;
                            continue;
                        }
                        else
                        {
                            if (pTL->exists(pEH->getExpAsString()))
                            {
                                // In case where pTL is in several scilab variable,
                                // we have to clone it for keep the other variables unchanged.
                                if (pTL->getRef() > 1)
                                {
                                    pTL = pTL->clone()->getAs<TList>();
                                }

                                pTL->set(pEH->getExpAsString(), pEH->getCurrent());

                                if (pEH->getParent()->setCurrent(pTL))
                                {
                                    pEH->getParent()->setReinsertion();
                                    pEH->resetReinsertion();
                                }

                                evalFields.pop_back();
                                delete pEH;
                                continue;
                            }

                            pParentArgs = new typed_list();
                            pParentArgs->push_back(new String(pEH->getExpAsString().c_str()));
                        }
                    }
                    else if (pParent->isCell())
                    {
                        Cell* pCell = pParent->getAs<Cell>();
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

                InternalType* pIT = insertionCall(*_pExp, pParentArgs, pEHParent->getCurrent(), pEH->getCurrent());
                if (pIT == NULL)
                {
                    std::wostringstream os;
                    os << _W("Submatrix incorrectly defined.\n");
                    throw ast::InternalError(os.str(), 999, _pExp->getLocation());
                }

                if (pEHParent->setCurrent(pIT))
                {
                    pEHParent->setReinsertion();
                }

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

        if (!evalFields.empty())
        {
            for (std::list<ExpHistory*>::const_iterator i = evalFields.begin(), end = evalFields.end(); i != end; i++)
            {
                delete *i;
            }
        }

        return ctx->getCurrentLevel(pFirstField->getExp()->getSymbol());
    }
    catch (const ast::InternalError error)
    {
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

InternalType* insertionCall(const ast::Exp& e, typed_list* _pArgs, InternalType* _pVar, InternalType* _pInsert)
{
    InternalType* pOut = NULL;
    InternalType *pIL = NULL;
    //fisrt extract implicit list
    if (_pInsert->isColon())
    {
        //double* pdbl = NULL;
        //_pInsert = new Double(-1, -1, &pdbl);
        //pdbl[0] = 1;
        pIL = Double::Identity(-1, -1);
        _pInsert->killMe();
        _pInsert = pIL;
    }
    else if (_pInsert->isImplicitList())
    {
        pIL = _pInsert->getAs<ImplicitList>()->extractFullMatrix();
        if (pIL && pIL->isDeletable())
        {
            _pInsert->killMe();
            _pInsert = pIL;
        }
    }
    else if (_pInsert->isContainer() && _pInsert->isRef())
    {
        //std::cout << "assign container type during insertion" << std::endl;
        //InternalType* pIL = _pInsert->clone();
        //_pInsert = pIL;
    }

    if (_pInsert->isDouble() && _pInsert->getAs<Double>()->isEmpty() && _pVar == NULL)
    {
        // l(x) = [] when l is not defined => create l = []
        pOut = Double::Empty();
    }
    else if (_pInsert->isDouble() && _pInsert->getAs<Double>()->isEmpty() && _pVar->isStruct() == false && _pVar->isList() == false)
    {
        //insert [] so deletion except for Struct and List which can insert []
        InternalType::ScilabType varType = _pVar->getType();
        switch (varType)
        {
            case InternalType::ScilabDouble :
            {
                pOut = _pVar->getAs<Double>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabString :
            {
                pOut = _pVar->getAs<String>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabCell :
            {
                pOut = _pVar->getAs<Cell>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabBool :
            {
                pOut = _pVar->getAs<Bool>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabPolynom :
            {
                pOut = _pVar->getAs<Polynom>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabInt8 :
            {
                pOut = _pVar->getAs<Int8>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabUInt8 :
            {
                pOut = _pVar->getAs<UInt8>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabInt16 :
            {
                pOut = _pVar->getAs<Int16>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabUInt16 :
            {
                pOut = _pVar->getAs<UInt16>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabInt32 :
            {
                pOut = _pVar->getAs<Int32>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabUInt32 :
            {
                pOut = _pVar->getAs<UInt32>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabInt64 :
            {
                pOut = _pVar->getAs<Int64>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabUInt64 :
            {
                pOut = _pVar->getAs<UInt64>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabSparse :
            {
                pOut = _pVar->getAs<Sparse>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabSparseBool :
            {
                pOut = _pVar->getAs<SparseBool>()->remove(_pArgs);
                break;
            }
            case InternalType::ScilabStruct :
            {
                pOut = _pVar->getAs<Struct>()->insert(_pArgs, _pInsert);
                break;
            }
            case InternalType::ScilabHandle :
            {
                GraphicHandle* pH = _pVar->getAs<GraphicHandle>();
                if ((*_pArgs)[0]->isString())
                {
                    String *pS = (*_pArgs)[0]->getAs<String>();

                    typed_list in;
                    typed_list out;
                    optional_list opt;
                    ast::ExecVisitor exec;

                    in.push_back(pH);
                    in.push_back(pS);
                    in.push_back(_pInsert);

                    Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"set"));
                    Callable::ReturnValue ret = pCall->call(in, opt, 1, out, &exec);
                    if (ret == Callable::OK)
                    {
                        pOut = _pVar;
                    }
                }
                else
                {
                    pOut = pH->insert(_pArgs, _pInsert);
                }

                break;
            }
            default :
            {
                //overload !
                pOut = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                break;
            }
        }
    }
    else if (_pVar == NULL || (_pVar->isDouble() && _pVar->getAs<Double>()->getSize() == 0))
    {
        //insert in a new variable or []
        //call static insert function
        //if _pVar == NULL and pArg is single string, it's a struct creation
        if ((*_pArgs)[0]->isString())
        {
            String *pS = (*_pArgs)[0]->getAs<String>();
            Struct* pStr = new Struct(1, 1);

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
            switch (_pInsert->getType())
            {
                case InternalType::ScilabDouble :
                    pOut = Double::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabString :
                    pOut = String::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabCell :
                    pOut = Cell::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabBool :
                    pOut = Bool::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabPolynom :
                    pOut = Polynom::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabInt8 :
                    pOut = Int8::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabUInt8 :
                    pOut = UInt8::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabInt16 :
                    pOut = Int16::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabUInt16 :
                    pOut = UInt16::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabInt32 :
                    pOut = Int32::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabUInt32 :
                    pOut = UInt32::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabInt64 :
                    pOut = Int64::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabUInt64 :
                    pOut = UInt64::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabSparse :
                    pOut = Sparse::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabSparseBool :
                    pOut = SparseBool::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::ScilabHandle:
                    pOut = GraphicHandle::insertNew(_pArgs, _pInsert);
                    break;
                default :
                {
                    // overload
                    Double* pEmpty = Double::Empty();
                    pOut = callOverload(e, L"i", _pArgs, _pInsert, pEmpty);
                    pEmpty->killMe();
                    break;
                }
            }
        }
    }
    else
    {
        //call type insert function
        InternalType* pRet = NULL;

        //check types compatibilties
        if (_pVar->isDouble() && _pInsert->isDouble())
        {
            pRet = _pVar->getAs<Double>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isDouble() && _pInsert->isSparse())
        {
            Sparse* pSp = _pInsert->getAs<Sparse>();
            Double* pD = new Double(pSp->getRows(), pSp->getCols(), pSp->isComplex());
            pSp->fill(*pD);
            pRet = _pVar->getAs<Double>()->insert(_pArgs, pD);
            delete pD;
        }
        else if (_pVar->isString() && _pInsert->isString())
        {
            pRet = _pVar->getAs<String>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isCell() && _pInsert->isCell())
        {
            pRet = _pVar->getAs<Cell>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isBool() && _pInsert->isBool())
        {
            pRet = _pVar->getAs<Bool>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isSparse() && _pInsert->isSparse())
        {
            pRet = _pVar->getAs<Sparse>()->insert(_pArgs, _pInsert->getAs<Sparse>());
        }
        else if (_pVar->isSparse() && _pInsert->isDouble())
        {
            pRet = _pVar->getAs<Sparse>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isSparseBool() && _pInsert->isSparseBool())
        {
            pRet = _pVar->getAs<SparseBool>()->insert(_pArgs, _pInsert->getAs<SparseBool>());
        }
        else if (_pVar->isSparseBool() && _pInsert->isBool())
        {
            pRet = _pVar->getAs<SparseBool>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isDouble() && _pInsert->isPoly())
        {
            Double* pDest = _pVar->getAs<Double>();
            Polynom* pIns = _pInsert->getAs<Polynom>();
            int iSize = pDest->getSize();
            int* piRanks = new int[iSize];
            memset(piRanks, 0x00, iSize * sizeof(int));
            Polynom* pP = new Polynom(pIns->getVariableName(), pDest->getDims(), pDest->getDimsArray(), piRanks);
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
            Polynom* pDest = _pVar->getAs<Polynom>();
            Double* pIns = _pInsert->getAs<Double>();
            bool isComplexIns = pIns->isComplex();
            int iSize = pIns->getSize();
            int* piRanks = new int[iSize];
            memset(piRanks, 0x00, iSize * sizeof(int));

            //create a new polynom with Double to insert it into dest polynom
            Polynom* pP = new Polynom(pDest->getVariableName(), pIns->getDims(), pIns->getDimsArray(), piRanks);
            delete[] piRanks;

            if (isComplexIns)
            {
                double* pR = pIns->get();
                double* pI = pIns->getImg();
                SinglePoly** pSP = pP->get();
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
                SinglePoly** pSP = pP->get();
                for (int idx = 0 ; idx < pP->getSize() ; idx++)
                {
                    double dblR = pdblR[idx];
                    pSP[idx]->setCoef(&dblR, NULL);
                }
            }

            pRet = pDest->insert(_pArgs, pP);
            pP->killMe();
        }
        else if (_pVar->isPoly() && _pInsert->isPoly())
        {
            pRet = _pVar->getAs<Polynom>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isInt8() && _pInsert->isInt8())
        {
            pRet = _pVar->getAs<Int8>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUInt8() && _pInsert->isUInt8())
        {
            pRet = _pVar->getAs<UInt8>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isInt16() && _pInsert->isInt16())
        {
            pRet = _pVar->getAs<Int16>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUInt16() && _pInsert->isUInt16())
        {
            pRet = _pVar->getAs<UInt16>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isInt32() && _pInsert->isInt32())
        {
            pRet = _pVar->getAs<Int32>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUInt32() && _pInsert->isUInt32())
        {
            pRet = _pVar->getAs<UInt32>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isInt64() && _pInsert->isInt64())
        {
            pRet = _pVar->getAs<Int64>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUInt64() && _pInsert->isUInt64())
        {
            pRet = _pVar->getAs<UInt64>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isStruct())
        {
            Struct* pStruct = _pVar->getAs<Struct>();
            // insert something in a field of a struct
            if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
            {
                //s("x") = y
                String *pS = (*_pArgs)[0]->getAs<String>();
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
                    pStruct->removeField(pS->get(0));
                }
                else
                {
                    /* Add a field */
                    pStruct->addField(pS->get(0));
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
                    String* pStrFieldsName = pStruct->getFieldNames();
                    Struct* pStructInsert = _pInsert->clone()->getAs<Struct>();
                    String* pStrInsertFieldsName = pStructInsert->getFieldNames();
                    Struct* pStructRet = NULL;

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
                                List* pLExtract = pStructInsert->extractFieldWithoutClone(pwcsField);

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
                    pStructRet = pRet->getAs<Struct>();

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
            TList* pTL = _pVar->getAs<TList>();
            if (_pArgs->size() == 1)
            {
                if ((*_pArgs)[0]->isString())
                {
                    //s("x") = y
                    String *pS = (*_pArgs)[0]->getAs<String>();
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
                        pTL->set(pS->get(0), _pInsert);
                        pRet = pTL;
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
                            pTL = pTL->clone()->getAs<TList>();
                        }

                        pRet = pTL->insert(_pArgs, _pInsert);

                        // If we have inserted something else than a String
                        // in the first element, the TList have to be a List.
                        if (pTL->get(0)->isString() == false)
                        {
                            List* pL = new List();
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
            List* pL = NULL;
            // In case where pL is in several scilab variable,
            // we have to clone it for keep the other variables unchanged.
            if (_pVar->getRef() > 1)
            {
                pL = _pVar->clone()->getAs<List>();
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
                pL = _pVar->getAs<List>();
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
                GraphicHandle* pH = _pVar->getAs<GraphicHandle>();
                String *pS = (*_pArgs)[0]->getAs<String>();
                typed_list in;
                typed_list out;
                optional_list opt;
                ast::ExecVisitor exec;

                in.push_back(pH);
                in.push_back(pS);
                in.push_back(_pInsert);

                Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"set"));
                if (pCall)
                {
                    Callable::ReturnValue ret = pCall->call(in, opt, 1, out, &exec);
                    if (ret == Callable::OK)
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
                pRet = _pVar->getAs<GraphicHandle>()->insert(_pArgs, _pInsert);
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
                        InternalType* pIT = pIL->extractFullMatrix();
                        (*_pArgs)[i]->killMe();
                        (*_pArgs)[i] = pIT;
                    }
                }
            }

            pRet = _pVar->getAs<UserType>()->insert(_pArgs, _pInsert);
            if (pRet == NULL)
            {
                pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
            }
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

    InternalType* pOnPrompt = NULL;
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
