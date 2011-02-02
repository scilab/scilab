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

#ifndef __ARRAYOF_HXX__
    #error This file must only be include by arrayof.hxx
#endif

namespace types
{
    /*
    ** Type
    */
    class ImplicitList : public InternalType
    {
    protected :
        InternalType*		    m_poStart;
        InternalType*		    m_poStep;
        InternalType*		    m_poEnd;
        int			            m_iSize;

        InternalType::RealType  m_eStartType;
        InternalType::RealType  m_eStepType;
        InternalType::RealType  m_eEndType;
        InternalType::RealType  m_eOutType;

        bool                    m_bComputed;

    public :
        ImplicitList();
        virtual ~ImplicitList();
        ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd);

        InternalType*           clone();

        ImplicitList*           getAsImplicitList(void) { return this; }
        RealType                getType(void) { return RealImplicitList; }
        bool                    isImplicitList() { return true; }

        RealType                getOutputType();

        InternalType*           getStart();
        void                    setStart(InternalType*	_poIT);

        InternalType*           getStep();
        void                    setStep(InternalType* _poIT);

        InternalType*           getEnd();
        void                    setEnd(InternalType* _poIT);

        bool                    compute();
        bool                    isComputable();

        std::wstring            toString(int _iPrecision, int _iLineLen);


        InternalType::RealType  getStartType();
        InternalType::RealType  getStepType();
        InternalType::RealType  getEndType();

        long long               getSize();

        //extract single value in a InternalType
        InternalType*           extractValue(int _iOccur); //Single value
        double                  extractValueInDouble(int _iOccur);
        long long               extractValueInInteger(int _iOccur);
        unsigned long long      extractValueInUnsignedInteger(int _iOccur);

        //extract matrix in a Internaltype
        InternalType*           extractFullMatrix();

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring    getTypeStr() {return L"implicitlist";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"";}

        // templated extract for
        // double / char / short / int / long long / unsigned ...
        template<typename T>
        void					extractFullMatrix(T *_pT);
    };
}

