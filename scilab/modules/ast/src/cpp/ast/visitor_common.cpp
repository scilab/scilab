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

#include "alltypes.hxx"

size_t ast::Ast::globalNodeNumber = 0;

/*
 * Generate destination variable from _poSource type and size parameters
 */
types::InternalType* allocDest(types::InternalType* _poSource, int _iRows, int _iCols)
{
    types::InternalType* poResult = NULL;
    switch (_poSource->getType())
    {
        case types::GenericType::ScilabDouble :
            poResult = new types::Double(_iRows, _iCols, false);
            break;
        case types::GenericType::ScilabBool :
            poResult = new types::Bool(_iRows, _iCols);
            break;
        case types::GenericType::ScilabInt8 :
            poResult = new types::Int8(_iRows, _iCols);
            break;
        case types::GenericType::ScilabUInt8 :
            poResult = new types::UInt8(_iRows, _iCols);
            break;
        case types::GenericType::ScilabInt16 :
            poResult = new types::Int16(_iRows, _iCols);
            break;
        case types::GenericType::ScilabUInt16 :
            poResult = new types::UInt16(_iRows, _iCols);
            break;
        case types::GenericType::ScilabInt32 :
            poResult = new types::Int32(_iRows, _iCols);
            break;
        case types::GenericType::ScilabUInt32 :
            poResult = new types::UInt32(_iRows, _iCols);
            break;
        case types::GenericType::ScilabInt64 :
            poResult = new types::Int64(_iRows, _iCols);
            break;
        case types::GenericType::ScilabUInt64 :
            poResult = new types::UInt64(_iRows, _iCols);
            break;
        case types::GenericType::ScilabString :
            poResult = new types::String(_iRows, _iCols);
            break;
        case types::GenericType::ScilabPolynom :
        {
            int* piRank = new int[_iRows * _iCols];
            for (int i = 0 ; i < _iRows * _iCols ; i++)
            {
                piRank[i] = 1;
            }
            poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
            break;
        }
        case types::InternalType::ScilabImplicitList :
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
    types::InternalType *poResult	            = NULL;
    types::InternalType::ScilabType TypeSource	= _poSource->getType();
    types::InternalType::ScilabType TypeDest		= types::InternalType::ScilabInternal;
    int iCurRow                                 = _iRows;
    int iCurCol                                 = _iCols;


    if (_poDest == NULL)
    {
        //First call, alloc _poSource
        poResult    = allocDest(_poSource, _iRows, _iCols);
        TypeDest	= TypeSource;
        iCurCol	    = 0;
        iCurRow		= 0;
    }
    else
    {
        TypeDest    = _poDest->getType();
        poResult    = _poDest;
    }

    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
    }
    else
    {
        switch (TypeDest)
        {
            case types::GenericType::ScilabDouble :
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
    types::InternalType *poResult	            = NULL;
    types::InternalType::ScilabType TypeSource	= _poSource->getType();
    types::InternalType::ScilabType TypeDest		= types::InternalType::ScilabInternal;
    int iCurRow                                 = _iRows;
    int iCurCol                                 = _iCols;

    if (_poDest == NULL)
    {
        //First call, alloc _poSource
        poResult    = allocDest(_poSource, _iRows, _iCols);
        iCurCol	    = 0;
        iCurRow		= 0;
        TypeDest	= TypeSource;
    }
    else
    {
        TypeDest	= _poDest->getType();
        poResult    = _poDest;
    }


    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
    }
    else
    {
        switch (TypeDest)
        {
            case types::GenericType::ScilabDouble :
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
    types::InternalType *poResult	= NULL;
    types::InternalType::ScilabType TypeSource	= _poSource->getType();
    types::InternalType::ScilabType TypeDest		=	types::InternalType::ScilabInternal;
    int iCurRow = _iRows;
    int iCurCol = _iCols;

    if (_poDest == NULL)
    {
        switch (TypeSource)
        {
            case types::GenericType::ScilabDouble :
                poResult = new types::Double(_iRows, _iCols, false);
                break;
            case types::GenericType::ScilabBool :
                poResult = new types::Bool(_iRows, _iCols);
                break;
            case types::GenericType::ScilabInt8 :
                poResult = new types::Int8(_iRows, _iCols);
                break;
            case types::GenericType::ScilabUInt8 :
                poResult = new types::UInt8(_iRows, _iCols);
                break;
            case types::GenericType::ScilabInt16 :
                poResult = new types::Int16(_iRows, _iCols);
                break;
            case types::GenericType::ScilabUInt16 :
                poResult = new types::UInt16(_iRows, _iCols);
                break;
            case types::GenericType::ScilabInt32 :
                poResult = new types::Int32(_iRows, _iCols);
                break;
            case types::GenericType::ScilabUInt32 :
                poResult = new types::UInt32(_iRows, _iCols);
                break;
            case types::GenericType::ScilabInt64 :
                poResult = new types::Int64(_iRows, _iCols);
                break;
            case types::GenericType::ScilabUInt64 :
                poResult = new types::UInt64(_iRows, _iCols);
                break;
            case types::GenericType::ScilabString :
                poResult = new types::String(_iRows, _iCols);
                break;
            case types::GenericType::ScilabSparse :
                poResult = new types::Sparse(_iRows, _iCols);
                break;
            case types::GenericType::ScilabSparseBool :
                poResult = new types::SparseBool(_iRows, _iCols);
                break;
            case types::GenericType::ScilabPolynom :
            {
                int* piRank = new int[_iRows * _iCols];
                for (int i = 0 ; i < _iRows * _iCols ; i++)
                {
                    piRank[i] = 1;
                }
                poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
                break;
            }
            case types::InternalType::ScilabImplicitList :
                poResult = new types::ImplicitList();
                break;
            case types::GenericType::ScilabHandle :
                poResult = new types::GraphicHandle(_iRows, _iCols);
                break;
            default :
                // FIXME What should we do here ...
                break;
        }
        iCurCol = 0;
        iCurRow = 0;
        TypeDest =	TypeSource;
    }
    else
    {
        TypeDest = _poDest->getType();
        poResult = _poDest;
    }


    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
        switch (TypeDest)
        {
            case types::GenericType::ScilabDouble :
                if (TypeSource == types::GenericType::ScilabPolynom)
                {
                    types::Double *poDest = _poDest->getAs<types::Double>();
                    //Convert Dest to ScilabPolynom
                    int *piRank = new int[poDest->getSize()];
                    for (int i = 0 ; i < poDest->getSize() ; i++)
                    {
                        piRank[i] = 1;
                    }

                    poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), poDest->getRows(), poDest->getCols(),  piRank);

                    double *pR = poDest->getReal();
                    double *pI = poDest->getImg();
                    for (int i = 0 ; i < poDest->getSize() ; i++)
                    {
                        types::Double *pdbl = NULL;
                        if (poDest->isComplex())
                        {
                            pdbl = new types::Double(pR[i], pI[i]);
                        }
                        else
                        {
                            pdbl = new types::Double(pR[i]);
                        }

                        poResult->getAs<types::Polynom>()->setCoef(i, pdbl);
                        delete pdbl;
                    }

                    Polynom* pP = _poSource->getAs<types::Polynom>();

                    for (int i = 0 ; i < pP->getRows() ; i++)
                    {
                        for (int j = 0 ; j < pP->getCols() ; j++)
                        {
                            poResult->getAs<types::Polynom>()->setCoef(iCurRow + i, iCurCol + j, _poSource->getAs<types::Polynom>()->get(i, j)->getCoef());
                        }
                    }
                }
                break;
            case types::GenericType::ScilabPolynom :
                if (TypeSource == types::GenericType::ScilabDouble)
                {
                    //Add Source like coef of the new element
                    Double* pD = _poSource->getAs<Double>();
                    types::Polynom* pPolyOut = poResult->getAs<types::Polynom>();

                    if (pD->isComplex())
                    {
                        pPolyOut->setComplex(true);
                        for (int i = 0 ; i < pD->getRows() ; i++)
                        {
                            for (int j = 0 ; j < pD->getCols() ; j++)
                            {
                                types::SinglePoly* pSPOut = pPolyOut->get(iCurRow + i, iCurCol + j);

                                pSPOut->setRank(1);
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

                                pSPOut->setRank(1);
                                double pDbl = pD->get(i, j);
                                pSPOut->setCoef(&pDbl, NULL);
                            }
                        }
                    }
                }
                break;
            case types::GenericType::ScilabSparse :
                if (TypeSource == types::GenericType::ScilabDouble)
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
                }
                break;
            case types::GenericType::ScilabSparseBool :
                if (TypeSource == types::GenericType::ScilabBool)
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
                }
                break;
            default:
                break;
        }
        return poResult;
    }
    else
    {
        //Just add the new value in the current item
        switch (TypeDest)
        {
            case types::GenericType::ScilabDouble :
                ((Double*)poResult)->append(iCurRow, iCurCol, (Double*)_poSource);
                break;
            case types::GenericType::ScilabPolynom :
                poResult->getAs<types::Polynom>()->append(iCurRow, iCurCol, _poSource->getAs<types::Polynom>());
                break;
            case types::GenericType::ScilabBool:
                poResult->getAs<types::Bool>()->append(iCurRow, iCurCol, _poSource->getAs<types::Bool>());
                break;
            case types::GenericType::ScilabInt8 :
                poResult->getAs<types::Int8>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int8>());
                break;
            case types::GenericType::ScilabUInt8 :
                poResult->getAs<types::UInt8>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt8>());
                break;
            case types::GenericType::ScilabInt16 :
                poResult->getAs<types::Int16>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int16>());
                break;
            case types::GenericType::ScilabUInt16 :
                poResult->getAs<types::UInt16>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt16>());
                break;
            case types::GenericType::ScilabInt32 :
                poResult->getAs<types::Int32>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int32>());
                break;
            case types::GenericType::ScilabUInt32 :
                poResult->getAs<types::UInt32>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt32>());
                break;
            case types::GenericType::ScilabInt64 :
                poResult->getAs<types::Int64>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int64>());
                break;
            case types::GenericType::ScilabUInt64 :
                poResult->getAs<types::UInt64>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt64>());
                break;
            case types::GenericType::ScilabSparse :
                poResult->getAs<types::Sparse>()->append(iCurRow, iCurCol, _poSource->getAs<types::Sparse>());
                break;
            case types::GenericType::ScilabSparseBool :
                poResult->getAs<types::SparseBool>()->append(iCurRow, iCurCol, _poSource->getAs<types::SparseBool>());
                break;
            case types::GenericType::ScilabString :
            {
                types::String* pSource = _poSource->getAs<types::String>();
                poResult->getAs<types::String>()->append(iCurRow, iCurCol, pSource);
            }
            break;
            case types::GenericType::ScilabImplicitList :
            {
                if (_poSource->getAs<ImplicitList>()->getStartType() == types::InternalType::ScilabPolynom)
                {
                    poResult->getAs<ImplicitList>()->setStart(_poSource->getAs<ImplicitList>()->getStart());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setStart(_poSource->getAs<ImplicitList>()->getStart());
                }

                if (_poSource->getAs<ImplicitList>()->getStepType() == types::InternalType::ScilabPolynom)
                {
                    poResult->getAs<ImplicitList>()->setStep(_poSource->getAs<ImplicitList>()->getStep());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setStep(_poSource->getAs<ImplicitList>()->getStep());
                }

                if (_poSource->getAs<ImplicitList>()->getEndType() == types::InternalType::ScilabPolynom)
                {
                    poResult->getAs<ImplicitList>()->setEnd(_poSource->getAs<ImplicitList>()->getEnd());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setEnd(_poSource->getAs<ImplicitList>()->getEnd());
                }
                break;
            }
            case types::GenericType::ScilabHandle :
                poResult->getAs<types::GraphicHandle>()->append(iCurRow, iCurCol, _poSource->getAs<types::GraphicHandle>());
                break;
            default:
                break;
        }
        return poResult;
    }
    return NULL;
}

const std::wstring* getStructNameFromExp(const ast::Exp* _pExp)
{
    const ast::FieldExp* pField =  dynamic_cast<const ast::FieldExp*>(_pExp);
    const ast::SimpleVar* pVar =  dynamic_cast<const ast::SimpleVar*>(_pExp);
    const ast::CallExp* pCall =  dynamic_cast<const ast::CallExp*>(_pExp);

    if (pField)
    {
        return getStructNameFromExp(pField->head_get());
    }
    else if (pVar)
    {
        return &(pVar->name_get().name_get());
    }
    else if (pCall)
    {
        return getStructNameFromExp(&(pCall->name_get()));
    }
    else
    {
        std::wostringstream os;
        os << _W("Unknow expression");
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ast::ScilabError(os.str(), 999, _pExp->location_get());
    }
    return NULL;
}

/*** overload insertion                 ||         extraction       ***/
//%x_i_x(i1, i2, ..., in, source, dest) || %x_e(i1, i2, ..., in, source, dest)
//i1, ..., in : indexes
//dest : variable where insert data     || NULL
//source : data to insert               || extract indexes from source
types::InternalType* callOverload(const ast::Exp& e, std::wstring _strType, types::typed_list* _pArgs, types::InternalType* _source, types::InternalType* _dest)
{
    types::InternalType* pITOut = NULL;
    types::typed_list in;
    types::typed_list out;

    std::wstring function_name;
    function_name = L"%" + _source->getShortTypeStr() + L"_" + _strType;

    for (int i = 0; i < _pArgs->size(); i++)
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
        if (Overload::call(function_name, in, 1, out, &exec))
        {
            //manage error
            std::wostringstream os;
            os << _W("Error in overload function: ") << function_name << std::endl;
            throw ast::ScilabError(os.str(), 999, e.location_get());
        }
    }

    for (int i = 0; i < _pArgs->size(); i++)
    {
        (*_pArgs)[i]->DecreaseRef();
    }

    _source->DecreaseRef();
    if (_dest)
    {
        _dest->DecreaseRef();
    }

    if (out.size() == 1)
    {
        pITOut = out[0];
    }
    else if (out.size() > 1)
    {
        List* pListOut = new List();
        for (int i = 0; i < out.size(); i++)
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
        if (getFieldsFromExp(pField->head_get(), fields))
        {
            return getFieldsFromExp(pField->tail_get(), fields);
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
        ast::ExecVisitor execMe;
        typed_list* pCurrentArgs = execMe.GetArgumentList(pCall->args_get());

        bool bErr = getFieldsFromExp(&pCall->name_get(), fields);
        if (fields.back()->getArgs())
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
        }

        if (pCell)
        {
            // a{x}
            fields.back()->setCellExp();
        }

        return bErr;
    }
    else
    {
        return false;
    }
}

types::InternalType* evaluateFields(const ast::Exp* _pExp, std::list<ExpHistory*>& fields, InternalType* _pAssignValue)
{
    std::list<ExpHistory*> evalFields;

    //*** get main variable ***//
    std::list<ExpHistory*>::iterator iterFields = fields.begin();
    ExpHistory* pFirstField = *iterFields;
    InternalType* pIT = symbol::Context::getInstance()->getCurrentLevel(pFirstField->getExp()->name_get());
    if (pIT == NULL)
    {
        if (pFirstField->isCellExp())
        {
            // a{x}, where "a" doesn't exists
            pIT = new Cell(1, 1);
            symbol::Context::getInstance()->put(pFirstField->getExp()->stack_get(), pIT);
        }
        else if (fields.size() > 1)
        {
            // is a field exp
            //"a" does not exist or it is another type, create it with size 1,1 and return it
            //create new structure variable
            pIT = new Struct(1, 1);
            symbol::Context::getInstance()->put(pFirstField->getExp()->stack_get(), pIT);
        }
        // else
        // is a call exp
        // a(x) = "something" and a does not exist
        // a will be create in insertionCall
    }
    else if (pIT->getRef() > 1 && pIT->isHandle() == false)
    {
        pIT = pIT->clone();
        symbol::Context::getInstance()->put(pFirstField->getExp()->stack_get(), pIT);
    }
    else if (pIT == _pAssignValue)
    {
        // clone me before insert me in myself.
        // ie : a.b = 2; a.b.c.d = a;
        _pAssignValue = _pAssignValue->clone();
    }

    iterFields++;

    std::list<ExpHistory*> workFields;
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

        types::InternalType* pITCurrent = pEH->getCurrent();

        if (pEH->isCellExp() && pITCurrent->isCell() == false)
        {
            std::wostringstream os;
            os << _W("Wrong insertion : use extraction with {} only on a Cell.");
            throw ast::ScilabError(os.str(), 999, _pExp->location_get());
        }

        if (pITCurrent->isStruct())
        {
            Struct* pStruct = pITCurrent->getAs<Struct>();
            std::wstring pwcsFieldname = (*iterFields)->getExpAsString();
            int iSizeStuct = pStruct->getSize();

            if (pEH->needResize())
            {
                if (pEH->getArgsDims() == 1)
                {
                    std::wostringstream os;
                    os << _W("Invalid index.");
                    throw ast::ScilabError(os.str(), 999, _pExp->location_get());
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
                // extract field x and append it to elements for next recursion.
                List* pLOut = pStruct->extractFieldWithoutClone(pwcsFieldname);
                for (int iList = 0; iList < pLOut->getSize(); iList++)
                {
                    InternalType* pIT = pLOut->get(iList);
                    if (pIT->getRef() > 2) //One for my own ref + 1 for "extractFieldWithoutClone" artificial ref
                    {
                        // clone element before modify it.
                        //pIT->DecreaseRef();
                        pIT = pIT->clone();
                        pStruct->get(iList)->set(pwcsFieldname, pIT);
                    }

                    ExpHistory* pEHChield = new ExpHistory( pEH,
                                                            (*iterFields)->getExp(),
                                                            (*iterFields)->getArgs(),
                                                            (*iterFields)->getLevel(),
                                                            (*iterFields)->isCellExp(),
                                                            pIT);
                    pEHChield->setWhereReinsert(iList);
                    workFields.push_back(pEHChield);
                }

                delete pLOut;
            }
        }
        else if (pITCurrent->isTList() || pITCurrent->isMList())
        {
            TList* pTL = pITCurrent->getAs<TList>();
            typed_list* pArgs = pEH->getArgs();
            if (pArgs && (*pArgs)[0]->isString() == false)
            {
                if (pArgs->size() > 1 || pITCurrent->isMList())
                {
                    // call overload
                    InternalType* pExtract = callOverload(*pEH->getExp(), L"6", pArgs, pTL, NULL);
                    if ((*iterFields)->getExp() == NULL)
                    {
                        // a(x)(y) => a.b(y)
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
                    // resize TList
                    int iNewSize = pEH->getSizeFromArgs();
                    if (pTL->getSize() < iNewSize)
                    {
                        pTL->set(iNewSize - 1, new ListUndefined());
                    }

                    std::vector<InternalType*> vectpIT = pTL->extract(pArgs);
                    std::vector<InternalType*>::iterator iterVect;
                    double* pdblArgs = (*pArgs)[0]->getAs<Double>()->get();
                    int iLoop = 0;

                    if ((*iterFields)->getExp() == NULL)
                    {
                        // a(x)(y) => a.b(y)
                        // extract a(x) and push_BACK to extract next level
                        for (iterVect = vectpIT.begin(); iterVect != vectpIT.end(); iterVect++, iLoop++)
                        {
                            ExpHistory* pEHExtract = new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), *iterVect);
                            pEHExtract->setWhereReinsert((int)(pdblArgs[iLoop] - 1));
                            workFields.push_back(pEHExtract);
                        }
                    }
                    else
                    {
                        // a(x).b
                        // extract a(x) and push_FRONT to extract b from a(x)
                        for (iterVect = vectpIT.begin(); iterVect != vectpIT.end(); iterVect++, iLoop++)
                        {
                            ExpHistory* pEHExtract = new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), *iterVect);
                            pEHExtract->setWhereReinsert((int)(pdblArgs[iLoop] - 1));
                            workFields.push_front(pEHExtract);
                        }
                    }
                }
            }
            else
            {
                // get string "x"
                InternalType* pExtract = NULL;
                std::wstring pwcsFieldname = L"";
                bool bReinsert = false;
                ExpHistory* pEHChield = NULL;

                if (pArgs)
                {
                    // a('x')
                    pwcsFieldname = (*pArgs)[0]->getAs<String>()->get(0);
                }
                else
                {
                    // a.x
                    pwcsFieldname = (*iterFields)->getExpAsString();
                }

                // check if field exists
                if (pTL->exists(pwcsFieldname) == false)
                {
                    std::list<ExpHistory*>::iterator iterFieldsNext(iterFields);
                    ++iterFieldsNext;

                    if (iterFieldsNext != fields.end() || (*iterFields)->getArgs() != NULL)
                    {
                        // M=mlist(['MType','x','y'], ...
                        // M.rows1 = "somthing"
                        if (pArgs == NULL)
                        {
                            pArgs = new typed_list();
                            pArgs->push_back(new String(pwcsFieldname.c_str()));
                        }

                        // call overload
                        pExtract = callOverload(*pEH->getExp(), L"6", pArgs, pTL, NULL);
                        bReinsert = true;

                        if (pEH->getArgs() == NULL)
                        {
                            delete pArgs;
                        }
                    }
                }
                else
                {
                    // extract field x and append it to elements for next recursion.
                    pExtract = pTL->getField(pwcsFieldname);
                }

                // History management
                if (pEH->getArgs())
                {
                    if ((*iterFields)->getExp() == NULL)
                    {
                        // a('x')(y) => a.b(y)
                        // extract a(x) and push_BACK to extract next level
                        pEHChield = new ExpHistory(pEH, NULL, (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract);
                        workFields.push_back(pEHChield);
                    }
                    else
                    {
                        // a('x').b -> a('x')('b')
                        // extract a(x) and push_FRONT to extract b from a(x)
                        pEHChield = new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pExtract);
                        workFields.push_front(pEHChield);
                    }
                }
                else
                {
                    // a.x
                    pEHChield = new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract);
                    workFields.push_back(pEHChield);
                }

                if (bReinsert)
                {
                    pEHChield->setReinsertion();
                }
            }
        }
        else if (pITCurrent->isList())
        {
            List* pL = pITCurrent->getAs<List>();
            if (pEH->getParent() && pEH->getParent()->getLevel() == pEH->getLevel())
            {
                // pITCurrent is an extraction of other Type
                for (int iLoop = 0; iLoop < pL->getSize(); iLoop++)
                {
                    ExpHistory* pEHExtract = new ExpHistory(pEH, pEH->getExp(), NULL, pEH->getLevel(), pEH->isCellExp(), pL->get(iLoop));
                    pEHExtract->setWhereReinsert(iLoop);
                    workFields.push_front(pEHExtract);
                }
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

                    // call overload
                    typed_list* args = new typed_list();
                    args->push_back(new String(pwcsFieldname.c_str()));
                    pEH->setArgs(args);

                    InternalType* pExtract = callOverload(*pEH->getExp(), L"6", args, pL, NULL);

                    // append extraction of a.x for next level.
                    workFields.push_back(new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                    workFields.back()->setReinsertion();
                }
            }
        }
        else if (pITCurrent->isHandle())
        {
            // call overload
            if (pEH->getArgs())
            {
                // call overload
                InternalType* pExtract = callOverload(*pEH->getExp(), L"e", pEH->getArgs(), pITCurrent, NULL);

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

                // call overload
                InternalType* pExtract = callOverload(*pEH->getExp(), L"e", args, pITCurrent, NULL);

                // append extraction of a.x for next level.
                workFields.push_back(new ExpHistory(pEH, (*iterFields)->getExp(), (*iterFields)->getArgs(), (*iterFields)->getLevel(), (*iterFields)->isCellExp(), pExtract));
                workFields.front()->setReinsertion();
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
                                throw ast::ScilabError(os.str(), 999, _pExp->location_get());
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
                                throw ast::ScilabError(os.str(), 999, _pExp->location_get());
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
                        throw ast::ScilabError(os.str(), 999, _pExp->location_get());
                    }
                }
            }
            else
            {
                std::wostringstream os;
                os << _W("Wrong insertion in a Cell.");
                throw ast::ScilabError(os.str(), 999, _pExp->location_get());
            }
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

    //*** insert what we have to assign ***//
    while (workFields.empty() == false)
    {
        ExpHistory* pEH = workFields.front();
        evalFields.push_back(pEH);
        workFields.pop_front();

        if (pEH->getArgs())
        {
            typed_list* pArgs = pEH->getArgs();
            if (pEH->isCellExp())
            {
                Cell* pCell = pEH->getCurrent()->getAs<Cell>();
                // insert "something" in b{x}
                if ((*pArgs)[0]->isString())
                {
                    std::wostringstream os;
                    os << _W("Wrong insertion in a Cell.");
                    throw ast::ScilabError(os.str(), 999, _pExp->location_get());
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
                    os << _W("Error in insertion of Struct.");
                    throw ast::ScilabError(os.str(), 999, _pExp->location_get());
                }

                if (pEH->setCurrent(pIT))
                {
                    pEH->setReinsertion();
                }
            }
        }
        else
        {
            InternalType* pParent = pEH->getParent()->getCurrent();
            if (pParent->isStruct())
            {
                Struct* pStruct = pParent->getAs<Struct>();
                if (_pAssignValue->isListDelete())
                {
                    pStruct->removeField(pEH->getExpAsString());
                }
                else
                {
                    pStruct->get(pEH->getWhereReinsert())->set(pEH->getExpAsString(), _pAssignValue);
                }
            }
            else if (pParent->isTList() || pParent->isMList())
            {
                TList* pTL = pParent->getAs<TList>();
                if (_pAssignValue->isListDelete() || (pTL->exists(pEH->getExpAsString()) == false))
                {
                    typed_list args;
                    args.push_back(new String(pEH->getExpAsString().c_str()));
                    InternalType* pIT = insertionCall(*_pExp, &args, pEH->getParent()->getCurrent(), _pAssignValue);
                    if (pIT == NULL)
                    {
                        std::wostringstream os;
                        os << _W("Error in insertion of TList.");
                        throw ast::ScilabError(os.str(), 999, _pExp->location_get());
                    }

                    if (pEH->getParent()->setCurrent(pIT))
                    {
                        pEH->getParent()->setReinsertion();
                        pEH->resetReinsertion();
                    }
                }
                else
                {
                    pTL->set(pEH->getExpAsString(), _pAssignValue);
                }
            }
            else
            {
                pEH->setCurrent(_pAssignValue);
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
                symbol::Context::getInstance()->put(pEH->getExp()->stack_get(), pEH->getCurrent());
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
                        pTL->set(pEH->getWhereReinsert(), pEH->getCurrent());
                        evalFields.pop_back();
                        delete pEH;
                        continue;
                    }
                    else
                    {
                        if (pTL->exists(pEH->getExpAsString()))
                        {
                            pTL->set(pEH->getExpAsString(), pEH->getCurrent());
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
                os << _W("Error in insertion of Struct.");
                throw ast::ScilabError(os.str(), 999, _pExp->location_get());
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

    return symbol::Context::getInstance()->getCurrentLevel(pFirstField->getExp()->name_get());
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
        _pInsert = pIL;
    }
    else if (_pInsert->isImplicitList())
    {
        pIL = _pInsert->getAs<ImplicitList>()->extractFullMatrix();
        if (pIL && pIL->isDeletable())
        {
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
        if (_pVar->isDouble())
        {
            pOut = _pVar->getAs<Double>()->remove(_pArgs);
        }
        else if (_pVar->isString())
        {
            pOut = _pVar->getAs<String>()->remove(_pArgs);
        }
        else if (_pVar->isCell())
        {
            pOut = _pVar->getAs<Cell>()->remove(_pArgs);
        }
        else if (_pVar->isBool())
        {
            pOut = _pVar->getAs<Bool>()->remove(_pArgs);
        }
        else if (_pVar->isPoly())
        {
            pOut = _pVar->getAs<Polynom>()->remove(_pArgs);
        }
        else if (_pVar->isInt8())
        {
            pOut = _pVar->getAs<Int8>()->remove(_pArgs);
        }
        else if (_pVar->isUInt8())
        {
            pOut = _pVar->getAs<UInt8>()->remove(_pArgs);
        }
        else if (_pVar->isInt16())
        {
            pOut = _pVar->getAs<Int16>()->remove(_pArgs);
        }
        else if (_pVar->isUInt16())
        {
            pOut = _pVar->getAs<UInt16>()->remove(_pArgs);
        }
        else if (_pVar->isInt32())
        {
            pOut = _pVar->getAs<Int32>()->remove(_pArgs);
        }
        else if (_pVar->isUInt32())
        {
            pOut = _pVar->getAs<UInt32>()->remove(_pArgs);
        }
        else if (_pVar->isInt64())
        {
            pOut = _pVar->getAs<Int64>()->remove(_pArgs);
        }
        else if (_pVar->isUInt64())
        {
            pOut = _pVar->getAs<UInt64>()->remove(_pArgs);
        }
        else if (_pVar->isStruct())
        {
            // a("b") = [] is not a deletion !!
            Struct* pStr = _pVar->getAs<Struct>();

            pOut = _pVar->getAs<Struct>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isHandle())
        {
            InternalType* pRet = NULL;

            types::GraphicHandle* pH = _pVar->getAs<GraphicHandle>();
            types::String *pS = (*_pArgs)[0]->getAs<types::String>();

            typed_list in;
            typed_list out;
            optional_list opt;
            ast::ExecVisitor exec;

            in.push_back(pH);
            in.push_back(pS);
            in.push_back(_pInsert);

            Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"set"));
            Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, &exec);
            if (ret == Callable::OK)
            {
                pRet = _pVar;
            }
            pOut = pRet;
        }
    }
    else if (_pVar == NULL || (_pVar->isDouble() && _pVar->getAs<Double>()->getSize() == 0))
    {
        //insert in a new variable or []
        //call static insert function
        //if _pVar == NULL and pArg is single string, it's a struct creation
        if ((*_pArgs)[0]->isString())
        {
            String *pS = (*_pArgs)[0]->getAs<types::String>();
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
                throw ast::ScilabError(os.str(), 999, e.location_get());
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
                default :
                {
                    // overload
                    types::Double* pEmpty = types::Double::Empty();
                    pOut = callOverload(e, L"i", _pArgs, _pInsert, pEmpty);
                    delete pEmpty;
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
            free(pD);
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
            Polynom* pP = new Polynom(pIns->getVariableName(), pDest->getDims(), pDest->getDimsArray());
            pP->setComplex(pDest->isComplex());

            for (int idx = 0 ; idx < pP->getSize() ; idx++)
            {
                double* pR = NULL;
                double* pI = NULL;
                if (pP->isComplex())
                {
                    SinglePoly* pS = new SinglePoly(&pR, &pI, 1);
                    double dblR = pDest->get(idx);
                    double dblI = pDest->getImg(idx);
                    pS->setCoef(&dblR, &dblI);
                    pP->set(idx, pS);
                    delete pS;
                }
                else
                {
                    SinglePoly* pS = new SinglePoly(&pR, 1);
                    double dblR = pDest->get(idx);
                    pS->setCoef(&dblR, NULL);
                    pP->set(idx, pS);
                    delete pS;
                }
            }

            pRet = pP->insert(_pArgs, pIns);
            pDest->DecreaseRef();
        }
        else if (_pVar->isPoly() && _pInsert->isDouble())
        {
            Polynom* pDest = _pVar->getAs<Polynom>();
            Double* pIns = _pInsert->getAs<Double>();
            Polynom* pP = new Polynom(pDest->getVariableName(), pIns->getDims(), pIns->getDimsArray());
            pP->setComplex(pIns->isComplex());

            for (int idx = 0 ; idx < pP->getSize() ; idx++)
            {
                double* pR = NULL;
                double* pI = NULL;
                if (pP->isComplex())
                {
                    SinglePoly* pS = new SinglePoly(&pR, &pI, 1);
                    double dblR = pIns->get(idx);
                    double dblI = pIns->getImg(idx);
                    pS->setCoef(&dblR, &dblI);
                    pP->set(idx, pS);
                    delete pS;
                }
                else
                {
                    SinglePoly* pS = new SinglePoly(&pR, 1);
                    double dblR = pIns->get(idx);
                    pS->setCoef(&dblR, NULL);
                    pP->set(idx, pS);
                    delete pS;
                }
            }
            pRet = pDest->insert(_pArgs, pP);
            delete pP;
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
                String *pS = (*_pArgs)[0]->getAs<types::String>();
                if (pS->isScalar() == false)
                {
                    if (pIL)
                    {
                        pIL->killMe();
                    }
                    //manage error
                    std::wostringstream os;
                    os << _W("Invalid Index.\n");
                    throw ast::ScilabError(os.str(), 999, e.location_get());
                }

                pStruct->addField(pS->get(0));
                for (int i = 0; i < pStruct->getSize(); i++)
                {
                    pStruct->get(i)->set(pS->get(0), _pInsert);
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
                            }
                        }
                    }

                    // insert elements in following pArgs
                    pRet = pStruct->insert(_pArgs, pStructInsert);
                    pStructRet = pRet->getAs<Struct>();

                    // insert fields of pStructInsert in pRet
                    for (int i = 0; i < pStrInsertFieldsName->getSize(); i++)
                    {
                        if (pStructRet->exists(pStrInsertFieldsName->get(i)) == false)
                        {
                            pStructRet->addField(pStrInsertFieldsName->get(i));
                        }
                    }
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
                    String *pS = (*_pArgs)[0]->getAs<types::String>();
                    if (pS->isScalar() == false)
                    {
                        if (pIL)
                        {
                            pIL->killMe();
                        }

                        //manage error
                        std::wostringstream os;
                        os << _W("Invalid Index.\n");
                        throw ast::ScilabError(os.str(), 999, e.location_get());
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
                        //types::typed_list in;
                        //types::typed_list out;
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
                        pRet = pTL->insert(_pArgs, _pInsert);
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
            pRet = _pVar->getAs<List>()->insert(_pArgs, _pInsert);
            if (pRet == NULL)
            {
                // call overload
                pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
            }
        }
        else if (_pVar->isHandle())
        {
            if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
            {
                //s(["x"])
                types::GraphicHandle* pH = _pVar->getAs<types::GraphicHandle>();
                types::String *pS = (*_pArgs)[0]->getAs<types::String>();
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
                    Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, &exec);
                    if (ret == Callable::OK)
                    {
                        pRet = _pVar;
                    }
                }
            }
            else
            {
                pRet = _pVar->getAs<types::GraphicHandle>()->extract(_pArgs);
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
        types::typed_list in;
        types::typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execCall;
        pOnPrompt->getAs<types::Callable>()->call(in, opt, 1, out, &execCall);
    }
}

List* getPropertyTree(ast::Exp* e, List* pList)
{

    //a.b
    ast::SimpleVar* pVar = dynamic_cast<ast::SimpleVar*>(e);
    if (pVar)
    {
        pList->append(new String(pVar->name_get().name_get().c_str()));
        return pList;
    }

    //a(x).b
    ast::CallExp* pCall = dynamic_cast<ast::CallExp*>(e);
    if (pCall)
    {
        pList = getPropertyTree(&pCall->name_get(), pList);
        ast::ExecVisitor exec;
        std::list<ast::Exp*> l = pCall->args_get();
        std::list<ast::Exp*>::const_iterator it;
        for (it = l.begin() ; it != l.end() ; it++)
        {
            ast::Exp* pArg = (*it);
            try
            {
                pArg->accept(exec);
                pList->append(exec.result_get());
                exec.result_clear();
            }
            catch (ast::ScilabException e)
            {
                throw e;
            }
        }

        return pList;
    }

    //a.b.c
    ast::FieldExp* pField = dynamic_cast<ast::FieldExp*>(e);
    if (pField)
    {
        pList = getPropertyTree(pField->head_get(), pList);
        pList = getPropertyTree(pField->tail_get(), pList);
        return pList;
    }

    return pList;
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
