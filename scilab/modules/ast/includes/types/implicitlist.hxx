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

namespace types
{
/*
** Type
*/
class EXTERN_AST ImplicitList : public InternalType
{
protected :
    InternalType*		        m_poStart;
    InternalType*		        m_poStep;
    InternalType*		        m_poEnd;

    Double*		                m_pDblStart;
    Double*		                m_pDblStep;
    Double*		                m_pDblEnd;

    int			                m_iSize;

    InternalType::ScilabType    m_eStartType;
    InternalType::ScilabType    m_eStepType;
    InternalType::ScilabType    m_eEndType;
    InternalType::ScilabType    m_eOutType;

    bool                        m_bComputed;

public :
    ImplicitList();
    virtual ~ImplicitList();
    ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd);

    ImplicitList* clone();

    inline ScilabType getType(void)
    {
        return ScilabImplicitList;
    }
    inline ScilabId getId(void)
    {
        return IdImplicitList;
    }

    bool isImplicitList()
    {
        return true;
    }

    virtual bool isAssignable(void)
    {
        return true;
    }

    inline virtual bool isInvokable() const
    {
        return true;
    }


    ScilabType getOutputType();

    inline InternalType* getStart()
    {
        return m_poStart;
    }
    void setStart(InternalType*	_poIT);

    inline InternalType* getStep()
    {
        return m_poStep;
    }
    void setStep(InternalType* _poIT);

    inline InternalType* getEnd()
    {
        return m_poEnd;
    }
    void setEnd(InternalType* _poIT);

    bool compute();
    bool isComputable();

    bool toString(std::wostringstream& ostr);


    InternalType::ScilabType getStartType();
    InternalType::ScilabType getStepType();
    InternalType::ScilabType getEndType();

    inline long long getSize()
    {
        return m_iSize;
    }

    //extract single value in a InternalType
    void extractValue(int _iOccur, InternalType*); //Single value
    void extractValueAsDouble(int _iOccur, Double*);
    template<typename T>
    void extractValueAsInteger(int _iOccur, T* val);
    template<typename T>
    void extractValueAsUnsignedInteger(int _iOccur, T* val);

    InternalType* getInitalType();
    //extract matrix in a Internaltype
    InternalType* extractFullMatrix();

    virtual InternalType* extract(typed_list* _pArgs);
    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e) override ;


    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring getTypeStr()
    {
        return L"implicitlist";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring getShortTypeStr()
    {
        return L"ip";
    }

    // templated extract for
    // double / char / short / int / long long / unsigned ...
    template<typename T>
    void extractFullMatrix(T *_pT);
    void extractFullMatrix(Double *_pdbl);

    virtual bool isTrue();

    virtual bool transpose(InternalType *& out);
    virtual bool neg(InternalType *& out);
    virtual ast::Exp * getExp(const Location & loc);
};
}

#endif /* !__IMPLICITLIST_HXX__ */
