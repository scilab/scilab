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

#include "alltypes.hxx"

using namespace types;

bool bConditionState(types::InternalType *_pITResult)
{
    if (_pITResult->isDouble() &&
            _pITResult->getAs<types::Double>()->isComplex() == false)
    {
        types::Double *pR = _pITResult->getAs<types::Double>();
        if (pR->isEmpty())
        {
            //[]
            return false;
        }

        double *pReal = pR->getReal();
        for (int i = 0 ; i < pR->getSize() ; i++)
        {
            if (pReal[i] == 0)
            {
                return false;
            }
        }
    }
    else if (_pITResult->isBool())
    {
        types::Bool *pB		= _pITResult->getAs<types::Bool>();
        int *piData	= pB->get();

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            if (piData[i] == 0)
            {
                return false;
                break;
            }
        }
    }
    else if (_pITResult->isInt())
    {
    }
    else
    {
        return false;
    }
    return true;
}

/*
 * Generate destination variable from _poSource type and size parameters
 */
types::InternalType* allocDest(types::InternalType* _poSource, int _iRows, int _iCols)
{
    types::InternalType* poResult = NULL;
    switch (_poSource->getType())
    {
        case types::GenericType::RealDouble :
            poResult = new types::Double(_iRows, _iCols, false);
            break;
        case types::GenericType::RealBool :
            poResult = new types::Bool(_iRows, _iCols);
            break;
        case types::GenericType::RealInt8 :
            poResult = new types::Int8(_iRows, _iCols);
            break;
        case types::GenericType::RealUInt8 :
            poResult = new types::UInt8(_iRows, _iCols);
            break;
        case types::GenericType::RealInt16 :
            poResult = new types::Int16(_iRows, _iCols);
            break;
        case types::GenericType::RealUInt16 :
            poResult = new types::UInt16(_iRows, _iCols);
            break;
        case types::GenericType::RealInt32 :
            poResult = new types::Int32(_iRows, _iCols);
            break;
        case types::GenericType::RealUInt32 :
            poResult = new types::UInt32(_iRows, _iCols);
            break;
        case types::GenericType::RealInt64 :
            poResult = new types::Int64(_iRows, _iCols);
            break;
        case types::GenericType::RealUInt64 :
            poResult = new types::UInt64(_iRows, _iCols);
            break;
        case types::GenericType::RealString :
            poResult = new types::String(_iRows, _iCols);
            break;
        case types::GenericType::RealPoly :
        {
            int* piRank = new int[_iRows * _iCols];
            for (int i = 0 ; i < _iRows * _iCols ; i++)
            {
                piRank[i] = 1;
            }
            poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
            break;
        }
        case types::InternalType::RealImplicitList :
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
    types::InternalType::RealType TypeSource	= _poSource->getType();
    types::InternalType::RealType TypeDest		= types::InternalType::RealInternal;
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
            case types::GenericType::RealDouble :
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
    types::InternalType::RealType TypeSource	= _poSource->getType();
    types::InternalType::RealType TypeDest		= types::InternalType::RealInternal;
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
            case types::GenericType::RealDouble :
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
    types::InternalType::RealType TypeSource	= _poSource->getType();
    types::InternalType::RealType TypeDest		=	types::InternalType::RealInternal;
    int iCurRow = _iRows;
    int iCurCol = _iCols;

    if (_poDest == NULL)
    {
        switch (TypeSource)
        {
            case types::GenericType::RealDouble :
                poResult = new types::Double(_iRows, _iCols, false);
                break;
            case types::GenericType::RealBool :
                poResult = new types::Bool(_iRows, _iCols);
                break;
            case types::GenericType::RealInt8 :
                poResult = new types::Int8(_iRows, _iCols);
                break;
            case types::GenericType::RealUInt8 :
                poResult = new types::UInt8(_iRows, _iCols);
                break;
            case types::GenericType::RealInt16 :
                poResult = new types::Int16(_iRows, _iCols);
                break;
            case types::GenericType::RealUInt16 :
                poResult = new types::UInt16(_iRows, _iCols);
                break;
            case types::GenericType::RealInt32 :
                poResult = new types::Int32(_iRows, _iCols);
                break;
            case types::GenericType::RealUInt32 :
                poResult = new types::UInt32(_iRows, _iCols);
                break;
            case types::GenericType::RealInt64 :
                poResult = new types::Int64(_iRows, _iCols);
                break;
            case types::GenericType::RealUInt64 :
                poResult = new types::UInt64(_iRows, _iCols);
                break;
            case types::GenericType::RealString :
                poResult = new types::String(_iRows, _iCols);
                break;
            case types::GenericType::RealSparse :
                poResult = new types::Sparse(_iRows, _iCols);
                break;
            case types::GenericType::RealSparseBool :
                poResult = new types::SparseBool(_iRows, _iCols);
                break;
            case types::GenericType::RealPoly :
            {
                int* piRank = new int[_iRows * _iCols];
                for (int i = 0 ; i < _iRows * _iCols ; i++)
                {
                    piRank[i] = 1;
                }
                poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
                break;
            }
            case types::InternalType::RealImplicitList :
                poResult = new types::ImplicitList();
                break;
            case types::GenericType::RealHandle :
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
            case types::GenericType::RealDouble :
                if (TypeSource == types::GenericType::RealPoly)
                {
                    types::Double *poDest = _poDest->getAs<types::Double>();
                    //Convert Dest to RealPoly
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
            case types::GenericType::RealPoly :
                if (TypeSource == types::GenericType::RealDouble)
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
            case types::GenericType::RealSparse :
                if (TypeSource == types::GenericType::RealDouble)
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
            case types::GenericType::RealSparseBool :
                if (TypeSource == types::GenericType::RealBool)
                {
                    types::Bool* poSource = _poSource->getAs<types::Bool>();
                    types::SparseBool* spResult = poResult->getAs<types::SparseBool>();

                    // Add poSource at the end of spResult
                    for (int i = 0; i < poSource->getRows(); i++)
                    {
                        for (int j = 0; j < poSource->getCols(); j++)
                        {
                            bool bValue = poSource->get(i, j);
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
            case types::GenericType::RealDouble :
                ((Double*)poResult)->append(iCurRow, iCurCol, (Double*)_poSource);
                break;
            case types::GenericType::RealPoly :
                poResult->getAs<types::Polynom>()->append(iCurRow, iCurCol, _poSource->getAs<types::Polynom>());
                break;
            case types::GenericType::RealBool:
                poResult->getAs<types::Bool>()->append(iCurRow, iCurCol, _poSource->getAs<types::Bool>());
                break;
            case types::GenericType::RealInt8 :
                poResult->getAs<types::Int8>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int8>());
                break;
            case types::GenericType::RealUInt8 :
                poResult->getAs<types::UInt8>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt8>());
                break;
            case types::GenericType::RealInt16 :
                poResult->getAs<types::Int16>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int16>());
                break;
            case types::GenericType::RealUInt16 :
                poResult->getAs<types::UInt16>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt16>());
                break;
            case types::GenericType::RealInt32 :
                poResult->getAs<types::Int32>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int32>());
                break;
            case types::GenericType::RealUInt32 :
                poResult->getAs<types::UInt32>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt32>());
                break;
            case types::GenericType::RealInt64 :
                poResult->getAs<types::Int64>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int64>());
                break;
            case types::GenericType::RealUInt64 :
                poResult->getAs<types::UInt64>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt64>());
                break;
            case types::GenericType::RealSparse :
                poResult->getAs<types::Sparse>()->append(iCurRow, iCurCol, _poSource->getAs<types::Sparse>());
                break;
            case types::GenericType::RealSparseBool :
                poResult->getAs<types::SparseBool>()->append(iCurRow, iCurCol, _poSource->getAs<types::SparseBool>());
                break;
            case types::GenericType::RealString :
            {
                types::String* pSource = _poSource->getAs<types::String>();
                poResult->getAs<types::String>()->append(iCurRow, iCurCol, pSource);
            }
            break;
            case types::GenericType::RealImplicitList :
            {
                if (_poSource->getAs<ImplicitList>()->getStartType() == types::InternalType::RealPoly)
                {
                    poResult->getAs<ImplicitList>()->setStart(_poSource->getAs<ImplicitList>()->getStart());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setStart(_poSource->getAs<ImplicitList>()->getStart());
                }

                if (_poSource->getAs<ImplicitList>()->getStepType() == types::InternalType::RealPoly)
                {
                    poResult->getAs<ImplicitList>()->setStep(_poSource->getAs<ImplicitList>()->getStep());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setStep(_poSource->getAs<ImplicitList>()->getStep());
                }

                if (_poSource->getAs<ImplicitList>()->getEndType() == types::InternalType::RealPoly)
                {
                    poResult->getAs<ImplicitList>()->setEnd(_poSource->getAs<ImplicitList>()->getEnd());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setEnd(_poSource->getAs<ImplicitList>()->getEnd());
                }
                break;
            }
            case types::GenericType::RealHandle :
                poResult->getAs<types::GraphicHandle>()->append(iCurRow, iCurCol, _poSource->getAs<types::GraphicHandle>());
                break;
            default:
                break;
        }
        return poResult;
    }
    return NULL;
}

const std::wstring* getStructNameFromExp(const Exp* _pExp)
{
    const FieldExp* pField =  dynamic_cast<const FieldExp*>(_pExp);
    const SimpleVar* pVar =  dynamic_cast<const SimpleVar*>(_pExp);
    const CallExp* pCall =  dynamic_cast<const CallExp*>(_pExp);

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
        throw ScilabError(os.str(), 999, _pExp->location_get());
    }
    return NULL;
}

bool getStructFromExp(const Exp* _pExp, types::InternalType** _pMain, types::InternalType** _pCurrent, typed_list** _pArgs, types::InternalType* _pIT)
{
    const FieldExp* pField      = dynamic_cast<const FieldExp*>(_pExp);
    const SimpleVar* pVar       = dynamic_cast<const SimpleVar*>(_pExp);
    const CallExp* pCall        = dynamic_cast<const CallExp*>(_pExp);
    const CellCallExp* pCell    = dynamic_cast<const CellCallExp*>(_pExp);

    if (pField)
    {
        //y.x

        //evaluate head "y"
        typed_list *pArgs       = NULL;
        InternalType* pMain     = *_pMain;
        InternalType* pCurrent  = *_pCurrent;

        bool bOK = getStructFromExp(pField->head_get(), &pMain, &pCurrent, &pArgs, NULL);
        if (bOK)
        {
            pVar    = dynamic_cast<const SimpleVar*>(pField->tail_get());

            if (pCurrent->isStruct())
            {
                Struct* pCurStr = pCurrent->getAs<Struct>();
                //clone _pIT BEFORE addField in case of st.b = st
                types::InternalType* pIT = _pIT ? _pIT->clone() : NULL;

                //create field "x"
                std::wstring var = pVar->name_get().name_get();
                bool bOK = pCurStr->addField(pVar->name_get().name_get());
                if (*_pMain == NULL && _pIT != NULL)
                {
                    //first stack, assign value to field and return main structure

                    if (pArgs != NULL && pArgs->size() != 0)
                    {
                        //args returned by "parent"
                        std::wstring var = pVar->name_get().name_get();
                        //be careful, extract functions copy values

                        Struct *pStr = pCurStr->extractWithoutClone(pArgs)->getAs<Struct>();
                        pStr->setCloneInCopyValue(false);
                        SingleStruct* pSS = pStr->get(0);
                        pSS->set(pVar->name_get().name_get(), pIT);
                        pSS->IncreaseRef();
                        delete pStr;
                        pSS->DecreaseRef();
                    }
                    else if (_pArgs == NULL || *_pArgs == NULL)
                    {
                        std::wstring var = pVar->name_get().name_get();
                        //std::wcout << var << L" <- " << pIT->getTypeStr() << std::endl;
                        pCurStr->get(0)->set(pVar->name_get().name_get(), pIT);
                    }
                    else
                    {
                        Struct* pStr = new Struct(1, 1);
                        std::wstring var = pVar->name_get().name_get();
                        pStr->addField(pVar->name_get().name_get());
                        pStr->get(0)->set(pVar->name_get().name_get(), pIT);
                        pCurStr->insertWithoutClone(*_pArgs, pStr->get(0));
                        delete pStr;
                    }
                }
                else
                {
                    //y.x.w
                    //in this case, we are in the middle of expression
                    //we know that "x" is a struct but we can't assign value yet
                    //so assign empty struct and return new pCurrent
                    Struct* pStr = NULL;

                    /*try to extract field*/
                    if (pArgs == NULL)
                    {
                        //without extract argument
                        pStr = pCurStr->get(0)->get(pVar->name_get().name_get())->getAs<Struct>();
                    }
                    else
                    {
                        Struct* pStepStr = pCurStr->extractWithoutClone(pArgs)->getAs<Struct>();
                        pStepStr->setCloneInCopyValue(false);
                        SingleStruct* pSS = pStepStr->get(0);
                        pStr = pSS->get(pVar->name_get().name_get())->getAs<Struct>();
                        //we can delete pStepStr without deleted its fields
                        pSS->IncreaseRef();
                        delete pStepStr;
                        pSS->DecreaseRef();
                    }

                    if (pStr == NULL)
                    {
                        //new field or not struct field
                        if (_pArgs == NULL || *_pArgs == NULL)
                        {
                            pStr = new Struct(1, 1);
                        }
                        else
                        {
                            Struct* p = new Struct(1, 1);
                            pStr = Struct::insertNew(*_pArgs, p)->getAs<Struct>();
                            delete p;
                        }

                        if (pArgs != NULL)
                        {
                            std::wstring var = pVar->name_get().name_get();

                            Struct* pStepStr = pCurStr->extractWithoutClone(pArgs)->getAs<Struct>();
                            pStepStr->setCloneInCopyValue(false);
                            SingleStruct* pSS = pStepStr->get(0);
                            pSS->set(pVar->name_get().name_get(), pStr);
                            pSS->IncreaseRef();
                            delete pStepStr;
                            pSS->DecreaseRef();
                        }
                        else
                        {
                            std::wstring var = pVar->name_get().name_get();
                            pCurStr->get(0)->set(pVar->name_get().name_get(), pStr);
                        }
                    }

                    pCurrent = pStr;
                }

                *_pMain = pMain;
                *_pCurrent = pCurrent;
            }
            else //handle
            {
                String* pTail = new String(pVar->name_get().name_get().c_str());
                if (_pArgs != NULL && *_pArgs == NULL)
                {
                    *_pArgs = new typed_list;
                    pArgs = *_pArgs;
                }
                else if (pArgs == NULL)
                {
                    pArgs = new typed_list;
                }

                if (pArgs)
                {
                    pArgs->push_back(pTail);
                    if (_pIT == NULL)
                    {
                        //let caller work

                        //try to extract, if extract work, clear args and flag we need set operation after.
                        GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                        typed_list in;
                        typed_list out;
                        optional_list opt;
                        ExecVisitor exec;

                        if (pArgs->size() == 1)
                        {
                            in.push_back((*pArgs)[0]);
                        }
                        else
                        {
                            List* pList = new List();
                            for (int i = 0 ; i < pArgs->size() ; i++)
                            {
                                pList->append((*pArgs)[i]);
                            }
                            in.push_back(pList);
                        }

                        in.push_back(pMain);
                        in.front()->IncreaseRef();
                        pMain->IncreaseRef();

                        Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"%h_e"));
                        Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, &exec);

                        in.front()->DecreaseRef();
                        pMain->DecreaseRef();

                        if (in.front()->isList())
                        {
                            //delete pList
                            delete in.front();
                        }

                        if (ret != Callable::OK)
                        {
                            std::wostringstream os;
                            os << _W("unable to update handle");
                            throw ScilabError(os.str(), 999, pField->location_get());
                        }

                        if (out[0]->isHandle() || out[0]->isStruct())
                        {
                            *_pCurrent = out[0];
                            (*_pCurrent)->IncreaseRef();

                            //clean *_pArgs to do nt extract previons fields
                            if (_pArgs && *_pArgs)
                            {
                                (*_pArgs)->clear();
                            }

                        }
                        else
                        {
                            *_pCurrent = pCurrent;
                        }

                        *_pMain = pMain;
                        return true;
                    }

                    //call %x_i_h
                    GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                    typed_list in;
                    typed_list out;
                    optional_list opt;
                    ExecVisitor exec;


                    if (pArgs->size() == 1)
                    {
                        in.push_back((*pArgs)[0]);
                    }
                    else
                    {
                        List* pList = new List();
                        for (int i = 0 ; i < pArgs->size() ; i++)
                        {
                            pList->append((*pArgs)[i]);
                        }
                        in.push_back(pList);
                    }

                    std::wstring str = L"%" + _pIT->getShortTypeStr() + L"_i_h";

                    in.push_back(_pIT);
                    in.push_back(pMain);
                    in.front()->IncreaseRef();
                    _pIT->IncreaseRef();
                    pMain->IncreaseRef();

                    Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(str));
                    Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, &exec);
                    in.front()->DecreaseRef();
                    //_pIT->DecreaseRef();
                    pMain->DecreaseRef();

                    if (in.front()->isList())
                    {
                        //delete pList
                        delete in.front();
                    }

                    if (ret != Callable::OK)
                    {
                        std::wostringstream os;
                        os << _W("unable to update handle");
                        throw ScilabError(os.str(), 999, pField->location_get());
                    }
                }
                else
                {
                    std::wostringstream os;
                    os << _W("impossible !");
                    throw ScilabError(os.str(), 999, pField->location_get());
                }

            }


            //clean pArgs return by getStructFromExp
            for (int iArg = 0 ; pArgs != NULL && iArg < pArgs->size() ; iArg++)
            {
                if ((*pArgs)[iArg]->isDeletable())
                {
                    delete (*pArgs)[iArg];
                }
            }
            delete pArgs;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (pVar)
    {
        //a.x : with x not only a SimpleVar
        types::InternalType *pStr = NULL;
        types::InternalType *pIT = symbol::Context::getInstance()->get(pVar->name_get());
        if (pIT == NULL ||
                (   pIT->isStruct() == false &&
                    pIT->isHandle() == false &&
                    pIT->isMList() == false &&
                    pIT->isTList() == false))
        {
            //"a" doest not exist or it is another type, create it with size 1,1 and return it
            //create new structure variable
            if (_pArgs == NULL || *_pArgs == NULL)
            {
                pStr = new types::Struct(1, 1);
            }
            else
            {
                if ((**_pArgs)[0]->isString())
                {
                    pStr = new types::Struct(1, 1);
                }
                else
                {
                    Struct* p = new Struct(1, 1);
                    pStr = Struct::insertNew(*_pArgs, p)->getAs<Struct>();
                    delete p;
                }
            }
            //Add variable to scope
            symbol::Context::getInstance()->put(pVar->name_get(), *pStr);
        }
        else if (pIT->isHandle() || pIT->isStruct())
        {
            pStr = pIT;
        }
        else
        {
            //TList or MList, work will be done outside
            return false;
        }

        if (*_pMain == NULL)
        {
            *_pMain = pStr;
        }
        *_pCurrent = pStr;
        return true;
    }
    else if (pCall)
    {
        //a(x,y)
        ExecVisitor execMe;
        InternalType* pCurrent = NULL;

        typed_list *pCurrentArgs = execMe.GetArgumentList(pCall->args_get());
        typed_list *pReturnedArgs = NULL;

        //Struct* pStruct = Struct::insertNew(pArgs, new Struct(1,1))->getAs<Struct>();
        if (*_pMain == NULL)
        {
            //a is the new main but can be a complex expression
            //bool bOK = getStructFromExp(&pCall->name_get(), _pMain, &pCurrent, &pArgs, pStruct);
            bool bOK = getStructFromExp(&pCall->name_get(), _pMain, &pCurrent, &pReturnedArgs, NULL);
            if (bOK == false)
            {
                return false;
            }

            if ((*pCurrentArgs)[0]->isString())
            {
                String* pS = (*pCurrentArgs)[0]->getAs<String>();
                if (pCurrentArgs->size() != 1 || pS->isScalar() == false)
                {
                    //manage error
                    std::wostringstream os;
                    os << _W("Invalid Index.\n");
                    //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, (*(pCall->args_get().begin()))->location_get());
                }

                wchar_t* pFieldName = pS->get(0);


                if (pCurrent->isStruct())
                {
                    Struct* pStr = NULL;
                    Struct *pCurStr = pCurrent->getAs<Struct>();
                    if (pReturnedArgs && (*pReturnedArgs)[0]->isString() == false)
                    {
                        pStr = pCurStr->extractWithoutClone(pReturnedArgs)->getAs<Struct>();
                        pStr->setCloneInCopyValue(false);
                    }
                    else
                    {
                        pStr = pCurStr;
                    }

                    SingleStruct* pSS = pStr->get(0);

                    //check if field already exists
                    if (pStr->exists(pFieldName))
                    {
                        InternalType* pField = pSS->get(pFieldName);
                        if (pField->isStruct())
                        {
                            pStr = pField->getAs<Struct>();
                        }
                        else
                        {
                            //erase previous value by a struct(1,1)
                            pSS->set(pFieldName, new Struct(1, 1));
                            pStr = pSS->get(pFieldName)->getAs<Struct>();
                        }
                    }
                    else
                    {
                        //field does not exist
                        pCurStr->addField(pFieldName);
                        pSS->set(pFieldName, new Struct(1, 1));
                        pCurrent = pSS->get(pFieldName);
                    }

                    if (pReturnedArgs && (*pReturnedArgs)[0]->isString() == false)
                    {
                        pSS->IncreaseRef();
                        delete pStr;
                        pSS->DecreaseRef();

                        //clean pReturnedArgs return by GetArgumentList
                        for (int iArg = 0 ; iArg < pReturnedArgs->size() ; iArg++)
                        {
                            if ((*pReturnedArgs)[iArg]->isDeletable())
                            {
                                delete (*pReturnedArgs)[iArg];
                            }
                        }
                        delete pReturnedArgs;
                    }
                }
                else
                {
                    //handle
                    GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                }
            }
            else
            {
                /*try to extract sub struct, if it fails, resize the struct and try again*/
                if (pCurrent->isStruct())
                {
                    Struct* pCurStr = pCurrent->getAs<Struct>();
                    InternalType* pIT = pCurStr->extract(pCurrentArgs);
                    if (pIT == NULL)
                    {
                        //fail to extract, pCurrent is not enough big, resize it !
                        Struct* p = new Struct(1, 1);
                        pCurStr->insert(pCurrentArgs, p); //insert empty struct, caller will assign the good value
                        delete p;
                    }
                    else
                    {
                        delete pIT;
                    }

                    *_pArgs = pCurrentArgs;
                }
                else
                {
                    //handle
                    GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                }
            }
        }
        else
        {
            //we have a parent, so assign "a" to this parent
            //(*_pMain)->set(0, pStruct->get(0));
        }
        *_pCurrent = pCurrent;
        return true;
    }
    else if (pCell)
    {
    }
    else
    {
        std::wostringstream os;
        os << _W("Unknown expression");
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ScilabError(os.str(), 999, _pExp->location_get());
    }
    return false;
}

void callOnPrompt(void)
{
    types::InternalType* pOnPrompt = NULL;
    pOnPrompt = symbol::Context::getInstance()->get(symbol::Symbol(L"%onprompt"));
    if (pOnPrompt != NULL && pOnPrompt->isCallable())
    {
        types::typed_list in;
        types::typed_list out;
        types::optional_list opt;
        ExecVisitor execCall;
        pOnPrompt->getAs<types::Callable>()->call(in, opt, 1, out, &execCall);
    }
}

List* getPropertyTree(Exp* e, List* pList)
{

    //a.b
    SimpleVar* pVar = dynamic_cast<SimpleVar*>(e);
    if (pVar)
    {
        pList->append(new String(pVar->name_get().name_get().c_str()));
        return pList;
    }

    //a(x).b
    CallExp* pCall = dynamic_cast<CallExp*>(e);
    if (pCall)
    {
        pList = getPropertyTree(&pCall->name_get(), pList);
        ExecVisitor exec;
        std::list<Exp*> l = pCall->args_get();
        std::list<Exp*>::const_iterator it;
        for (it = l.begin() ; it != l.end() ; it++)
        {
            Exp* pArg = (*it);
            try
            {
                pArg->accept(exec);
                pList->append(exec.result_get());
                exec.result_clear();
            }
            catch (ScilabException e)
            {
                throw e;
            }
        }

        return pList;
    }

    //a.b.c
    FieldExp* pField = dynamic_cast<FieldExp*>(e);
    if (pField)
    {
        pList = getPropertyTree(pField->head_get(), pList);
        pList = getPropertyTree(pField->tail_get(), pList);
        return pList;
    }

    return pList;
}
