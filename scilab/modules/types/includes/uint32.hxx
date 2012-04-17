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

// This code is separated in bool.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __UINT32_HXX__
#define __UINT32_HXX__

#include "arrayof.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP UInt32 : public ArrayOf<unsigned int>
    {
    public :
                                    UInt32(unsigned int _uiVal);
                                    UInt32(int _iRows, int _iCols);
                                    UInt32(int _iDims, int* _piDims);
                                    UInt32(int _iRows, int _iCols, unsigned int** _puiData);
                                    ~UInt32();

        InternalType*               clone();


        /*Config management*/
        void                        whoAmI();

        bool                        isInt() { return true; }
        bool                        isUInt32() { return true; }

        bool                        operator==(const InternalType& it);
        bool                        operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring        getTypeStr() {return L"uint32";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring        getShortTypeStr() {return L"i";}

    protected :
        RealType				    getType(void){return InternalType::RealUInt32;}

    private :
        virtual bool                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

        virtual unsigned int        getNullValue();
        virtual UInt32*             createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual unsigned int        copyValue(unsigned int _uiData);
        virtual void                deleteAll();
        virtual void                deleteImg();
        virtual unsigned int*       allocData(int _iSize);
    };
}

#ifdef _MSC_VER
template class TYPES_IMPEXP types::ArrayOf<unsigned int>;//UInt32
#endif

#endif /* !__UINT32_HXX__ */
