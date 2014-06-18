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

// This code is separated in implicitList.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __IMPLICITLIST_HXX__
#define __IMPLICITLIST_HXX__

#include "internal.hxx"
#include "double.hxx"
#include "dynlib_types.h"
#include "type_traits.hxx"

namespace types
{
/*
** Type
*/
class TYPES_IMPEXP ImplicitList : public InternalType
{
protected :
    InternalType*		    m_poStart;
    InternalType*		    m_poStep;
    InternalType*		    m_poEnd;

    Double*		            m_pDblStart;
    Double*		            m_pDblStep;
    Double*		            m_pDblEnd;

    int			            m_iSize;

    InternalType::ScilabType  m_eStartType;
    InternalType::ScilabType  m_eStepType;
    InternalType::ScilabType  m_eEndType;
    InternalType::ScilabType  m_eOutType;

    bool                    m_bComputed;

public :
    ImplicitList();
    virtual ~ImplicitList();
    ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd);

    InternalType*           clone();

    inline ScilabType       getType(void)
    {
        return ScilabImplicitList;
    }
    inline ScilabId         getId(void)
    {
        return IdImplicitList;
    }

    bool                    isImplicitList()
    {
        return true;
    }

    ScilabType                getOutputType();

    InternalType*           getStart();
    void                    setStart(InternalType*	_poIT);

    InternalType*           getStep();
    void                    setStep(InternalType* _poIT);

    InternalType*           getEnd();
    void                    setEnd(InternalType* _poIT);

    bool                    compute();
    bool                    isComputable();

    bool                    toString(std::wostringstream& ostr);


    InternalType::ScilabType  getStartType();
    InternalType::ScilabType  getStepType();
    InternalType::ScilabType  getEndType();

    long long               getSize();

    //extract single value in a InternalType
    InternalType*           extractValue(int _iOccur); //Single value
    double                  extractValueInDouble(int _iOccur);
    long long               extractValueInInteger(int _iOccur);
    unsigned long long      extractValueInUnsignedInteger(int _iOccur);

    //extract matrix in a Internaltype
    InternalType*           extractFullMatrix();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"implicitlist";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"";
    }

    // templated extract for
    // double / char / short / int / long long / unsigned ...
    template<typename T>
    void					extractFullMatrix(T *_pT);
    void                    extractFullMatrix(double *_pdbl);

    virtual bool isTrue()
    {
	// TODO : manage int & co
	if (m_poStart->isDouble() && m_poStep->isDouble() && m_poEnd->isDouble())
	{
	    return type_traits::isTrue(m_poStart->getAs<Double>()->get(0), m_poStep->getAs<Double>()->get(0), m_poEnd->getAs<Double>()->get(0));
	}

	return false;
    }

    virtual bool transpose(InternalType *& out);
    virtual bool neg(InternalType *& out);
};
}

#endif /* !__IMPLICITLIST_HXX__ */
