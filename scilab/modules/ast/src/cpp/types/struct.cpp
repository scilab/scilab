/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "struct.hxx"
#include "string.hxx"
#include "double.hxx"
#include "list.hxx"
#include "int.hxx"
#include "scilabWrite.hxx"
#include "exp.hxx"
#include "types_tools.hxx"

namespace types
{
Struct::Struct()
{
    m_bDisableCloneInCopyValue = false;
    SingleStruct** pIT  = NULL;
    int piDims[2] = {0, 0};
    create(piDims, 2, &pIT, NULL);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Struct::Struct(int _iRows, int _iCols)
{
    m_bDisableCloneInCopyValue = false;
    SingleStruct** pIT  = NULL;
    SingleStruct *p = new SingleStruct();
    int piDims[2] = {_iRows, _iCols};
    create(piDims, 2, &pIT, NULL);
    for (int i = 0 ; i < getSize() ; i++)
    {
        set(i, p);
    }

    p->killMe();
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Struct::Struct(int _iDims, const int* _piDims)
{
    m_bDisableCloneInCopyValue = false;
    SingleStruct** pIT  = NULL;
    SingleStruct *p = new SingleStruct();
    create(_piDims, _iDims, &pIT, NULL);
    for (int i = 0 ; i < getSize() ; i++)
    {
        set(i, p);
    }
    p->killMe();

#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Struct::~Struct()
{
    if (isDeletable() == true)
    {
        for (int i = 0 ; i < m_iSizeMax ; i++)
        {
            SingleStruct *pStr = m_pRealData[i];
            if (pStr)
            {
                pStr->DecreaseRef();
                pStr->killMe();
            }
        }

        delete[] m_pRealData;
    }
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

Struct::Struct(Struct *_oStructCopyMe)
{
    m_bDisableCloneInCopyValue = false;
    SingleStruct** pIT = NULL;
    create(_oStructCopyMe->getDimsArray(), _oStructCopyMe->getDims(), &pIT, NULL);
    for (int i = 0 ; i < getSize() ; i++)
    {
        pIT[i] = _oStructCopyMe->get(i)->clone();
        pIT[i]->IncreaseRef();
    }
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Struct* Struct::clone()
{
    return new Struct(this);
}

bool Struct::transpose(InternalType *& out)
{
    if (isScalar())
    {
        out = clone();
        return true;
    }

    if (m_iDims == 2)
    {
        int piDims[2] = {getCols(), getRows()};
        Struct * pSt = new Struct(2, piDims);
        out = pSt;
        for (int i = 0; i < m_iSize; ++i)
        {
            pSt->m_pRealData[i]->DecreaseRef();
            pSt->m_pRealData[i]->killMe();
        }

        Transposition::transpose_clone(getRows(), getCols(), m_pRealData, pSt->m_pRealData);

        return true;
    }

    return false;
}

bool Struct::extract(const std::string & name, InternalType *& out)
{
    if (exists(name))
    {
        out = extractField(name);
    }
    else
    {
        char szError[bsiz];
        os_sprintf(szError, _("Unknown field : %ls.\n"), name.c_str());
        throw std::string(szError);
    }

    return true;
}

bool Struct::invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e)
{
    if (in.size() == 0)
    {
        out.push_back(this);
        return true;
    }
    else if (in.size() == 1)
    {
        InternalType * arg = in[0];
        std::vector<InternalType *> _out;
        if (arg->isString())
        {
            std::vector<std::string> stFields;
            String * pString = arg->getAs<types::String>();
            for (int i = 0; i < pString->getSize(); ++i)
            {
                std::string stField(pString->get(i));
                if (this->exists(stField))
                {
                    stFields.push_back(stField);
                }
                else
                {
                    char szError[bsiz];
                    os_sprintf(szError, _("Field \"%s\" does not exists\n"), stField.c_str());
                    throw ast::InternalError(szError, 999, e.getLocation());
                }
            }

            _out = extractFields(stFields);
            if (_out.size() == 1)
            {
                InternalType * pIT = _out[0];
                if (pIT->isList() && pIT->getAs<List>()->getSize() == 1)
                {
                    out.push_back(pIT->getAs<List>()->get(0));
                    return true;
                }
            }

            out.swap(_out);
            return true;
        }
    }

    return ArrayOf<SingleStruct*>::invoke(in, opt, _iRetCount, out, e);
}

Struct* Struct::set(int _iRows, int _iCols, SingleStruct* _pIT)
{
    if (_iRows < getRows() && _iCols < getCols())
    {
        return set(_iCols * getRows() + _iRows, _pIT);
    }
    return NULL;
}

Struct* Struct::set(int _iRows, int _iCols, const SingleStruct* _pIT)
{
    if (_iRows < getRows() && _iCols < getCols())
    {
        return set(_iCols * getRows() + _iRows, _pIT);
    }
    return NULL;
}

Struct* Struct::set(int _iIndex, SingleStruct* _pIT)
{
    typedef Struct* (Struct::*set_t)(int, SingleStruct*);
    Struct* pIT = checkRef(this, (set_t)&Struct::set, _iIndex, _pIT);
    if (pIT != this)
    {
        return pIT;
    }

    if (_iIndex < getSize())
    {
        if (m_bDisableCloneInCopyValue && m_pRealData[_iIndex] == _pIT)
        {
            return this;
        }

        InternalType* pOld = m_pRealData[_iIndex];

        m_pRealData[_iIndex] = copyValue(_pIT);
        if (m_bDisableCloneInCopyValue == false)
        {
            //only in clone mode
            m_pRealData[_iIndex]->IncreaseRef();
        }

        if (pOld != NULL)
        {
            pOld->DecreaseRef();
            pOld->killMe();
        }

        return this;
    }
    return NULL;
}

Struct* Struct::set(int _iIndex, const SingleStruct* _pIT)
{
    typedef Struct* (Struct::*set_t)(int, const SingleStruct*);
    Struct* pIT = checkRef(this, (set_t)&Struct::set, _iIndex, _pIT);
    if (pIT != this)
    {
        return pIT;
    }

    if (_iIndex < getSize())
    {
        InternalType* pOld = m_pRealData[_iIndex];

        m_pRealData[_iIndex] = const_cast<SingleStruct*>(_pIT)->clone();

        if (pOld != NULL)
        {
            pOld->DecreaseRef();
            pOld->killMe();
        }

        return this;
    }
    return NULL;
}

Struct* Struct::set(SingleStruct** _pIT)
{
    typedef Struct* (Struct::*set_t)(SingleStruct**);
    Struct* pIT = checkRef(this, (set_t)&Struct::set, _pIT);
    if (pIT != this)
    {
        return pIT;
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        if (set(i, _pIT[i]) == NULL)
        {
            return NULL;
        }
    }
    return this;
}

String* Struct::getFieldNames()
{
    if (getSize() != 0)
    {
        return get(0)->getFieldNames();
    }
    else
    {
        return NULL;
    }
}

bool Struct::exists(const std::string& _sKey)
{
    if (getSize() != 0)
    {
        return get(0)->exists(_sKey);
    }
    else
    {
        return false;
    }
}

bool Struct::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isStruct() == false)
    {
        return false;
    }

    Struct* pStr = const_cast<InternalType &>(it).getAs<Struct>();

    for (int i = 0 ; i < getDims() ; i++)
    {
        if (pStr->getDimsArray()[i] != getDimsArray()[i])
        {
            return false;
        }
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        if (*get(i) != *pStr->get(i))
        {
            return false;
        }
    }
    return true;
}

bool Struct::operator!=(const InternalType& it)
{
    return !(*this == it);
}

SingleStruct* Struct::getNullValue()
{
    return new SingleStruct();
}

Struct* Struct::createEmpty(int _iDims, int* _piDims, bool /*_bComplex*/)
{
    Struct* pStr = new Struct(_iDims, _piDims);
    pStr->setCloneInCopyValue(!m_bDisableCloneInCopyValue);
    return pStr;
}

SingleStruct* Struct::copyValue(SingleStruct* _pData)
{
    SingleStruct* pStr = NULL;
    if (m_bDisableCloneInCopyValue)
    {
        pStr = _pData;
        pStr->IncreaseRef();
        //std::wcout << "copyValueWithoutClone -> " << pStr << " : " << pStr->getRef() << std::endl;
    }
    else
    {
        pStr = _pData->clone();
    }

    return pStr;
}

void Struct::deleteAll()
{
    for (int i = 0 ; i < getSize() ; i++)
    {
        m_pRealData[i]->DecreaseRef();
        m_pRealData[i]->killMe();
    }
    delete[] m_pRealData;
    m_pRealData = NULL;
}

void Struct::deleteImg()
{
    return;
}

bool Struct::isEmpty()
{
    if (getDims() == 2 && getRows() == 0 && getCols() == 0)
    {
        return true;
    }
    return false;
}

SingleStruct** Struct::allocData(int _iSize)
{
    SingleStruct** pData = new SingleStruct*[_iSize];
    for (int i = 0 ; i < _iSize ; i++)
    {
        pData[i] = NULL;
    }
    return pData;
}

bool Struct::subMatrixToString(std::ostringstream& /*ostr*/, int* /*_piDims*/, int /*_iDims*/)
{
    return true;
}

Struct* Struct::addField(const std::string& _sKey)
{
    Struct* pIT = checkRef(this, &Struct::addField, _sKey);
    if (pIT != this)
    {
        return pIT;
    }

    if (getSize() == 0)
    {
        //change dimension to 1x1 and add field
        resize(1, 1);
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        get(i)->addField(_sKey);
    }

    return this;
}

Struct* Struct::addFieldFront(const std::string& _sKey)
{
    Struct* pIT = checkRef(this, &Struct::addFieldFront, _sKey);
    if (pIT != this)
    {
        return pIT;
    }

    if (getSize() == 0)
    {
        //change dimension to 1x1 and add field
        resize(1, 1);
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        get(i)->addFieldFront(_sKey);
    }

    return this;
}

Struct* Struct::removeField(const std::string& _sKey)
{
    Struct* pIT = checkRef(this, &Struct::removeField, _sKey);
    if (pIT != this)
    {
        return pIT;
    }

    for (int j = 0; j < getSize(); j++)
    {
        get(j)->removeField(_sKey);
    }

    return this;
}

bool Struct::toString(std::ostringstream& ostr)
{
    if (getSize() == 0)
    {
        ostr << "0x0 struct array with no field.";
    }
    else if (getSize() == 1)
    {
        SingleStruct* pSS =  get(0);
        String* pstFields =  pSS->getFieldNames();
        if (pstFields->getSize() == 0)
        {
            ostr << "1x1 struct array with no field.";
        }

        for (int i = 0 ; i < pstFields->getSize() ; i++)
        {
            std::string wstField(pstFields->get(i));
            InternalType* pIT = pSS->get(wstField);

            //                ostr << "  " << wstField << ": ";
            ostr << "  " << wstField << ": ";
            ostr << pIT->toStringInLine();
            ostr << std::endl;
        }
        pstFields->killMe();;
    }
    else
    {
        ostr << "  ";
        for (int i = 0 ; i < m_iDims ; i++)
        {
            if (i > 0)
            {
                ostr << "x";
            }
            ostr << m_piDims[i];
        }
        ostr << " struct array with ";

        String* pwstFields = getFieldNames();
        ostr <<  L"fields:" << std::endl;
        for (int i = 0 ; i < pwstFields->getSize() ; i++)
        {
            ostr << "    " << pwstFields->get(i) << std::endl;
        }
        pwstFields->killMe();
    }

    return true;
}

List* Struct::extractFieldWithoutClone(const std::string& _stField)
{
    List* pL = new List();
    for (int j = 0 ; j < getSize() ; j++)
    {
        pL->set(j, get(j)->get(_stField));
    }

    return pL;
}

std::vector<InternalType*> Struct::extractFields(const std::vector<std::string>& _stFields)
{
    std::vector<InternalType*> ResultList;

    for (int i = 0 ; i < (int)_stFields.size() ; i++)
    {
        ResultList.push_back(extractField(_stFields[i]));
    }

    return ResultList;
}

InternalType * Struct::extractField(const std::string & wstField)
{
    if (wstField == "dims")
    {
        Int32 * pDims = new Int32(1, getDims());
        for (int j = 0 ; j < getDims() ; j++)
        {
            pDims->set(j, getDimsArray()[j]);
        }

        return pDims;
    }
    else
    {
        if (getSize() == 1)
        {
            return get(0)->get(wstField);
        }
        else
        {
            List * pL = new List();
            for (int j = 0 ; j < getSize() ; j++)
            {
                pL->append(get(j)->get(wstField));
            }

            return pL;
        }
    }
}

std::vector<InternalType*> Struct::extractFields(typed_list* _pArgs)
{
    std::vector<InternalType*> ResultList;

    int iDims           = (int)_pArgs->size();
    typed_list pArg;

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    delete[] piMaxDim;
    delete[] piCountDim;

    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        ResultList.push_back(createEmptyDouble());
        return ResultList;
    }

    Double* pIndex = pArg[0]->getAs<Double>();

    for (int i = 0 ; i < iSeqCount ; i++)
    {
        int iIndex = (int)pIndex->get(i);

        if (iIndex == 1)
        {
            //struct fields name
            String* pS = getFieldNames();
            String* pFields = NULL;
            if (pS)
            {
                pFields = new String(1, pS->getSize() + 2);
                for (int j = 0; j < pS->getSize(); j++)
                {
                    pFields->set(2 + j, pS->get(j));
                }

                pS->killMe();
            }
            else
            {
                pFields = new String(1, 2);
            }

            pFields->set(0, "st");
            pFields->set(1, "dims");

            ResultList.push_back(pFields);
        }
        else if (iIndex == 2)
        {
            //struct dims
            Int32* pDims = new Int32(1, getDims());
            for (int j = 0 ; j < getDims() ; j++)
            {
                pDims->set(j, getDimsArray()[j]);
            }

            ResultList.push_back(pDims);
        }
        else if (getSize() == 0)
        {
            break;
        }
        else if (iIndex > (int)get(0)->getNumFields() + 2)
        {
            break;
        }
        else if (getSize() == 1)
        {
            //return elements
            const std::vector<InternalType*> & pData = get(0)->getData();
            ResultList.push_back(pData[iIndex - 3]->clone());
        }
        else
        {
            //return each elements for sub structs in a list
            List* pL = new List();

            for (int j = 0 ; j < getSize() ; j++)
            {
                //-2 for fieldlist and dims, -1 for indexed at 0
                const std::vector<InternalType*> & pData = get(j)->getData();
                pL->append(pData[iIndex - 3]->clone());
            }

            ResultList.push_back(pL);
        }
    }

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);
    return ResultList;
}

Struct* Struct::resize(int _iNewRows, int _iNewCols)
{
    int piDims[2] = {_iNewRows, _iNewCols};
    return resize(piDims, 2);
}

Struct* Struct::resize(int* _piDims, int _iDims)
{
    typedef Struct* (Struct::*resize_t)(int*, int);
    Struct* pIT = checkRef(this, (resize_t)&Struct::resize, _piDims, _iDims);
    if (pIT != this)
    {
        return pIT;
    }

    m_bDisableCloneInCopyValue = true;
    Struct* pSRes = ArrayOf<SingleStruct*>::resize(_piDims, _iDims)->getAs<Struct>();
    m_bDisableCloneInCopyValue = false;
    if (pSRes)
    {
        // insert field(s) only in new element(s) of current struct
        String* pFields = getFieldNames();
        for (int iterField = 0; iterField < pFields->getSize(); iterField++)
        {
            for (int iterStruct = 0; iterStruct < getSize(); iterStruct++)
            {
                get(iterStruct)->addField(pFields->get(iterField));
            }
        }

        pFields->killMe();
    }

    return pSRes;
}

InternalType* Struct::insertWithoutClone(typed_list* _pArgs, InternalType* _pSource)
{
    //std::wcout << "insertWithoutClone start" << std::endl;
    m_bDisableCloneInCopyValue = true;
    InternalType* pIT = insert(_pArgs, _pSource);
    _pSource->IncreaseRef();
    //std::wcout << "insertWithoutClone -> " << _pSource << " : " << _pSource->getRef() << std::endl;
    m_bDisableCloneInCopyValue = false;
    //std::wcout << "insertWithoutClone end" << std::endl;
    return pIT;
}

InternalType* Struct::extractWithoutClone(typed_list* _pArgs)
{
    //std::wcout << "extractWithoutClone start" << std::endl;
    m_bDisableCloneInCopyValue = true;
    InternalType* pIT = extract(_pArgs);
    m_bDisableCloneInCopyValue = false;
    //std::wcout << "extractWithoutClone end" << std::endl;
    return pIT;
}

void Struct::setCloneInCopyValue(bool _val)
{
    m_bDisableCloneInCopyValue = !_val;
}

void Struct::deleteData(SingleStruct* data)
{
    if (data)
    {
        data->killMe();
    }
}

Struct* Struct::createEmpty()
{
    return new Struct();
}
}
