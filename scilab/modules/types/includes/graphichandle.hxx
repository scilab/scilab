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

#ifndef __GRAPHICHANDLE_HXX__
#define __GRAPHICHANDLE_HXX__

#include "arrayof.hxx"
#include "dynlib_types.h"


namespace types
{
    class TYPES_IMPEXP SingleHandle : public InternalType
    {
    private :
        char* m_pstHandleID;

    protected :
        char* getHandleID();
        void  setHandleID(char* _pstHandleID);


    public :
        SingleHandle();
        SingleHandle(char* _HandleID);
        ~SingleHandle();

        InternalType* clone();

        InternalType::RealType getType();

        bool isSingleHandle();

        bool toString(std::wostringstream& ostr);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring getTypeStr() {return L"handle";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"h";}

        bool operator==(const InternalType& it);
        bool operator!=(const InternalType& it);

    };
    
    class TYPES_IMPEXP GraphicHandle : public ArrayOf<SingleHandle*>
    {
    public :
                                GraphicHandle(SingleHandle* _pSingleHandle);
                                GraphicHandle(int _iRows, int _iCols);
                                GraphicHandle(int _iDims, int* _piDims);

        virtual                 ~GraphicHandle();

        InternalType*           clone();

        void                    whoAmI();

        bool                    isHandle() { return true; }

        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring     getTypeStr() {return L"handle";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring     getShortTypeStr() {return L"h";}
    
    protected :
        RealType				getType(void);

    private :
        virtual bool            subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

        virtual SingleHandle*   getNullValue();
        virtual GraphicHandle*  createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual SingleHandle*   copyValue(SingleHandle* _pData);
        virtual void            deleteAll();
        virtual void            deleteImg();
        virtual SingleHandle**  allocData(int _iSize);
    };
}

#ifdef _MSC_VER
template class TYPES_IMPEXP types::ArrayOf<types::SingleHandle*>; //GraphicHandle
#endif
#endif /* !__GRAPHICHANDLE_HXX__ */