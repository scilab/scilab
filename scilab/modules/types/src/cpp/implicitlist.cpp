/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <sstream>
#include <math.h>
#include "arrayof.hxx"

#include "core_math.h"
#include "tostring_common.hxx"

extern "C"
{
    #include "elem_common.h"
}

std::wstring printInLinePoly(types::SinglePoly* _pPoly, std::wstring _stVar, int _iPrecision, int _iLineLen);
std::wstring printDouble(types::Double* _pD, int _iPrecision, int _iLineLen);
long long convert_input(types::InternalType* _poIT);
unsigned long long convert_unsigned_input(types::InternalType* _poIT);

namespace types
{
    ImplicitList::~ImplicitList()
    {
        if(isDeletable() == true)
        {
            if(m_poStart)
            {
                m_poStart->DecreaseRef();
                if(m_poStart->isDeletable())
                {
                    delete m_poStart;
                }
            }

            if(m_poStep)
            {
                m_poStep->DecreaseRef();
                if(m_poStep->isDeletable())
                {
                    delete m_poStep;
                }
            }

            if(m_poEnd)
            {
                m_poEnd->DecreaseRef();
                if(m_poEnd->isDeletable())
                {
                    delete m_poEnd;
                }
            }
        }
    }

    ImplicitList::ImplicitList()
    {
        m_bComputed = false;
    }

    ImplicitList::ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd)
    {
        m_iSize = -1;
        m_eOutType = RealGeneric;
        setStart(_poStart);
        setStep(_poStep);
        setEnd(_poEnd);
        compute();
    }

    InternalType* ImplicitList::clone()
    {
        return new ImplicitList(m_poStart, m_poStep, m_poEnd);
    }

    InternalType::RealType ImplicitList::getStartType()
    {
        return m_poStart->getType();
    }

    InternalType::RealType ImplicitList::getStepType()
    {
        return m_poStep->getType();
    }

    InternalType::RealType ImplicitList::getEndType()
    {
        return m_poEnd->getType();
    }

    InternalType* ImplicitList::getStart()
    {
        return m_poStart;
    }

    void ImplicitList::setStart(InternalType *_poIT)
    {
        _poIT->IncreaseRef();
        m_poStart = _poIT;
        if(m_poStart != NULL)
        {
            m_eStartType = m_poStart->getType();
        }
        m_bComputed = false;
    }

    InternalType* ImplicitList::getStep()
    {
        return m_poStep;
    }

    void ImplicitList::setStep(InternalType *_poIT)
    {
        _poIT->IncreaseRef();
        m_poStep = _poIT;
        if(m_poStep != NULL)
        {
            m_eStepType = m_poStep->getType();
        }
        m_bComputed = false;
    }

    InternalType* ImplicitList::getEnd()
    {
        return m_poEnd;
    }

    void ImplicitList::setEnd(InternalType* _poIT)
    {
        _poIT->IncreaseRef();
        m_poEnd = _poIT;
        if(m_poEnd != NULL)
        {
            m_eEndType = m_poEnd->getType();
        }
        m_bComputed = false;
    }

    long long ImplicitList::getSize()
    {
        return m_iSize;
    }

    bool ImplicitList::compute()
    {
        if(m_bComputed == true)
        {
            return true;
        }

        m_iSize = -1;
        if(isComputable() == true)
        {
            m_iSize = 0;
            if(m_eOutType == RealDouble)
            {
                double dblStart	= m_poStart->getAs<Double>()->getReal(0,0);
                double dblStep	= m_poStep->getAs<Double>()->getReal(0,0);
                double dblEnd   = m_poEnd->getAs<Double>()->getReal(0,0);

                if(dblStep > 0)
                {
                    m_iSize = static_cast<int>(floor((dblEnd - dblStart) / dblStep)) + 1;
                }
                else if(dblStep < 0)
                {
                    m_iSize = static_cast<int>(floor((dblStart - dblEnd) / -dblStep)) + 1;
                }
                else
                {
                    m_iSize = 0;
                }

                if(m_iSize < 0)
                {
                    m_iSize = 0;
                }
//                m_iSize = static_cast<long long>(floor(fabs(dblEnd - dblStart) / fabs(dblStep))) + 1;
            }
            else //m_eOutType == RealInt
            {
                if(m_eOutType == RealInt8 ||
                    m_eOutType == RealInt16 ||
                    m_eOutType == RealInt32 ||
                    m_eOutType == RealInt64)
                {//signed
                    long long llStart   = convert_input(m_poStart);
                    long long llStep    = convert_input(m_poStep);
                    long long llEnd     = convert_input(m_poEnd);

#ifdef _MSC_VER
                    m_iSize = static_cast<int>(floor( static_cast<double>(_abs64(llEnd - llStart) / _abs64(llStep)) )) + 1;
#else
                    m_iSize = static_cast<int>(floor( static_cast<double>(llabs(llEnd - llStart) / llabs(llStep)) )) + 1;
#endif
                }
                else
                {//unsigned
                    unsigned long long ullStart = convert_unsigned_input(m_poStart);
                    unsigned long long ullStep	= convert_unsigned_input(m_poStep);
                    unsigned long long ullEnd   = convert_unsigned_input(m_poEnd);

#ifdef _MSC_VER
                    m_iSize = static_cast<int>(floor(static_cast<double>(_abs64(ullEnd - ullStart) / _abs64(ullStep)) )) + 1;
#else
                    m_iSize = static_cast<int>(floor(static_cast<double>(llabs(ullEnd - ullStart) / llabs(ullStep)) )) + 1;
#endif
                }
            }
            m_bComputed = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ImplicitList::isComputable()
    {
        if(m_eStartType != RealDouble && m_poStart->isInt() == false)
        {
            return false;
        }

        if(m_eStepType != RealDouble && m_poStep->isInt() == false)
        {
            return false;
        }

        if(m_eEndType != RealDouble && m_poEnd->isInt() == false)
        {
            return false;
        }

        //"compute" output type
        m_eOutType = RealGeneric; //not defined type
        if(m_poStart->isInt())
        {
            m_eOutType  = m_poStart->getType();
        }
        else if(m_poStep->isInt())
        {
            m_eOutType  = m_poStep->getType();
        }
        else if(m_poEnd->isInt())
        {
            m_eOutType  = m_poEnd->getType();
        }
        else
        {
            m_eOutType  = RealDouble;
        }

        return true;
    }

    std::wstring ImplicitList::toString(int _iPrecision, int _iLineLen)
    {
        if(isComputable())
        {
            return extractFullMatrix()->toString(_iPrecision, _iLineLen);
        }
        else
        {
            std::wostringstream ostr;
            ostr << L" ";
            if(m_eStartType == RealDouble)
            {
                Double *pD = m_poStart->getAs<Double>();
                ostr << printDouble(pD, _iPrecision, _iLineLen);
            }
            else //Polynom
            {
                Polynom* pMP = m_poStart->getAs<types::Polynom>();
                ostr << printInLinePoly(pMP->get(0), pMP->getVariableName(), _iPrecision, _iLineLen);
            }

            ostr << L":";

            if(m_eStepType == RealDouble)
            {
                Double *pD = m_poStep->getAs<Double>();
                ostr << printDouble(pD, _iPrecision, _iLineLen);
            }
            else //Polynom
            {
                Polynom* pMP = m_poStep->getAs<types::Polynom>();
                ostr << printInLinePoly(pMP->get(0), pMP->getVariableName(), _iPrecision, _iLineLen);
            }

            ostr << L":";

            if(m_eEndType == RealDouble)
            {
                Double *pD = m_poEnd->getAs<Double>();
                ostr << printDouble(pD, _iPrecision, _iLineLen);
            }
            else //Polynom
            {
                Polynom* pMP = m_poEnd->getAs<types::Polynom>();
                ostr << printInLinePoly(pMP->get(0), pMP->getVariableName(), _iPrecision, _iLineLen);
            }
            ostr << std::endl;
            return ostr.str();
        }
    }

    InternalType::RealType ImplicitList::getOutputType()
    {
        return m_eOutType;
    }

    double ImplicitList::extractValueInDouble(int _iOccur)
    {
        double dblStart		= m_poStart->getAs<Double>()->getReal(0,0);
        double dblStep		= m_poStep->getAs<Double>()->getReal(0,0);
        return dblStart + _iOccur * dblStep;
    }

    long long ImplicitList::extractValueInInteger(int _iOccur)
    {
        return convert_input(m_poStart) + _iOccur * convert_input(m_poStep);
    }

    unsigned long long ImplicitList::extractValueInUnsignedInteger(int _iOccur)
    {
        return convert_unsigned_input(m_poStart) + _iOccur * convert_unsigned_input(m_poStep);
    }

    //extract single value in a InternalType
    InternalType* ImplicitList::extractValue(int _iOccur)
    {
        InternalType* pIT = NULL;
        if(compute())
        {
            long long llVal             = extractValueInInteger(_iOccur);
            unsigned long long ullVal   = extractValueInUnsignedInteger(_iOccur);
            if(m_eOutType == RealInt8)
            {
                pIT	= new Int8((char)llVal);
            }
            else if(m_eOutType == RealUInt8)
            {
                pIT	= new UInt8((unsigned char)ullVal);
            }
            else if(m_eOutType == RealInt16)
            {
                pIT	= new Int16((short)llVal);
            }
            else if(m_eOutType == RealUInt16)
            {
                pIT	= new UInt16((unsigned short)ullVal);
            }
            else if(m_eOutType == RealInt32)
            {
                pIT	= new Int32((int)llVal);
            }
            else if(m_eOutType == RealUInt32)
            {
                pIT	= new UInt32((unsigned int)ullVal);
            }
            else if(m_eOutType == RealInt64)
            {
                pIT	= new Int64((long long)llVal);
            }
            else if(m_eOutType == RealUInt64)
            {
                pIT	= new UInt64((unsigned long long)ullVal);
            }
            else //RealDouble
            {
                double dblStart = m_poStart->getAs<Double>()->getReal(0,0);
                double dblStep  = m_poStep->getAs<Double>()->getReal(0,0);
                Double* pD      = new Double(dblStart + _iOccur * dblStep);
                pIT = pD;
            }
        }
        return pIT;
    }

    //extract matrix in a Internaltype
    InternalType* ImplicitList::extractFullMatrix()
    {
        InternalType* pIT = NULL;
        if(compute())
        {
            if(m_eOutType == RealInt8)
            {
                pIT	= new Int8(1, m_iSize);
                extractFullMatrix(pIT->getAs<Int8>()->get());
            }
            else if(m_eOutType == RealUInt8)
            {
                pIT	= new UInt8(1, m_iSize);
                extractFullMatrix(pIT->getAs<UInt8>()->get());
            }
            else if(m_eOutType == RealInt16)
            {
                pIT	= new Int16(1, m_iSize);
                extractFullMatrix(pIT->getAs<Int16>()->get());
            }
            else if(m_eOutType == RealUInt16)
            {
                pIT	= new UInt16(1, m_iSize);
                extractFullMatrix(pIT->getAs<UInt16>()->get());
            }
            else if(m_eOutType == RealInt32)
            {
                pIT	= new Int32(1, m_iSize);
                extractFullMatrix(pIT->getAs<Int32>()->get());
            }
            else if(m_eOutType == RealUInt32)
            {
                pIT	= new UInt32(1, m_iSize);
                extractFullMatrix(pIT->getAs<UInt32>()->get());
            }
            else if(m_eOutType == RealInt64)
            {
                pIT	= new Int64(1, m_iSize);
                extractFullMatrix(pIT->getAs<Int64>()->get());
            }
            else if(m_eOutType == RealUInt64)
            {
                pIT	= new UInt64(1, m_iSize);
                extractFullMatrix(pIT->getAs<UInt64>()->get());
            }
            else //RealDouble
            {
                pIT = new Double(1, m_iSize);
                extractFullMatrix(pIT->getAs<Double>()->get());
            }

        }
        return pIT;
    }

    template<typename T>
    void ImplicitList::extractFullMatrix(T *_pT)
    {
        T tStart = static_cast<T>(convert_input(m_poStart));
        T tStep	= static_cast<T>(convert_input(m_poStep));

        for(int i = 0 ; i < m_iSize ; i++)
        {
            _pT[i] = tStart + tStep * i;
        }
    }
}

std::wstring printInLinePoly(types::SinglePoly* _pPoly, std::wstring _stVar, int _iPrecision, int _iLineLen)
{
    std::wostringstream ostr;
    for(int i = 0 ; i < _pPoly->getRank() ; i++)
    {
        double dbl = _pPoly->getCoef()->getReal()[i];
        if(dbl != 0)
        {
            int iWidth = 0, iPrec = 0;
            bool bFP = false; // FloatingPoint
            getDoubleFormat(dbl, _iPrecision, &iWidth, &iPrec, &bFP);
            addDoubleValue(&ostr, dbl, iWidth, iPrec, ostr.str().size() != 0, i == 0, false);
            if(i != 0)
            {
                ostr <<_stVar;
                if(i > 1)
                {
                    ostr << "^" << i;
                }
            }
        }
    }
    return ostr.str();
}

std::wstring printDouble(types::Double* _pD, int _iPrecision, int _iLineLen)
{
    std::wostringstream ostr;
    int iWidth = 0, iPrec = 0;
    bool bFP = false; // FloatingPoint
    getDoubleFormat(_pD->getReal(0,0), _iPrecision, &iWidth, &iPrec, &bFP);
    addDoubleValue(&ostr, _pD->getReal(0,0), iWidth, iPrec, false, true, false);
    return ostr.str();
}

long long convert_input(types::InternalType* _poIT)
{
    long long llValue = 0;
    switch(_poIT->getType())
    {
    case types::GenericType::RealDouble :
        llValue = static_cast<long long>(_poIT->getAs<types::Double>()->get(0));
        break;
    case types::GenericType::RealInt8 :
        llValue = static_cast<long long>(_poIT->getAs<types::Int8>()->get(0));
        break;
    case types::GenericType::RealUInt8 :
        llValue = static_cast<long long>(_poIT->getAs<types::UInt8>()->get(0));
        break;
    case types::GenericType::RealInt16 :
        llValue = static_cast<long long>(_poIT->getAs<types::Int16>()->get(0));
        break;
    case types::GenericType::RealUInt16 :
        llValue = static_cast<long long>(_poIT->getAs<types::UInt16>()->get(0));
        break;
    case types::GenericType::RealInt32 :
        llValue = static_cast<long long>(_poIT->getAs<types::Int32>()->get(0));
        break;
    case types::GenericType::RealUInt32 :
        llValue = static_cast<long long>(_poIT->getAs<types::UInt32>()->get(0));
        break;
    case types::GenericType::RealInt64 :
        llValue = static_cast<long long>(_poIT->getAs<types::Int64>()->get(0));
        break;
    case types::GenericType::RealUInt64 :
        llValue = static_cast<long long>(_poIT->getAs<types::UInt64>()->get(0));
        break;
    default:
        // FIXME : Trigger an error ??
        break;
    }
    return llValue;
}

unsigned long long convert_unsigned_input(types::InternalType* _poIT)
{
    unsigned long long ullValue = 0;
    switch(_poIT->getType())
    {
    case types::GenericType::RealDouble :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::Double>()->get(0));
        break;
    case types::GenericType::RealInt8 :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::Int8>()->get(0));
        break;
    case types::GenericType::RealUInt8 :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::UInt8>()->get(0));
        break;
    case types::GenericType::RealInt16 :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::Int16>()->get(0));
        break;
    case types::GenericType::RealUInt16 :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::UInt16>()->get(0));
        break;
    case types::GenericType::RealInt32 :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::Int32>()->get(0));
        break;
    case types::GenericType::RealUInt32 :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::UInt32>()->get(0));
        break;
    case types::GenericType::RealInt64 :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::Int64>()->get(0));
        break;
    case types::GenericType::RealUInt64 :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::UInt64>()->get(0));
        break;
    default:
        // FIXME : Trigger an error ??
        break;
    }
    return ullValue;
}
